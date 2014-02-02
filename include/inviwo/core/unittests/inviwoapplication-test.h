
#ifndef IVW_INVIWOAPPLICATION_TEST_H
#define IVW_INVIWOAPPLICATION_TEST_H


TEST(InviwoApplicationTest,initTest) {
    InviwoApplication* app = InviwoApplication::getPtr();
    ASSERT_TRUE(app!=0);
    EXPECT_TRUE(app->isInitialized());
}


TEST(InviwoApplicationTest,displayNameTest) {
    InviwoApplication* app = InviwoApplication::getPtr();
    ASSERT_TRUE(app!=0);
    EXPECT_TRUE(app->getDisplayName().find(IVW_VERSION)!=std::string::npos);
}

#endif