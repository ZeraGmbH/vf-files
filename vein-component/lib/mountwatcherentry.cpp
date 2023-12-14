#include "mountwatcherentry.h"
#include <QDir>
#include <QRegularExpression>
#include <QThread>
#include <poll.h>
#include <unistd.h>

using namespace vfFiles;

extern void appendErrorMsg(QString& errorMsg, const QString appendError);

MountWatcherWorker::MountWatcherWorker(const QString procFileMount, const QString &mountBasePath, QObject *parent) :
    QObject(parent),
    m_procFileMount(procFileMount),
    m_mountBasePath(!mountBasePath.endsWith(QStringLiteral("/")) ? mountBasePath+QStringLiteral("/") : mountBasePath)
{
}

void MountWatcherWorker::startPoll()
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

void MountWatcherWorker::readProcFile(QFile &procFile)
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
    emit mountChanged(newList);
}

MountWatcherEntry::MountWatcherEntry(QObject *parent) : QObject(parent)
{
}

bool MountWatcherEntry::create(VfCpp::VfCppEntity *entity,
                               const QString componentName,
                               const QString procFileMount,
                               const QString mountBasePath)
{
    // Plausis first
    QString strErrorMsg;
    QFile tmpFile(procFileMount);
    if(!tmpFile.exists()) {
        appendErrorMsg(strErrorMsg, QStringLiteral("MountWatcherEntry::create: Could not find proc-file: ") + procFileMount);
    }
    QDir tmpDir(mountBasePath);
    if(!tmpDir.exists()) {
        appendErrorMsg(strErrorMsg, QStringLiteral("MountWatcherEntry::create: Could not find mount-base-path: ") + mountBasePath);
    }

    bool ok = strErrorMsg.isEmpty();
    if(ok) {
        // prepare polling thread
        QThread *thread = new QThread(this);
        MountWatcherWorker *worker = new MountWatcherWorker(procFileMount, mountBasePath);
        worker->moveToThread(thread);

        // thread start(/stop)
        QObject::connect(thread, &QThread::started, worker, &MountWatcherWorker::startPoll, Qt::QueuedConnection);
        QObject::connect(thread, &QThread::finished, worker, &MountWatcherWorker::deleteLater); // very unlikely & thread context

        // mount changes
        QObject::connect(worker, &MountWatcherWorker::mountChanged, this, &MountWatcherEntry::onMountsChanged, Qt::QueuedConnection);

        // make chickens like me feel more confortable - part 1
        m_threadID = QThread::currentThreadId();

        // create vein component
        m_veinComponent = entity->createComponent(componentName, QStringList(), true);

        thread->start();
    }
    else {
        qWarning("%s", qPrintable(strErrorMsg)); // warnings created by << are quoted which is ugly
    }
    return ok;
}

void MountWatcherEntry::onMountsChanged(const QStringList mounts)
{
    Q_ASSERT(m_threadID == QThread::currentThreadId()); // make chickens like me feel more confortable - part 2
    if(m_veinComponent.value() != mounts) {
        m_veinComponent.setValue(mounts);
    }
}
