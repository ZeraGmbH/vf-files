#include "vf_files.h"
#include "dirwatcherentry.h"
#include "mountwatcherentry.h"
#include "defaultpathentry.h"
#include <QStorageInfo>

using namespace vfFiles;

vf_files::vf_files(QObject *parent, int id) : QObject(parent),
    m_entity(new VfCpp::veinmoduleentity(id)),
    m_dataSizeflags(QLocale::DataSizeSIFormat),
    m_isInitalized(false)
{
    setFindLimits();
}

bool vf_files::initOnce()
{
    // create our entity / rpcs once
    if(!m_isInitalized) {
        m_isInitalized = true;
        m_entity->initModule();
        m_entity->createComponent("EntityName", "_Files", true);
        m_entity->createRpc(this, "RPC_CopyFile",
                            VfCpp::cVeinModuleRpc::Param({
                                                             {"p_source", "QString"},
                                                             {"p_dest", "QString"},
                                                             {"p_overwrite", "bool"}}));
        m_entity->createRpc(this, "RPC_CopyDirFiles",
                            VfCpp::cVeinModuleRpc::Param({
                                                             {"p_sourceDir", "QString"},
                                                             {"p_destDir", "QString"},
                                                             {"p_nameFilters", "QStringList"},
                                                             {"p_cleanDestFirst", "bool"},
                                                             {"p_overwrite", "bool"}}));
        m_entity->createRpc(this, "RPC_FindFileSpecial",
                            VfCpp::cVeinModuleRpc::Param({
                                                             {"p_baseDir", "QString"},
                                                             {"p_nameFilterList", "QStringList"},
                                                             {"p_returnMatchingDirsOnly", "bool"}}));
        m_entity->createRpc(this, "RPC_DeleteFile",
                            VfCpp::cVeinModuleRpc::Param({
                                                             {"p_fullPathFile", "QString"}}));
        m_entity->createRpc(this, "RPC_GetDriveInfo",
                            VfCpp::cVeinModuleRpc::Param({
                                                             {"p_mountDir", "QString"},
                                                             {"p_localeName", "QString"},
                                                             {"p_getDriveName", "bool"},
                                                             {"p_getMemTotal", "bool"},
                                                             {"p_getMemFree", "bool"}}));
    }
    return true;
}

void appendErrorMsg(QString& errorMsg, const QString appendError)
{
    if(!errorMsg.isEmpty()) {
        errorMsg += QStringLiteral(" / ");
    }
    errorMsg += appendError;
}

QVariant vf_files::RPC_CopyFile(QVariantMap p_params)
{
    QString strError;

    QString sourceFile = p_params["p_source"].toString();
    QString destFile = p_params["p_dest"].toString();
    bool p_overwrite = p_params["p_overwrite"].toBool();

    bool bSourceExists = QFile::exists(sourceFile);
    bool bDestExists = QFile::exists(destFile);

    // check some obvious plausis first
    if(!bSourceExists) {
        appendErrorMsg(strError, QStringLiteral("RPC_CopyFile: ") + sourceFile + QStringLiteral(" does not exist"));
    }
    if(!p_overwrite && bDestExists) {
        appendErrorMsg(strError, QStringLiteral("RPC_CopyFile: Cannot overwrite ") + destFile);
    }

    // ensure dest directory is there
    if(strError.isEmpty()) {
        QFileInfo destFileInfo(destFile);
        QDir destDir = destFileInfo.absoluteDir();
        if(!destDir.mkpath(destDir.path())) {
            appendErrorMsg(strError, QStringLiteral("RPC_CopyFile: Cannot create directory ") + destDir.path());
        }
    }
    // do copy
    if(strError.isEmpty()) {
        QFile file(sourceFile);
        // we checked plausis above / copy cannot overwrite
        if(bDestExists) {
            file.remove(destFile);
        }
        if(!file.copy(destFile)) {
            appendErrorMsg(strError, QStringLiteral("RPC_CopyFile: ") + sourceFile + QStringLiteral(" was not copied to ") + destFile);
        }
    }
    bool ok = strError.isEmpty();
    if(!ok) {
        // drop a note to whoever listens (usually journal)
        qWarning("%s", qPrintable(strError));
    }
    return ok;
}

