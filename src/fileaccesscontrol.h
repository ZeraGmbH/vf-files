#ifndef FILEACCESSCONTROL_H
#define FILEACCESSCONTROL_H

#include <QDir>
#include <QVector>
#include <memory>

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

typedef std::shared_ptr<FileAccessControl> FileAccessControlPtr;

#endif // FILEACCESSCONTROL_H
