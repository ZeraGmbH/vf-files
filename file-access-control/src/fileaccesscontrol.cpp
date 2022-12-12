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

void FileAccessControl::addDirToAllowedDirList(QDir dirName)
{
    m_allowedDirs.append(dirName);
}
