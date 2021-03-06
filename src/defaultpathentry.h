#ifndef DEFAULTPATHENTRY_H
#define DEFAULTPATHENTRY_H

#include <QObject>
#include <veinmoduleentity.h>
#include <veincompproxy.h>

class DefaultPathEntry : public QObject
{
    Q_OBJECT
public:
    explicit DefaultPathEntry(QObject *parent = nullptr);
    bool create(VfCpp::veinmoduleentity* entity,
                const QString componentName,
                const QString path,
                const bool create);

private:
    VfCpp::VeinCompProxy<QStringList> m_veinComponent;

};

#endif // DEFAULTPATHENTRY_H
