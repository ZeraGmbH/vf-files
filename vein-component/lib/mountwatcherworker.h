#ifndef MOUNTWATCHERWORKER_H
#define MOUNTWATCHERWORKER_H

#include <QObject>
#include <QFile>

namespace vfFiles {

class MountWatcherWorker : public QObject
{
    Q_OBJECT
public:
    explicit MountWatcherWorker(const QString procFileMount, const QString &mountBasePath, QObject *parent = nullptr);

signals:
    void mountChanged(QStringList mounts);

public slots:
    void startPoll();

private:
    void readProcFile(QFile &procFile);

    QString m_procFileMount;
    QString m_mountBasePath;
    QStringList m_currentMounts;
};

}
#endif // MOUNTWATCHERWORKER_H
