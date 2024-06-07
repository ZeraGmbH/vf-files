#include "mountwatcherentrybase.h"
#include <QDir>

using namespace vfFiles;

extern void appendErrorMsg(QString& errorMsg, const QString appendError);

MountWatcherEntryBase::MountWatcherEntryBase(QObject *parent) :
    QObject(parent),
    m_thread(this)
{
}

MountWatcherEntryBase::~MountWatcherEntryBase()
{
    m_thread.terminate();
    m_thread.wait();
}

bool MountWatcherEntryBase::create(const QString procFileMount, const QString mountBasePath)
{
    // Plausis first
    QString strErrorMsg;
    if(m_created)
        appendErrorMsg(strErrorMsg, QStringLiteral("MountWatcherEntry::create cannot be called more than once"));
    QFile tmpFile(procFileMount);
    if(!tmpFile.exists())
        appendErrorMsg(strErrorMsg, QStringLiteral("MountWatcherEntry::create: Could not find proc-file: ") + procFileMount);
    QDir tmpDir(mountBasePath);
    if(!tmpDir.exists())
        appendErrorMsg(strErrorMsg, QStringLiteral("MountWatcherEntry::create: Could not find mount-base-path: ") + mountBasePath);

    bool ok = strErrorMsg.isEmpty();
    if(ok) {
        m_created = true;
        m_thread.startWatch(procFileMount, mountBasePath);
    }
    else
        qWarning("%s", qPrintable(strErrorMsg)); // warnings created by << are quoted which is ugly
    return ok;
}
