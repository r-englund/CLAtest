#include <inviwo/core/util/urlparser.h>

#ifndef IVW_URLPARSERTEST_TEST_H
#define IVW_URLPARSERTEST_TEST_H


TEST(URLParserTest,fileExistsTest) {
#ifdef __FILE__
    EXPECT_TRUE(URLParser::fileExists(__FILE__));
#endif
    EXPECT_TRUE(URLParser::fileExists(global_argv[0]));//Cant find current executable
}

TEST(URLParserTest,fileExtensionTest) {
    EXPECT_STREQ("",URLParser::getFileExtension("").c_str());
    EXPECT_STREQ("txt",URLParser::getFileExtension("test.txt").c_str());
    EXPECT_STREQ("txt",URLParser::getFileExtension("test.dobule.txt").c_str());
    EXPECT_STREQ("",URLParser::getFileExtension("noExtensions").c_str());
    EXPECT_STREQ("",URLParser::getFileExtension("C:/a/directory/for/test/noExtensions").c_str());
    EXPECT_STREQ("", URLParser::getFileExtension("C:/a/directory/for/test.test/noExtensions").c_str());
#ifdef __FILE__
    EXPECT_STREQ("h",URLParser::getFileExtension(__FILE__).c_str());
#endif
}



TEST(URLParserTest,FileDirectoryTest) {
    EXPECT_STREQ("C:/a/directory/for/test/",URLParser::getFileDirectory("C:/a/directory/for/test/file.txt").c_str());
    EXPECT_STREQ("C:\\a\\directory\\for\\test\\",URLParser::getFileDirectory("C:\\a\\directory\\for\\test\\file.txt").c_str());
    EXPECT_STREQ("",URLParser::getFileDirectory("justafile.txt").c_str());
    // EXPECT_STREQ("C:/a/directory/for/test/",URLParser::getFileDirectory("C:/a/directory/for/test//withdoubleslahs.txt").c_str());
}

#endif