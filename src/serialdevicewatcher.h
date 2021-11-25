#ifndef SERIALDEVICEWATCHER_H
#define SERIALDEVICEWATCHER_H

#include <QObject>
#include <QTimer>
#include <QJsonObject>
#include <vf-cpp-entity.h>
#include <vf-cpp-component.h>

namespace vfFiles {

using VfCpp::cVeinModuleComponent;

class SerialDeviceWatcher : public QObject
{
    Q_OBJECT
public:
    explicit SerialDeviceWatcher(QObject *parent = nullptr);
    bool create(VfCpp::veinmoduleentity* entity,
                const QString componentName);

private:
    void onTimer();

    cVeinModuleComponent::Ptr m_veinComponent = nullptr;
    QTimer m_periodicPollTimer;

};

}// namespace

#endif // SERIALDEVICEWATCHER_H