QVariant vf_files::RPC_CopyDirFiles(QVariantMap p_params)
{
    QString sourceDirPath = p_params["p_sourceDir"].toString();
    QString destDirPath = p_params["p_destDir"].toString();
    QStringList nameFilters = p_params["p_nameFilters"].toStringList();
    bool cleanDestFirst = p_params["p_cleanDestFirst"].toBool();
    bool overwrite = p_params["p_overwrite"].toBool();

    QString strError;
    // Plausi 1.
    if(sourceDirPath.isEmpty()) {
        appendErrorMsg(strError, QStringLiteral("RPC_CopyDirFiles: p_sourceDir is empty "));
    }
    if(destDirPath.isEmpty()) {
        appendErrorMsg(strError, QStringLiteral("RPC_CopyDirFiles: p_destDir is empty "));
    }
    // Plausi 2. + remove destination dir optionally
    QDir sourceDir(sourceDirPath);
    if(strError.isEmpty()) {
        if(!sourceDir.exists()) {
            appendErrorMsg(strError, QStringLiteral("RPC_CopyDirFiles: Directory to copy does not exist ") + sourceDirPath);
        }
        if(strError.isEmpty() && cleanDestFirst) {
            QDir destDir(destDirPath);
            if(destDir.exists() && !destDir.removeRecursively()) {
                appendErrorMsg(strError, QStringLiteral("RPC_CopyDirFiles: An error occured deleting destination directory ") + destDirPath);
            }
        }
    }
    // ensure dest directory is there
    if(strError.isEmpty()) {
        QDir destDir;
        if(!destDir.mkpath(destDirPath)) {
            appendErrorMsg(strError, QStringLiteral("RPC_CopyDirFiles: Cannot create directory ") + destDir.path());
        }
    }
    // copy
    if(strError.isEmpty()) {
        // Ensure trailing dir separator
        if(!sourceDirPath.endsWith(QDir::separator())) {
            sourceDirPath.append(QDir::separator());
        }
        if(!destDirPath.endsWith(QDir::separator())) {
            destDirPath.append(QDir::separator());
        }
        for(QString fileName : sourceDir.entryList(nameFilters, QDir::NoDotAndDotDot | QDir::Files)) {
            QString destFileName = destDirPath + fileName;
            bool destExists = QFile::exists(destFileName);
            // skip file in case dest exists and we do not overwrite
            if(destExists && !overwrite) {
                continue;
            }
            // overwrite: delete dest first
            if(destExists) {
                QFile destFile(destFileName);
                if(!destFile.remove()) {
                    appendErrorMsg(strError, QStringLiteral("RPC_CopyDirFiles: Cannot delete file for overwrite ") + destFileName);
                }
            }
            // copy file
            QString sourceFileName = sourceDirPath + fileName;
            if(!QFile::copy(sourceFileName, destFileName)) {
                appendErrorMsg(strError, QStringLiteral("RPC_CopyDirFiles: Cannot copy ") + sourceFileName + QStringLiteral(" to ") + destFileName);
            }
            // Errors are not expected / accepted here. So abort on first
            if(!strError.isEmpty()) {
                break;
            }
        }

    }
    bool ok = strError.isEmpty();
    if(!ok) {
        // drop a note to whoever listens (usually journal)
        qWarning("%s", qPrintable(strError));
    }
    return ok;
}

void vf_files::setFindLimits(const int maxRecursionDepth, const int findMaxHitsDirs, const int findMaxHitsFiles)
{
    m_findMaxRecursionDepth = maxRecursionDepth;
    m_findMaxHitsDirs = findMaxHitsDirs;
    m_findMaxHitsFiles = findMaxHitsFiles;
}

