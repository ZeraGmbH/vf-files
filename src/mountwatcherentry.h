#ifndef MOUNTWATCHERENTRY_H
#define MOUNTWATCHERENTRY_H

#include <vfcpp.h>
#include <QFile>

namespace vfFiles {

class MountWatcherEntry : public QObject
{
    Q_OBJECT
public:
    explicit MountWatcherEntry(QObject *parent = nullptr);
    bool create(VfCpp::VeinModuleEntity* entity,
                const QString componentName,
                const QString procFileMount,
                const QString mountBasePath);

public slots:
    void onMountsChanged(const QStringList mounts);

private:
    VfCpp::VeinSharedComp<QStringList> m_veinComponent;
    Qt::HANDLE m_threadID;
};

class MountWatcherWorker : public QObject {
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

}// namespace

#endif // MOUNTWATCHERENTRY_H
