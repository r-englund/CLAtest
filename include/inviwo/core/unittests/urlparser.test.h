#include "..\util\filedirectory.h"

#ifndef IVW_URLPARSERTEST_TEST_H
#define IVW_URLPARSERTEST_TEST_H


TEST(URLParserTest,fileExistsTest){
    EXPECT_FALSE(URLParser::fileExists(IVW_DIR));
#ifdef __FILE__
    EXPECT_TRUE(URLParser::fileExists(__FILE__));
#endif
    EXPECT_TRUE(URLParser::fileExists(global_argv[0]),"Cant find current executable");

}


#endif