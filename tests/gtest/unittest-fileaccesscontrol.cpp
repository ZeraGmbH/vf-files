#include <gtest/gtest.h>
#include <QString>
#include "fileaccesscontrol.h"

static const char* accessAllowedFile = TEST_DATA_PATH "/allowed-folder/access-allowed.txt";
static const char* accessNotAllowedFile = TEST_DATA_PATH "/denied-folder/access-not-allowed.txt";
static const char* nonExistingFile = TEST_DATA_PATH "/allowed-folder/foo.txt";
static const char* accessAllowedSubfolderFile = TEST_DATA_PATH "/allowed-folder/subfolder/subfolder-access-allowed.txt";
static const char* accessNotAllowedSubfolderFile = TEST_DATA_PATH "/denied-folder/subfolder/subfolder-access-not-allowed.txt";
static const char* nonExistingFileAllowedSubfolder = TEST_DATA_PATH "/allowed-folder/subfolder/foo.txt";
static const char* nonExistingFileNotAllowedSubfolder = TEST_DATA_PATH "/denied-folder/subfolder/foo.txt";

static const char* accessAllowedFolder =  TEST_DATA_PATH "/allowed-folder/";
static const char* accessAllowedSubFolder = TEST_DATA_PATH "/allowed-folder/subfolder";
static const char* accessNotAllowedSubFolder = TEST_DATA_PATH "/denied-folder/subfolder";
static const char* nonExistingFolder =  TEST_DATA_PATH "/foo/";
static const char* nastyTestFolderWithNoAccess =  TEST_DATA_PATH "/allowed-folder/../denied-folder";
static const char* nastyTestFolderWithAccess =  TEST_DATA_PATH "/denied-folder/../allowed-folder";

TEST(FILE_ACCESS, FILE_FROM_ACCESS_ALLOWED_FOLDER)
{
    QStringList allowedFolders{accessAllowedFolder};
    FileAccessControl testAccess(allowedFolders);
    EXPECT_TRUE(testAccess.isFileAccessAllowed(accessAllowedFile));
}

TEST(FILE_ACCESS, FILE_FROM_ACCESS_NOT_ALLOWED_FOLDER)
{
    QStringList allowedFolders{accessAllowedFolder};
    FileAccessControl testAccess(allowedFolders);
    EXPECT_FALSE(testAccess.isFileAccessAllowed(accessNotAllowedFile));
}

TEST(FILE_ACCESS, NON_EXISTING_FILE)
{
    QStringList allowedFolders{accessAllowedFolder};
    FileAccessControl testAccess(allowedFolders);
    EXPECT_FALSE(testAccess.isFileAccessAllowed(nonExistingFile));
}

TEST(FILE_ACCESS, EMPTY_ACCESS_ALLOWED_LIST)
{
    QStringList allowedFolders;
    FileAccessControl testAccess(allowedFolders);
    EXPECT_FALSE(testAccess.isFileAccessAllowed(accessAllowedFile));
}

TEST (FILE_ACCESS, FILE_FROM_ACCESS_ALLOWED_SUBFOLDER)
{
    QStringList allowedFolders{accessAllowedFolder};
    FileAccessControl testAccess(allowedFolders);
    EXPECT_TRUE(testAccess.isFileAccessAllowed(accessAllowedSubfolderFile));
}

TEST (FILE_ACCESS, FILE_FROM_ACCESS_NOT_ALLOWED_SUBFOLDER)
{
    QStringList allowedFolders{accessAllowedFolder};
    FileAccessControl testAccess(allowedFolders);
    EXPECT_FALSE(testAccess.isFileAccessAllowed(accessNotAllowedSubfolderFile));
}

TEST (FILE_ACCESS, NON_EXISTING_FILE_FROM_ACCESS_ALLOWED_SUBFOLDER)
{
    QStringList allowedFolders;
    FileAccessControl testAccess(allowedFolders);
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_FALSE(testAccess.isFileAccessAllowed(nonExistingFileAllowedSubfolder));
}

TEST (FILE_ACCESS, NON_EXISTING_FILE_FROM_ACCESS_NOT_ALLOWED_SUBFOLDER)
{
    QStringList allowedFolders;
    FileAccessControl testAccess(allowedFolders);
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_FALSE(testAccess.isFileAccessAllowed(nonExistingFileNotAllowedSubfolder));
}

TEST (FOLDER_ACCESS, ACCESS_ALLOWED_SUBFOLDER )
{
    QStringList allowedFolders;
    FileAccessControl testAccess(allowedFolders);
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_TRUE(testAccess.isFolderAccessAllowed(accessAllowedSubFolder));
}

TEST (FOLDER_ACCESS, ACCESS_NOT_ALLOWED_SUBFOLDER )
{
    QStringList allowedFolders;
    FileAccessControl testAccess(allowedFolders);
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_FALSE(testAccess.isFolderAccessAllowed(accessNotAllowedSubFolder));
}

TEST(FOLDER_ACCESS, NON_EXISTING_FOLDER)
{
    QStringList allowedFolders;
    FileAccessControl testAccess(allowedFolders);
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_FALSE(testAccess.isFolderAccessAllowed(nonExistingFolder));
}

TEST(FOLDER_ACCESS, EMPTY_ACCESS_ALLOWED_LIST)
{
    QStringList allowedFolders;
    FileAccessControl testAccess(allowedFolders);
    EXPECT_FALSE(testAccess.isFolderAccessAllowed(accessAllowedFolder));
}

TEST (FOLDER_ACCESS, NASTY_TEST_FOLDER_WITH_ACCESS )
{
    QStringList allowedFolders;
    FileAccessControl testAccess(allowedFolders);
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_TRUE(testAccess.isFolderAccessAllowed(nastyTestFolderWithAccess));
}

TEST (FOLDER_ACCESS, NASTY_TEST_FOLDER_WITH_NO_ACCESS )
{
    QStringList allowedFolders;
    FileAccessControl testAccess(allowedFolders);
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_FALSE(testAccess.isFolderAccessAllowed(nastyTestFolderWithNoAccess));
}
