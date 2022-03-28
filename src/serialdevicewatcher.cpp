#include <QSerialPortInfo>
#include <QDir>
#include "serialdevicewatcher.h"

namespace vfFiles {

SerialDeviceWatcher::SerialDeviceWatcher(QObject *parent) : QObject(parent)
{
}

bool SerialDeviceWatcher::create(VfCpp::veinmoduleentity *entity, const QString componentName)
{
    bool created = false;
    if(!m_veinComponent) {
        m_veinComponent = entity->createComponent(componentName, QJsonObject(), true);
        connect(&m_periodicPollTimer, &QTimer::timeout,
                this, &SerialDeviceWatcher::onTimer);
        m_periodicPollTimer.start(300);
        created = true;
    }
    return created;
}

void SerialDeviceWatcher::onTimer()
{
    // Performance measurements showed that QSerialPortInfo::availablePorts()
    // take ages and slows down system. So try /dev/ttyUSB* first
    QDir ttySysClassDir(QStringLiteral("/sys/class/tty"));
    ttySysClassDir.setNameFilters(QStringList("ttyUSB*"));
    const QFileInfoList fileInfos = ttySysClassDir.entryInfoList();
    if(fileInfos != m_LastUSBSerialDevsFound) {
        m_LastUSBSerialDevsFound = fileInfos;
        m_usbChangeDetected = true;
    }
    else if(m_usbChangeDetected) {
        m_usbChangeDetected = false;
        updateUsbSerialDevicesDetails();
    }
}

void SerialDeviceWatcher::updateUsbSerialDevicesDetails()
{
    QJsonObject json;
    QList<QSerialPortInfo> portInfos = QSerialPortInfo::availablePorts();
    for(auto portInfo : portInfos) {
        QString portName = portInfo.portName();
        if(portName.contains("ttyUSB")) {
            QJsonObject jsonEntry;
            jsonEntry.insert("description", portInfo.description());
            jsonEntry.insert("manufacturer", portInfo.manufacturer());
            jsonEntry.insert("serial", portInfo.serialNumber());
            QString systemLocation = portInfo.systemLocation();
            json.insert(systemLocation, jsonEntry);
        }
    }
    m_veinComponent->setValue(json);
}

}// namespace
