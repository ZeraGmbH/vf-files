#include "mountwatcherentrybase.h"
#include "mountwatcherworker.h"
#include <QDir>
#include <QThread>

using namespace vfFiles;

extern void appendErrorMsg(QString& errorMsg, const QString appendError);

MountWatcherEntryBase::MountWatcherEntryBase(QObject *parent) :
    QObject(parent)
{
}

bool MountWatcherEntryBase::create(const QString procFileMount, const QString mountBasePath)
{
    // Plausis first
    QString strErrorMsg;
    QFile tmpFile(procFileMount);
    if(!tmpFile.exists())
        appendErrorMsg(strErrorMsg, QStringLiteral("MountWatcherEntry::create: Could not find proc-file: ") + procFileMount);
    QDir tmpDir(mountBasePath);
    if(!tmpDir.exists())
        appendErrorMsg(strErrorMsg, QStringLiteral("MountWatcherEntry::create: Could not find mount-base-path: ") + mountBasePath);

    bool ok = strErrorMsg.isEmpty();
    if(ok) {
        // prepare polling thread
        QThread *thread = new QThread();
        MountWatcherWorker *worker = new MountWatcherWorker(procFileMount, mountBasePath);
        worker->moveToThread(thread);

        // thread start(/stop)
        QObject::connect(thread, &QThread::started, worker, &MountWatcherWorker::startPoll, Qt::QueuedConnection);
        QObject::connect(thread, &QThread::finished, worker, &MountWatcherWorker::deleteLater);

        // mount changes
        QObject::connect(worker, &MountWatcherWorker::mountChanged, this, &MountWatcherEntryBase::sigMountsChanged, Qt::QueuedConnection);

        thread->start();
    }
    else
        qWarning("%s", qPrintable(strErrorMsg)); // warnings created by << are quoted which is ugly
    return ok;
}