void vf_files::findInPath(const QString &baseDir,
                const QStringList &nameFilterList,
                const int currFilterOrPathDepth,
                const bool returMatchingDirsOnly,
                QStringList &resultList,
                QString &strError)
{
    if(currFilterOrPathDepth >= nameFilterList.length()) {
        appendErrorMsg(strError,
                       QStringLiteral("RPC_FindFileSpecial: findInPaths called odd params! baseDir: %1 / nameFilterList.length() %2 / currFilterOrPathDepth %3").arg(baseDir).arg(nameFilterList.length()).arg(currFilterOrPathDepth));
    }
    QDir searchDir(baseDir);
    if(!searchDir.exists()) {
        appendErrorMsg(strError, QStringLiteral("vf_files::findInPath called with non existent baseDir ") + baseDir);
    }
    if(!strError.isEmpty()) {
        return;
    }

    // first searches are on paths / the last searches for files
    bool bSearchFiles = currFilterOrPathDepth == nameFilterList.count() - 1;
    // nameFilterList is a list of filters. Each filter itself is a list of ';' separated filters
    QStringList currFilterList = nameFilterList[currFilterOrPathDepth].split(QStringLiteral(";"));

    // last recursion depth: on files
    if(bSearchFiles) {
        QStringList fileList = searchDir.entryList(currFilterList, QDir::NoDotAndDotDot | QDir::Files);
        if(!fileList.isEmpty()) {
            if(returMatchingDirsOnly) {
                if(resultList.count() >= m_findMaxHitsDirs) {
                    appendErrorMsg(strError, QStringLiteral("RPC_FindFileSpecial: maximum dirs found. Allowed: %1 / Current: %2").arg(m_findMaxHitsDirs).arg(resultList.count()));
                }
                else {
                    resultList.append(baseDir);
                }
            }
            else {
                if(resultList.count() + fileList.count() >= m_findMaxHitsFiles) {
                    appendErrorMsg(strError, QStringLiteral("RPC_FindFileSpecial: maximum files found. Allowed: %1 / Wanted: %2").arg(m_findMaxHitsFiles).arg(resultList.count()+fileList.count()));
                    fileList = fileList.mid(0, m_findMaxHitsFiles - resultList.count());
                }
                // Return file with full path (otherwise results are pretty useless...)
                QString baseDirExt = baseDir;
                if(!baseDirExt.endsWith(QDir::separator())) {
                    baseDirExt.append(QDir::separator());
                }
                for(const auto &file : fileList) {
                    resultList += baseDirExt + file;
                }
            }
        }
    }
    // dirs on the way
    else {
        QStringList dirList = searchDir.entryList(currFilterList, QDir::NoDotAndDotDot | QDir::Dirs);
        QString subDir = baseDir;
        if(!subDir.endsWith(QDir::separator())) {
            subDir.append(QDir::separator());
        }
        // iterate through all subs recursively (and as long as no error occured)
        for(const auto& subEntry : dirList) {
            if(!strError.isEmpty()) {
                break;
            }
            else {
                findInPath(subDir+subEntry, nameFilterList, currFilterOrPathDepth+1, returMatchingDirsOnly, resultList, strError);
            }
        }
    }
}

QVariant vf_files::RPC_FindFileSpecial(QVariantMap p_params)
{
    QString baseDir = p_params["p_baseDir"].toString();
    QStringList nameFilterList = p_params["p_nameFilterList"].toStringList();
    bool returMatchingDirsOnly = p_params["p_returnMatchingDirsOnly"].toBool();

    // Don't block our machine if bad guys come around
    QString strError;
    if(nameFilterList.count() > m_findMaxRecursionDepth) {
        appendErrorMsg(strError, QStringLiteral("RPC_FindFileSpecial: nameFilterList.count() exceeds max. recursion depth. Allowed: %1 / Set: %2").arg(m_findMaxRecursionDepth).arg(nameFilterList.count()));
    }
    QStringList resultList;
    if(strError.isEmpty()) {
        // Start recursive traversing...
        findInPath(baseDir, nameFilterList, 0, returMatchingDirsOnly, resultList, strError);
    }
    if(!strError.isEmpty()) {
        // drop a note to whoever listens (usually journal)
        qWarning("%s", qPrintable(strError));
    }
    return resultList;
}

QVariant vf_files::RPC_DeleteFile(QVariantMap p_params)
{
    QString strError;

    QString fileName = p_params["p_fullPathFile"].toString();
    QFile file(fileName);
    if(!file.exists()) {
        appendErrorMsg(strError, QStringLiteral("RPC_DeleteFile: File to delete %1 does not exist").arg(fileName));
    }
    else {
        QFileInfo fileInfo(file);
        if(!fileInfo.isFile()) {
            if(fileInfo.isDir()) {
                appendErrorMsg(strError, QStringLiteral("RPC_DeleteFile: Cannot delete directory %1").arg(fileName));
            }
            else {
                appendErrorMsg(strError, QStringLiteral("RPC_DeleteFile: %1 does not seem to be a file").arg(fileName));
            }
        }
    }
    if(strError.isEmpty()) {
        if(!file.remove()) {
            appendErrorMsg(strError, QStringLiteral("RPC_DeleteFile: Could not delete file %1").arg(fileName));
        }
    }
    if(!strError.isEmpty()) {
        // drop a note to whoever listens (usually journal)
        qWarning("%s", qPrintable(strError));
    }
    return strError.isEmpty();
}

