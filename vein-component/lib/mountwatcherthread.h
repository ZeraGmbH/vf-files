#ifndef MOUNTWATCHERTHREAD_H
#define MOUNTWATCHERTHREAD_H

#include <QThread>
#include <QFile>

namespace vfFiles {

class MountWatcherEntryBase;

class MountWatcherThread : public QThread
{
    Q_OBJECT
public:
    MountWatcherThread(MountWatcherEntryBase* watcher);
    void startWatch(const QString procFileMount, const QString &mountBasePath);
private:
    void run() override;
    void readProcFile(QFile &procFile);

    MountWatcherEntryBase* m_watcher;
    QString m_procFileMount;
    QString m_mountBasePath;
    QStringList m_currentMounts;
};

}
#endif // MOUNTWATCHERTHREAD_H
