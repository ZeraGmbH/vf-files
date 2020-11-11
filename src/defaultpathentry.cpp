#include "defaultpathentry.h"
#include <QDir>

DefaultPathEntry::DefaultPathEntry(QObject *parent) : QObject(parent)
{
}

bool DefaultPathEntry::create(VfCpp::veinmoduleentity *entity, const QString componentName, const QString path, const bool create)
{
    bool ok = true;
    // first try to create if we are asked for
    if(create) {
        // same return if path exists ot not
        QDir dir;
        ok = dir.mkpath(path);
    }
    if(ok) {
        m_veinComponent = entity->createComponent(componentName, path, true);
    }
    return ok;
}