QVariant vf_files::RPC_GetDriveInfo(QVariantMap p_params)
{
    QString strError;

    QString mountDir = p_params["p_mountDir"].toString();
    QString localeName = p_params["p_localeName"].toString();
    bool getDriveName = p_params["p_getDriveName"].toBool();
    bool getMemTotal = p_params["p_getMemTotal"].toBool();
    bool getMemFree = p_params["p_getMemFree"].toBool();

    bool mountExists = QFile::exists(mountDir);
    // check some obvious plausis first
    if(!mountExists) {
        appendErrorMsg(strError, QStringLiteral("RPC_GetDriveInfo: ") + mountDir + QStringLiteral(" does not exist"));
    }

    QStringList listMountInfo;
    if(strError.isEmpty()) {
        QStorageInfo storageInfo(mountDir);
        if(storageInfo.isValid()) {
            if(getDriveName) {
                listMountInfo.append(QStringLiteral("name:") + storageInfo.name());
            }
            if(getMemTotal || getMemFree) {
                QLocale locale(localeName);
                qint64 bytes;
                if(getMemTotal) {
                    bytes = storageInfo.bytesTotal();
                    listMountInfo.append(QStringLiteral("total:") + locale.formattedDataSize(bytes, 2, m_dataSizeflags));
                }
                if(getMemFree) {
                    bytes = storageInfo.bytesFree();
                    listMountInfo.append(QStringLiteral("free:") + locale.formattedDataSize(bytes, 2, m_dataSizeflags));
                }
            }
        }
        else {
            appendErrorMsg(strError, QStringLiteral("RPC_GetDriveInfo: Could not create storage info for ") + mountDir);
        }
    }
    if(!strError.isEmpty()) {
        // drop a note to whoever listens (usually journal)
        qWarning("%s", qPrintable(strError));
    }
    return listMountInfo;
}

bool vf_files::addDirToWatch(const QString componentName,
                             const QString dir,
                             const QStringList nameFilters,
                             QDir::Filters filters,
                             bool fullPathForResults)
{
    bool ok = false;
    if(m_isInitalized) { // prerequisites ok?
        // try to add another watcher component
        DirWatcherEntry* dirWatcherEntry = new DirWatcherEntry(this);
        if(dirWatcherEntry->create(m_entity, componentName, dir, nameFilters, filters, fullPathForResults)) {
            ok = true;
        }
        else {
            delete dirWatcherEntry;
        }
    }
    return ok;
}

bool vf_files::addMountToWatch(const QString componentName, const QString mountBasePath, const QString procFileMount)
{
    bool ok = false;
    if(m_isInitalized) { // prerequisites ok?
        // try to add another watcher component
        MountWatcherEntry* mountWatcherEntry = new MountWatcherEntry(this);
        if(mountWatcherEntry->create(m_entity, componentName, procFileMount, mountBasePath)) {
            ok = true;
        }
        else {
            delete mountWatcherEntry;
        }
    }
    return ok;
}

bool vf_files::addDefaultPathComponent(const QString componentName, const QString dir, bool createDir)
{
    bool ok = false;
    if(m_isInitalized) { // prerequisites ok?
        // try to add another default path component
        DefaultPathEntry* defaultPathEntry = new DefaultPathEntry(this);
        if(defaultPathEntry->create(m_entity, componentName, dir, createDir)) {
            ok = true;
        }
        else {
            delete defaultPathEntry;
        }
    }
    return ok;
}

void vf_files::setDataSizeFormat(QLocale::DataSizeFormats dataSizeflags)
{
    m_dataSizeflags = dataSizeflags;
}

VfCpp::veinmoduleentity *vf_files::getVeinEntity() const
{
    return m_entity;
}
