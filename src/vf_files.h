#ifndef VF_FILES_H
#define VF_FILES_H

#include <veinmoduleentity.h>
#include <QObject>
#include <QDir>

// TODO: PIMPL!!!

namespace vfFiles {

/**
 * @brief The vf_files class
 *
 * is an vein module useable with zera-modulemanager that offers helpers
 * for file handling accessible by vein.
 *
 *
 * Setup:
 *
 * @code
 * vf_files* obj=new vf_files(<veinId>)
 *
 * QTimer::singleShot(0, [][]{
 *      obj->initOnce()
 *      // watch external drives mounted to /media
 *      obj->addDirToWatch(QStringLiteral("/media"), QStringLiteral("MountedExternalDrives"));
 * })
 * @endcode
 *
 */

class DirWatcherEntry;

class vf_files : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief vf_files creates entity
     * @param parent
     * @param id: the vein entity id
     */
    explicit vf_files(QObject *parent = nullptr, int id=4);

    /**
     * @brief initOnce
     * @return true on success
     *
     * sets up entity with EntityName component only
     */
    bool initOnce();
    /**
     * @brief addDirToWatch: create a component containing a list of files/dirs of a directory
     * @param dir: the directory to scan
     * @param componentName: the vein component name
     * @param nameFilters: list of filters to reduce matches - check QDir::setNameFilters for more details
     * @param filters: see QDir::Filters
     * @param fullPathForResults: true: set full path in search hit list / false: just file/dir name
     * @return
     */
    bool addDirToWatch(const QString dir,
                       const QString componentName,
                       const QStringList nameFilters = QStringList(),
                       QDir::Filters filters = QDir::NoDotAndDotDot,
                       bool fullPathForResults = false);
    bool addDefaultPathComponent(const QString dir, const QString componentName, bool createDir=false);

    /**
     * @brief getVeinEntity
     * @return the actual entity
     *
     * Take the return value and add it to the top EventHandler system
     */
    VfCpp::veinmoduleentity *getVeinEntity() const;

private:
    VfCpp::veinmoduleentity *m_entity;
    bool m_isInitalized;
};

}
#endif // VF_FILES_H
