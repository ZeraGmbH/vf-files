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
    terminate();
    wait();
}

void MountWatcherThread::startWatch(const QString procFileMount, const QString &mountBasePath)
{
    m_procFileMount = procFileMount;
    m_mountBasePath = !mountBasePath.endsWith(QStringLiteral("/")) ? mountBasePath+QStringLiteral("/") : mountBasePath;
    start();
}

void MountWatcherThread::run()
{
    struct pollfd ev;
    int ret;

    // QFileSystemWatcher does not work on '/etc/mtab' see [1] (that is where
    // the deep down parts of code were taken from
    // [1] https://stackoverflow.com/questions/1113176/how-could-i-detect-when-a-directory-is-mounted-with-inotify
    QFile procFile(m_procFileMount);
    if(procFile.open(QIODevice::ReadOnly)) {
        // initial read
        readProcFile(procFile);
        // infinite loop waking poll once watched file changed
        do {
            ev.events = POLLERR | POLLPRI;
            ev.fd = procFile.handle();
            ev.revents = 0;
            ret = poll(&ev, 1, -1);
            lseek(procFile.handle(), 0, SEEK_SET);
            if (ev.revents & POLLERR) {
                // on change read
                readProcFile(procFile);
            }
        } while (ret >= 0);
    }
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
