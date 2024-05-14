#ifndef MOUNTWATCHERENTRYBASE_H
#define MOUNTWATCHERENTRYBASE_H

#include <QObject>
#include <QThread>

namespace vfFiles {

class MountWatcherEntryBase : public QObject
{
    Q_OBJECT
public:
    explicit MountWatcherEntryBase(QObject *parent = nullptr);
    ~MountWatcherEntryBase();
    bool create(const QString procFileMount,
                const QString mountBasePath);
signals:
    void sigMountsChanged(const QStringList mounts);

private:
    bool m_created = false;
    QThread m_thread;
};

}

#endif // MOUNTWATCHERENTRYBASE_H
