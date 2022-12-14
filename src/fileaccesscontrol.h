#ifndef FILEACCESSCONTROL_H
#define FILEACCESSCONTROL_H

#include <QDir>
#include <QVector>

class FileAccessControl
{
public:
    FileAccessControl();
    FileAccessControl(QStringList &allowdList);

    bool isFileAccessAllowed(QString fileName);
    bool isFolderAccessAllowed(QString folderName);
    void addDirToAllowedDirList(QString dirName);
private:
    QVector<QDir> m_allowedDirs;
};

#endif // FILEACCESSCONTROL_H
