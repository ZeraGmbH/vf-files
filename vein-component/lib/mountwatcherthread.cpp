#include "mountwatcherthread.h"
#include "mountwatcherentrybase.h"
#include <QRegularExpression>
#include <poll.h>
#include <unistd.h>

namespace vfFiles {

MountWatcherThread::MountWatcherThread(MountWatcherEntryBase *watcher) :
    m_watcher(watcher)
{
}

MountWatcherThread::~MountWatcherThread()
{
    close(m_threadAlivePipeWhileOpen[0]);
    close(m_threadAlivePipeWhileOpen[1]);
    wait();
}

void MountWatcherThread::startWatch(const QString procFileMount, const QString &mountBasePath)
{
    pipe(m_threadAlivePipeWhileOpen);
    m_procFileMount.setFileName(procFileMount);
    if(m_procFileMount.open(QIODevice::ReadOnly)) {
        m_mountBasePath = !mountBasePath.endsWith(QStringLiteral("/")) ? mountBasePath+QStringLiteral("/") : mountBasePath;
        start();
    }
    else
        qWarning("Could not open %s", qPrintable(procFileMount));
}

void MountWatcherThread::run()
{
    // QFileSystemWatcher does not work on '/etc/mtab'
    // Inspired by [1]
    // [1] https://stackoverflow.com/questions/1113176/how-could-i-detect-when-a-directory-is-mounted-with-inotify

    // initial read
    readProcFile(m_procFileMount);
    int ret;
    // infinite loop waking poll once watched file changed or pipe write end closed
    do {
        struct pollfd fds[2];
        fds[0] = {m_threadAlivePipeWhileOpen[1], 0, 0};
        fds[1] = {m_procFileMount.handle(), 0, 0};

        ret = poll(fds, 2, -1);
        if(fds[0].revents & POLLERR)
            return;
        if(fds[1].revents & POLLERR) {
            lseek(m_procFileMount.handle(), 0, SEEK_SET);
            // on change read
            readProcFile(m_procFileMount);
        }
    } while (ret >= 0);
}

void MountWatcherThread::readProcFile(QFile &procFile)
{
    QByteArray procfileContent = procFile.readAll();
    QStringList newList;
    // https://verwaltung.uni-koeln.de/stabsstelle01/content/benutzerberatung/it_faq/windows/faqitems163122/index_ger.html
    // https://doc.qt.io/qt-6/qregularexpression.html
    // Alternatively, you can use a raw string literal, in which case you don't need to escape backslashes in the pattern,
    // all characters between R"(...)" are considered raw characters.
    QRegularExpression regEx(QStringLiteral(" ") + m_mountBasePath + QStringLiteral(R"([-_0-9A-Za-z\/\\]*)"));
    QRegularExpressionMatchIterator matchIter = regEx.globalMatch(procfileContent);
    while (matchIter.hasNext()) {
        QRegularExpressionMatch match = matchIter.next();
        newList.append(match.captured().replace(QStringLiteral(" "), QString()));
    }
    if(m_currentMounts != newList) {
        m_currentMounts = newList;
        QMetaObject::invokeMethod(m_watcher,
                                  "sigMountsChanged",
                                  Qt::QueuedConnection,
                                  Q_ARG(QStringList, newList));
    }
}

}
