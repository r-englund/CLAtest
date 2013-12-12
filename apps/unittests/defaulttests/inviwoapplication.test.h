#ifndef IVW_INVIWOAPPLICATION_TEST_H
#define IVW_INVIWOAPPLICATION_TEST_H


TEST(InviwoApplicationTest,initTest){
    InviwoApplication* app = InviwoApplication::getPtr();
    ASSERT_TRUE(app!=0);
    EXPECT_TRUE(app->isInitialized());

    EXPECT_STREQ(("unittest "+IVW_VERSION).c_str(),
                 app->getDisplayName().c_str());
    
    EXPECT_STREQ(IVW_DIR.c_str(),
                 app->getBasePath().c_str());

}

TEST(InviwoApplicationTest,test2){
    
}

#endif