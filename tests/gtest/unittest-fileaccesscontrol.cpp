#include <gtest/gtest.h>
#include <QString>
#include "fileaccesscontrol.h"

static const QString accessAllowedFile = QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/allowed-folder/access-allowed.txt");
static const QString accessNotAllowedFile = QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/denied-folder/access-not-allowed.txt");
static const QString nonExistingFile = QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/allowed-folder/foo.txt");
static const QString accessAllowedSubfolderFile = QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/allowed-folder/subfolder/subfolder-access-allowed.txt");
static const QString accessNotAllowedSubfolderFile = QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/denied-folder/subfolder/subfolder-access-not-allowed.txt");
static const QString nonExistingFileAllowedSubfolder = QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/allowed-folder/subfolder/foo.txt");
static const QString nonExistingFileNotAllowedSubfolder = QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/denied-folder/subfolder/foo.txt");

static const QString accessAllowedFolder =  QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/allowed-folder/");
static const QString accessAllowedSubFolder = QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/allowed-folder/subfolder");
static const QString accessNotAllowedSubFolder = QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/denied-folder/subfolder");
static const QString nonExistingFolder =  QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/foo/");
static const QString nastyTestFolderWithNoAccess =  QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/allowed-folder/../denied-folder");
static const QString nastyTestFolderWithAccess =  QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/denied-folder/../allowed-folder");

TEST(FILE_ACCESS, FILE_FROM_ACCESS_ALLOWED_FOLDER)
{
    QVector<QDir> lokalDirVector{accessAllowedFolder};
    FileAccessControl testAccess(lokalDirVector);
    EXPECT_TRUE(testAccess.isFileAccessAllowed(accessAllowedFile));
}

TEST(FILE_ACCESS, FILE_FROM_ACCESS_NOT_ALLOWED_FOLDER)
{
    QVector<QDir> lokalDirVector{accessAllowedFolder};
    FileAccessControl testAccess(lokalDirVector);
    EXPECT_FALSE(testAccess.isFileAccessAllowed(accessNotAllowedFile));
}

TEST(FILE_ACCESS, NON_EXISTING_FILE)
{
    QVector<QDir> lokalDirVector{accessAllowedFolder};
    FileAccessControl testAccess(lokalDirVector);
    EXPECT_FALSE(testAccess.isFileAccessAllowed(nonExistingFile));
}

TEST(FILE_ACCESS, EMPTY_ACCESS_ALLOWED_LIST)
{
    QVector<QDir> lokalDirVector;
    FileAccessControl testAccess(lokalDirVector);
    EXPECT_FALSE(testAccess.isFileAccessAllowed(accessAllowedFile));
}

TEST (FILE_ACCESS, FILE_FROM_ACCESS_ALLOWED_SUBFOLDER)
{
    QVector<QDir> lokalDirVector{accessAllowedFolder};
    FileAccessControl testAccess(lokalDirVector);
    EXPECT_TRUE(testAccess.isFileAccessAllowed(accessAllowedSubfolderFile));
}

TEST (FILE_ACCESS, FILE_FROM_ACCESS_NOT_ALLOWED_SUBFOLDER)
{
    QVector<QDir> lokalDirVector{accessAllowedFolder};
    FileAccessControl testAccess(lokalDirVector);
    EXPECT_FALSE(testAccess.isFileAccessAllowed(accessNotAllowedSubfolderFile));
}

TEST (FILE_ACCESS, NON_EXISTING_FILE_FROM_ACCESS_ALLOWED_SUBFOLDER)
{
    QVector<QDir> lokalDirVector;
    FileAccessControl testAccess(lokalDirVector);
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_FALSE(testAccess.isFileAccessAllowed(nonExistingFileAllowedSubfolder));
}

TEST (FILE_ACCESS, NON_EXISTING_FILE_FROM_ACCESS_NOT_ALLOWED_SUBFOLDER)
{
    QVector<QDir> lokalDirVector;
    FileAccessControl testAccess(lokalDirVector);
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_FALSE(testAccess.isFileAccessAllowed(nonExistingFileNotAllowedSubfolder));
}

TEST (FOLDER_ACCESS, ACCESS_ALLOWED_SUBFOLDER )
{
    QVector<QDir> lokalDirVector;
    FileAccessControl testAccess(lokalDirVector);
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_TRUE(testAccess.isFolderAccessAllowed(accessAllowedSubFolder));
}

TEST (FOLDER_ACCESS, ACCESS_NOT_ALLOWED_SUBFOLDER )
{
    QVector<QDir> lokalDirVector;
    FileAccessControl testAccess(lokalDirVector);

    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_FALSE(testAccess.isFolderAccessAllowed(accessNotAllowedSubFolder));
}

TEST(FOLDER_ACCESS, NON_EXISTING_FOLDER)
{
    QVector<QDir> lokalDirVector;
    FileAccessControl testAccess(lokalDirVector);
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_FALSE(testAccess.isFolderAccessAllowed(nonExistingFolder));
}

TEST(FOLDER_ACCESS, EMPTY_ACCESS_ALLOWED_LIST)
{
    QVector<QDir> lokalDirVector;
    FileAccessControl testAccess(lokalDirVector);
    EXPECT_FALSE(testAccess.isFolderAccessAllowed(accessAllowedFolder));
}

TEST (FOLDER_ACCESS, NASTY_TEST_FOLDER_WITH_ACCESS )
{
    QVector<QDir> lokalDirVector;
    FileAccessControl testAccess(lokalDirVector);
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_TRUE(testAccess.isFolderAccessAllowed(nastyTestFolderWithAccess));
}

TEST (FOLDER_ACCESS, NASTY_TEST_FOLDER_WITH_NO_ACCESS )
{
    QVector<QDir> lokalDirVector;
    FileAccessControl testAccess(lokalDirVector);
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_FALSE(testAccess.isFolderAccessAllowed(nastyTestFolderWithNoAccess));
}