#include <QFileInfo>
#include "fileaccesscontrol.h"

FileAccessControl::FileAccessControl()
{

}

FileAccessControl::FileAccessControl(const QStringList &allowdList)
{
    for(const auto &str: allowdList) {
        m_allowedDirs.append(str);
    }
}

bool FileAccessControl::isFileAccessAllowed(const QString &fileName) const
{
    QFileInfo fileInfo(fileName);
    return isFolderAccessAllowed(fileInfo.absolutePath());
}

bool FileAccessControl::isFolderAccessAllowed(const QString &folderName) const
{
    bool accessAllowed = false;
    QString folderPath = folderName;

    //deny access to all folder paths which contain '..'
    if (!folderPath.contains("..")) {
        while (!accessAllowed && !folderPath.isEmpty()) {
            accessAllowed = m_allowedDirs.contains(folderPath);
            folderPath = folderPath.left(folderPath.lastIndexOf(QDir::separator()));
        }
    }

    return accessAllowed;
}

void FileAccessControl::addDirToAllowedDirList(const QString &dirName)
{
    m_allowedDirs.append(dirName);
}
