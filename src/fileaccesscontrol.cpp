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

    if (fileInfo.exists()) {
        return isFolderAccessAllowed(fileInfo.absolutePath());
    }
    else {
        return false;
    }
}

bool FileAccessControl::isFolderAccessAllowed(QString folderName)
{
    QDir dir(folderName);
    bool accessAllowed = false;

    if (dir.exists()) {
        accessAllowed = m_allowedDirs.contains(dir);
        while (!accessAllowed && dir.cdUp()) {
            accessAllowed = m_allowedDirs.contains(dir);
        }
    }

    return accessAllowed;
}

void FileAccessControl::addDirToAllowedDirList(QString dirName)
{
    m_allowedDirs.append(dirName);
}
