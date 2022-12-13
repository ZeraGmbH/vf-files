#include <QFileInfo>
#include "fileaccesscontrol.h"

FileAccessControl::FileAccessControl(QVector<QDir> &allowdList) :
    m_allowedDirs(allowdList)
{

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

void FileAccessControl::addDirToAllowedDirList(QDir dirName)
{
    m_allowedDirs.append(dirName);
}
