#include <gtest/gtest.h>
#include <QString>
#include "fileaccesscontrol.h"

static const QString accessAllowedFile = QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/allowed-folder/access-allowed.txt");

static const QString accessAllowedFolder =  QStringLiteral(TEST_DATA_PATH) + QStringLiteral("/allowed-folder/");

TEST(FILE_ACCESS, FILE_WITH_ACCESS_ALLOWED)
{
    FileAccessControl testAccess;
    testAccess.addDirToAllowedDirList(accessAllowedFolder);
    EXPECT_TRUE(testAccess.isAccessAllowed(accessAllowedFile));
}
