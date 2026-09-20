#ifndef FILEACCESSCONTROL_H
#define FILEACCESSCONTROL_H

#include <QDir>
#include <QVector>
#include <memory>

class FileAccessControl
{
public:
    FileAccessControl();
    explicit FileAccessControl(const QStringList &allowdList);

    bool isFileAccessAllowed(const QString &fileName) const;
    bool isFolderAccessAllowed(const QString &folderName) const;
    void addDirToAllowedDirList(const QString &dirName);
private:
    QVector<QDir> m_allowedDirs;
};

typedef std::shared_ptr<FileAccessControl> FileAccessControlPtr;

#endif // FILEACCESSCONTROL_H
