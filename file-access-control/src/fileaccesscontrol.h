#ifndef FILEACCESSCONTROL_H
#define FILEACCESSCONTROL_H

#include <QDir>
#include <QVector>

class FileAccessControl
{
public:
    FileAccessControl();

    bool isFileAccessAllowed(QString fileName);
    bool isFolderAccessAllowed(QString folderName);
    void addDirToAllowedDirList(QDir dirName);
private:
    QVector<QDir> m_allowedDirs;
};

#endif // FILEACCESSCONTROL_H
