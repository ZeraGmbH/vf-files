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
 *      obj->addDirToWatch(QStringLiteral("MountedExternalDrives"), QStringLiteral("/media"));
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
     * @param componentName: vein component name
     * @param dir: the directory to scan
     * @param nameFilters: list of filters to reduce matches - check QDir::setNameFilters for more details
     * @param filters: see QDir::Filters
     * @param fullPathForResults: true: set full path in search hit list / false: just file/dir name
     * @return true on success
     */
    bool addDirToWatch(const QString componentName,
                       const QString dir,
                       const QStringList nameFilters = QStringList(),
                       QDir::Filters filters = QDir::NoDotAndDotDot,
                       bool fullPathForResults = false);
    /**
     * @brief addDefaultPathComponent: create a component containing a system wide default directory
     * @param componentName: vein component name
     * @param dir: full path of the directory
     * @param createDir: true: create direcrory
     * @return true on success
     */
    bool addDefaultPathComponent(const QString componentName, const QString dir, bool createDir=false);

    /**
     * @brief getVeinEntity
     * @return the actual entity
     *
     * Take the return value and add it to the top EventHandler system
     */
    VfCpp::veinmoduleentity *getVeinEntity() const;

public slots:
    /**
     * @brief RPC_CopyFile
     * @param p_params: QString p_source / QString p_dest / bool p_overwrite
     * @return true on success
     */
    QVariant RPC_CopyFile(QVariantMap p_params);
    /**
     * @brief RPC_GetDriveInfo
     * @param p_params: QString p_mountDir / QString p_localeName / bool p_getDriveName / bool p_getMemTotal / bool p_getMemAvail
     * @note p_localeName is ignored in case p_getMemTotal and p_getMemAvail are not set
     * @return QStringList list of mount information. Entries are prefixed by "name:" / "total:" / "avail:"
     */
    QVariant RPC_GetDriveInfo(QVariantMap p_params);

private:
    VfCpp::veinmoduleentity *m_entity;
    bool m_isInitalized;
};

}
#endif // VF_FILES_H
