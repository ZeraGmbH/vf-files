#ifndef DEFAULTPATHENTRY_H
#define DEFAULTPATHENTRY_H

#include <QObject>
#include <vfcpp.h>

class DefaultPathEntry : public QObject
{
    Q_OBJECT
public:
    explicit DefaultPathEntry(QObject *parent = nullptr);
    bool create(VfCpp::VeinModuleEntity* entity,
                const QString componentName,
                const QString path,
                const bool create);

private:
    VfCpp::VeinSharedComp<QStringList> m_veinComponent;

};

#endif // DEFAULTPATHENTRY_H
