#include <QFileInfo>
#include "fileaccesscontrol.h"

FileAccessControl::FileAccessControl()
{

}

FileAccessControl::FileAccessControl(QStringList &allowdList)
{
    for(const auto &str: allowdList) {
        m_allowedDirs.append(str);
    }
}

bool FileAccessControl::isFileAccessAllowed(QString fileName)
{
    QFileInfo fileInfo(fileName);

    return isFolderAccessAllowed(fileInfo.absolutePath());
}

bool FileAccessControl::isFolderAccessAllowed(QString folderName)
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

void FileAccessControl::addDirToAllowedDirList(QString dirName)
{
    m_allowedDirs.append(dirName);
}
