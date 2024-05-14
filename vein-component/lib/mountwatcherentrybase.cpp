#include "mountwatcherentrybase.h"
#include "mountwatcherworker.h"
#include <QDir>

using namespace vfFiles;

extern void appendErrorMsg(QString& errorMsg, const QString appendError);

MountWatcherEntryBase::MountWatcherEntryBase(QObject *parent) :
    QObject(parent)
{
}

MountWatcherEntryBase::~MountWatcherEntryBase()
{
    m_thread.quit();
    m_thread.wait();
}

bool MountWatcherEntryBase::create(const QString procFileMount, const QString mountBasePath)
{
    // Plausis first
    QString strErrorMsg;
    if(m_created)
        appendErrorMsg(strErrorMsg, QStringLiteral("MountWatcherEntry::create cannot be called more thanm once"));
    QFile tmpFile(procFileMount);
    if(!tmpFile.exists())
        appendErrorMsg(strErrorMsg, QStringLiteral("MountWatcherEntry::create: Could not find proc-file: ") + procFileMount);
    QDir tmpDir(mountBasePath);
    if(!tmpDir.exists())
        appendErrorMsg(strErrorMsg, QStringLiteral("MountWatcherEntry::create: Could not find mount-base-path: ") + mountBasePath);

    bool ok = strErrorMsg.isEmpty();
    if(ok) {
        m_created = true;
        // prepare polling thread
        MountWatcherWorker *worker = new MountWatcherWorker(procFileMount, mountBasePath);
        worker->moveToThread(&m_thread);

        // thread start(/stop)
        QObject::connect(&m_thread, &QThread::started, worker, &MountWatcherWorker::startPoll, Qt::QueuedConnection);

        // mount changes
        QObject::connect(worker, &MountWatcherWorker::mountChanged, this, &MountWatcherEntryBase::sigMountsChanged, Qt::QueuedConnection);

        m_thread.start();
    }
    else
        qWarning("%s", qPrintable(strErrorMsg)); // warnings created by << are quoted which is ugly
    return ok;
}
