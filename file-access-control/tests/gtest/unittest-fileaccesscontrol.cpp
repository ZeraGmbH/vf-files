#include <gtest/gtest.h>
#include <QString>
#include "fileaccesscontrol.h"

static const QString accessAllowedFile = QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/allowed-folder/access-allowed.txt");
static const QString accessNotAllowedFile = QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/denied-folder/access-not-allowed.txt");
static const QString nonExistingFile = QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/allowed-folder/foo.txt");

static const QString accessAllowedFolder =  QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/allowed-folder/");

TEST(FILE_ACCESS, FILE_WITH_ACCESS_ALLOWED)
{
    FileAccessControl testAccess;
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_TRUE(testAccess.isFileAccessAllowed(accessAllowedFile));
}

TEST(FILE_ACCESS, FILE_WITH_ACCESS_NOT_ALLOWED)
{
    FileAccessControl testAccess;
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_FALSE(testAccess.isFileAccessAllowed(accessNotAllowedFile));
}

TEST(FILE_ACCESS, NON_EXISTING_FILE)
{
    FileAccessControl testAccess;
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_FALSE(testAccess.isFileAccessAllowed(nonExistingFile));
}
