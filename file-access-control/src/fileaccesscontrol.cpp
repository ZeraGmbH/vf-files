#include <QFileInfo>
#include "fileaccesscontrol.h"

FileAccessControl::FileAccessControl()
{

}

bool FileAccessControl::isAccessAllowed(QString fileName)
{
    QFileInfo fileInfo(fileName);
    QString absoluteFilePath(fileInfo.absolutePath());

    return m_allowedDirs.contains(absoluteFilePath);
}

void FileAccessControl::addDirToAllowedDirList(QDir dirName)
{
    m_allowedDirs.append(dirName);
}
