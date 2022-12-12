#include <QFileInfo>
#include "fileaccesscontrol.h"

FileAccessControl::FileAccessControl()
{

}

bool FileAccessControl::isFileAccessAllowed(QString fileName)
{
    QFileInfo fileInfo(fileName);

    if (fileInfo.exists()) {
        return m_allowedDirs.contains(fileInfo.absolutePath());
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

void FileAccessControl::addDirToAllowedDirList(QDir dirName)
{
    m_allowedDirs.append(dirName);
}
