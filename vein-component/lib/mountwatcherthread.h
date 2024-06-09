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
    ~MountWatcherThread() override;
    void startWatch(const QString procFileMount, const QString &mountBasePath);
private:
    void run() override;
    void readProcFile(QFile &procFile);

    MountWatcherEntryBase* m_watcher;
    QFile m_procFileMount;
    int m_threadAlivePipeWhileOpen[2];

    QString m_mountBasePath;
    QStringList m_currentMounts;
};

}
#endif // MOUNTWATCHERTHREAD_H
