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
     * @brief addMountToWatch: create a component containing a list of mounted paths
     * @param componentName: vein component name
     * @param procFileMount: name of proc file to watch - somthing like '/etc/mtab' / '/proc/mounts' / '/proc/self/mounts'
     * @param filters: see QDir::Filters
     * @param fullPathForResults: true: set full path in search hit list / false: just file/dir name
     * @return true on success
     */
    bool addMountToWatch(const QString componentName,
                         const QString mountBasePath,
                         const QString procFileMount = QStringLiteral("/etc/mtab"));

    /**
     * @brief addDefaultPathComponent: create a component containing a system wide default directory ending with dir separator
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
     * @brief RPC_CopyDirFiles: Simple - non recursive copy of directory
     * @param p_params: QString p_sourceDir / QString p_destDir / QStringList p_nameFilters / bool p_cleanDestFirst / bool p_overwrite
     * @return true on success
     */
    QVariant RPC_CopyDirFiles(QVariantMap p_params);

    /**
     * @brief setFindLimits Set maximum limits for RPC_FindFileSpecial to avoid explosion of recursion
     * @param maxRecursionDepth Maximum recursion depth. Check immediate on call of RPC_FindFileSpecial
     * @param findMaxHitsDirs Maximum number of dirs to find
     * @param findMaxHitsFiles Maximum number of files to find
     */
    void setFindLimits(const int maxRecursionDepth = 3, const int findMaxHitsDirs=100, const int findMaxHitsFiles = 500);
    /**
     * @brief RPC_FindFileSpecial: Tailored file find - recursive so handle with caution
     *
     * This RPC finds matching files in matching directoriers. Some notes on p_nameFilterList:
     * @note Each list entry contains one or more filters that are separated by ';'
     * @note The first entries are applied on directories / the last on files
     *
     * A note on p_returnMatchingDirsOnly:
     * @note If set only paths containing matching files are returned. Dir-seperators are NOT appended
     *
     * @example
     * To find paths with customer data to import we set
     * p_baseDir="/media/sda1"
     * p_nameFilterList={"zera-*-?????????", "customerdata", "*.json"}
     * p_returnMatchingDirsOnly=true
     * or json style
     * {
     *   "p_baseDir": "/media/sda1",
     *   "p_nameFilterList": [ "zera-*-?????????", "customerdata", "*.json" ],
     *   "p_returnMatchingDirsOnly": true
     * }
     *
     * @param p_params: QString p_baseDir / QStringList p_nameFilterList / bool p_returnMatchingDirsOnly
     * @return QStringList list of files matching
     */
    QVariant RPC_FindFileSpecial(QVariantMap p_params);

    /**
     * @brief RPC_DeleteFile guess what
     * @param p_params QString p_fullPathFile
     * @return true on success
     */
    QVariant RPC_DeleteFile(QVariantMap p_params);

    /**
     * @brief setDataSizeFormat: Set format for representation of data quantities in RPC_GetDriveInfo/RPC_GetFileInfo
     * @param dataSizeflags: default is QLocale::DataSizeSIFormat - see QLocale::DataSizeFormats for further details
     */
    void setDataSizeFormat(QLocale::DataSizeFormats dataSizeflags);

    /**
     * @brief RPC_GetDriveInfo: Return name and size information for a mounted drive
     * @param p_params: QString p_mountDir / QString p_localeName
     * @note p_localeName due to returning localized string something as 'de_DE' or 'en_GB' is expected
     * @return QStringList list of mount information. Entries are prefixed by "name:" / "total:" / "avail:"
     */
    QVariant RPC_GetDriveInfo(QVariantMap p_params);

    /**
     * @brief RPC_GetFileInfo: Return information of a file given in param
     * @param p_params: QString p_fileName / QString p_localeName
     * @note p_localeName due to returning localized string something as 'de_DE' or 'en_GB' is expected
     * @return QStringList list file information. Entries are prefixed by "p_fileName:" / "fullname:" / "filename:" / "size:"
     */
    QVariant RPC_GetFileInfo(QVariantMap p_params);

private:
    // TODO PIMPL
    void findInPath(const QString &baseDir,
                    const QStringList &nameFilterList,
                    const int currFilterOrPathDepth,
                    const bool returMatchingDirsOnly,
                    QStringList &resultList,
                    QString &strError);


    VfCpp::veinmoduleentity *m_entity;
    QLocale::DataSizeFormats m_dataSizeflags;
    int m_findMaxRecursionDepth;
    int m_findMaxHitsDirs;
    int m_findMaxHitsFiles;
    bool m_isInitalized;
};

}
#endif // VF_FILES_H
