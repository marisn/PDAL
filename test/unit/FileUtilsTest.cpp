/******************************************************************************
* Copyright (c) 2011, Michael P. Gerlek (mpg@flaxen.com)
*
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following
* conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in
*       the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of Hobu, Inc. or Flaxen Geo Consulting nor the
*       names of its contributors may be used to endorse or promote
*       products derived from this software without specific prior
*       written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
* OF SUCH DAMAGE.
****************************************************************************/

#include <pdal/pdal_test_main.hpp>

#include <pdal/util/FileUtils.hpp>
#include <pdal/util/Utils.hpp>

#include "Support.hpp"

#include <iostream>
#ifdef _WIN32
#include <codecvt>
#include <winioctl.h>
#endif

using namespace pdal;

TEST(FileUtilsTest, test_file_ops)
{
    std::string tmp1(Support::temppath("unittest1.tmp"));
    std::string tmp2(Support::temppath("unittest2.tmp"));

    // first, clean up from any previous test run
    FileUtils::deleteFile(tmp1);
    FileUtils::deleteFile(tmp2);
    EXPECT_TRUE(FileUtils::fileExists(tmp1)==false);
    EXPECT_TRUE(FileUtils::fileExists(tmp2)==false);

    // write test
    std::ostream* ostr = FileUtils::createFile(tmp1);
    *ostr << "yow";
    FileUtils::closeFile(ostr);

    EXPECT_EQ(FileUtils::fileExists(tmp1), true);
    EXPECT_EQ(FileUtils::fileSize(tmp1), 3U);

    // rename test
    FileUtils::renameFile(tmp2,tmp1);
    EXPECT_TRUE(FileUtils::fileExists(tmp1)==false);
    EXPECT_TRUE(FileUtils::fileExists(tmp2)==true);

    // read test
    std::istream* istr = FileUtils::openFile(tmp2);
    std::string yow;
    *istr >> yow;
    FileUtils::closeFile(istr);
    EXPECT_TRUE(yow=="yow");

    // delete test
    FileUtils::deleteFile(tmp2);
    EXPECT_TRUE(FileUtils::fileExists(tmp2)==false);

    EXPECT_THROW(FileUtils::openFile("~foo1.glob"), pdal::pdal_error);
    EXPECT_NO_THROW(FileUtils::openFile("foo~1.glob"));
}

TEST(FileUtilsTest, test_vsi)
{
    // VSI write test
    std::string vsiFile("/vsimem/vsi.txt");
    std::ostream* oStrVsi = FileUtils::createFile(vsiFile);
    EXPECT_TRUE(FileUtils::fileExists(vsiFile));
    *oStrVsi << "yow";
    FileUtils::closeFile(oStrVsi);
    EXPECT_EQ(FileUtils::fileExists(vsiFile), true);
    EXPECT_EQ(FileUtils::fileSize(vsiFile), 3U);

    // VSI read test
    std::istream* iStrVsi = FileUtils::openFile(vsiFile);
    std::string yowVsi;
    *iStrVsi >> yowVsi;
    FileUtils::closeFile(iStrVsi);
    EXPECT_TRUE(yowVsi=="yow");
}

TEST(FileUtilsTest, test_readFileIntoString)
{
    const std::string filename = Support::datapath("text/text.txt");
    EXPECT_TRUE(FileUtils::fileExists(filename));

    std::string source = FileUtils::readFileIntoString(filename);

    std::string ref = "This is a file that allows us to test that we "
        "can read a text file into a string through the file utils.\n";

    EXPECT_TRUE(source == ref);
}

#ifdef _WIN32
static const std::string drive = "A:";
#else
static const std::string drive = "";
#endif

static std::string normalize(const std::string p)
{
    return Utils::replaceAll(p, "\\", "/");
}

static void compare_paths(const std::string a, const std::string b)
{
    EXPECT_EQ(normalize(a), normalize(b));
}

TEST(FileUtilsTest, test_toAbsolutePath)
{
    using namespace std;

    const string root = FileUtils::getcwd();

    // check 1-arg version: make absolute when file is relative,
    // via current working dir
    const string a = FileUtils::toAbsolutePath("foo.txt");
    compare_paths(a, root + "foo.txt");

    // check 1-arg version: make absolute when file is already absolute
    const string b = FileUtils::toAbsolutePath(drive + "/baz/foo.txt");
    compare_paths(b, drive + "/baz/foo.txt");

    // check 2-arg version: make absolute when file relative, via given base
    const string c = FileUtils::toAbsolutePath("foo.txt", drive + "/a/b/c/d");
    compare_paths(c, drive + "/a/b/c/d/foo.txt");

    // check 2-arg version: make absolute when file is relative, via given base (which isn't absolute)
    const string d = FileUtils::toAbsolutePath("foo.txt", "x/y/z");
    compare_paths(d, root + "x/y/z/" + "foo.txt");

    // check 1-arg version: make absolute when file is already absolute
    const string e = FileUtils::toAbsolutePath(drive+"/baz/foo.txt", drive+"/a/b/c/d");
    compare_paths(e, drive + "/baz/foo.txt");
}

TEST(FileUtilsTest, test_getDirectory)
{
    // test absolute case
    const std::string a = FileUtils::getDirectory(drive + "/a/b/foo.txt");
    compare_paths(a, drive + "/a/b/");

    // test relative case
    const std::string b = FileUtils::getDirectory("a/b/foo.txt");
    compare_paths(b, "a/b/");
}

TEST(FileUtilsTest, test_isAbsolute)
{
    // test absolute case
    const bool a = FileUtils::isAbsolutePath(drive + "/a/b/foo.txt");
    EXPECT_TRUE(a);

    // test relative case
    const bool b = FileUtils::isAbsolutePath("a/b/foo.txt");
    EXPECT_TRUE(!b);
}

TEST(FileUtilsTest, filename)
{
    std::string filename = "";
    EXPECT_EQ(FileUtils::getFilename(filename), "");

    filename = "/";
    EXPECT_EQ(FileUtils::getFilename(filename), "");

    filename = "/foo/bar/";
    EXPECT_EQ(FileUtils::getFilename(filename), "");

    filename = "/foo//bar//baz.c";
    EXPECT_EQ(FileUtils::getFilename(filename), "baz.c");

#ifdef _WIN32
    filename = "C:/foo/bar/baz.c";
    EXPECT_EQ(FileUtils::getFilename(filename), "baz.c");

    filename = "C:\\foo\\bar\\baz.c";
    EXPECT_EQ(FileUtils::getFilename(filename), "baz.c");

    filename = "C:\\foo/bar\\meaw/baz.c";
    EXPECT_EQ(FileUtils::getFilename(filename), "baz.c");
#else
    filename = "C:\\foo\\bar\\baz.c";
    EXPECT_EQ(FileUtils::getFilename(filename), filename);
#endif
}

TEST(FileUtilsTest, extension)
{
    EXPECT_EQ(FileUtils::extension("/foo//bar//baz.c"), ".c");
    EXPECT_EQ(FileUtils::extension("foobar"), "");
    EXPECT_EQ(FileUtils::extension("/foo/bar"), "");
    EXPECT_EQ(FileUtils::extension("/fo.o/b.ar.baz23"), ".baz23");
}

TEST(FileUtilsTest, stem)
{
    EXPECT_EQ(FileUtils::stem("/foo//bar//baz.c"), "baz");
    EXPECT_EQ(FileUtils::stem("foobar"), "foobar");
    EXPECT_EQ(FileUtils::stem("/foo/bar"), "bar");
    EXPECT_EQ(FileUtils::stem("/fo.o/b.ar.baz23"), "b.ar");
    EXPECT_EQ(FileUtils::stem("."), ".");
    EXPECT_EQ(FileUtils::stem(".."), "..");
}

TEST(FileUtilsTest, glob)
{
    auto TP = [](const std::string s)
    {
        return Support::temppath(s);
    };

    auto filenames = [&TP]()
    {
        std::vector<std::string> names;
        std::string name;
        for (int i = 0; i < 5; ++i)
        {
            std::string name;
            name = TP(std::string("foo") + std::to_string(i) + ".glob");
            names.push_back(name);
            name = TP(std::string("bar") + std::to_string(i) + ".glob");
            names.push_back(name);
        }
        return names;
    };

    for (std::string& file : filenames())
        FileUtils::deleteFile(file);

    for (std::string& file : filenames()) {
        auto f = FileUtils::createFile(file);
        FileUtils::closeFile(f);
    }

    EXPECT_EQ(FileUtils::glob(TP("*.glob")).size(), 10u);
    EXPECT_EQ(FileUtils::glob(TP("foo1.glob")).size(), 1u);

    for (std::string& file : filenames())
        FileUtils::deleteFile(file);

    EXPECT_EQ(FileUtils::glob(TP("*.glob")).size(), 0u);
    EXPECT_EQ(FileUtils::glob(TP("foo1.glob")).size(), 0u);

#ifdef _WIN32
    EXPECT_THROW(FileUtils::glob("~foo1.glob"), pdal::pdal_error);
    EXPECT_NO_THROW(FileUtils::glob(TP("foo1~.glob")));
#endif

    std::string temp_filename = Support::temppath("temp.glob");
    FileUtils::deleteFile(temp_filename);
    FileUtils::closeFile(FileUtils::createFile(temp_filename));
    EXPECT_EQ(FileUtils::glob(temp_filename).size(), 1u);
    FileUtils::deleteFile(temp_filename);
}

TEST(FileUtilsTest, test_file_ops_with_unicode_paths)
{
    // 1. Read Unicode encoded word, ie. Japanese, from .txt file.
    // 2. Create temporary directory named using the word. /word/word
    // 3. Create a file in the directory. /word/word/word.unicode
    // 4. Exercise the FileUtils using the Unicode-based path.

    for (std::string japanese_txt: {"japanese-pr2135.txt", "japanese-pr2227.txt"})
    {
        japanese_txt = Support::datapath("unicode/" + japanese_txt);
        EXPECT_TRUE(FileUtils::fileExists(japanese_txt));
        auto const japanese = FileUtils::readFileIntoString(japanese_txt);
        EXPECT_FALSE(japanese.empty());

        auto const japanese_root_dir = Support::temppath(japanese);     
        std::string tmp1(japanese_root_dir + "/"  + japanese + "/" + japanese + ".unicode");


        std::string japanese_dir = FileUtils::getDirectory(tmp1) ;
        EXPECT_TRUE(FileUtils::createDirectories(japanese_dir));

        // test directoryList
        auto const dirs = FileUtils::directoryList(japanese_root_dir);
        EXPECT_GE(dirs.size(), 1U);
        auto const dircount = std::count_if(dirs.cbegin(), dirs.cend(),
            [&japanese_dir](std::string const& d) {
                 return normalize(d + "/" ) == normalize(japanese_dir); });
        EXPECT_EQ(dircount, 1);

        std::string tmp2(Support::temppath("nonunicode.tmp"));
        

        // first, clean up from any previous test run
        FileUtils::deleteFile(tmp1);
        FileUtils::deleteFile(tmp2);
        EXPECT_FALSE(FileUtils::fileExists(tmp1));
        EXPECT_FALSE(FileUtils::fileExists(tmp2));

        // write test
        std::ostream *ostr = FileUtils::createFile(tmp1);
        EXPECT_TRUE(ostr != nullptr);
        *ostr << "yow";
        FileUtils::closeFile(ostr);

        EXPECT_EQ(FileUtils::fileExists(tmp1), true);
        EXPECT_EQ(FileUtils::fileSize(tmp1), 3U);

        // glob for files with Unicode path
        auto const filenames = FileUtils::glob(japanese_dir + "*");
        EXPECT_GE(filenames.size(), 1U);
        auto const tmp1count = std::count_if(filenames.cbegin(), filenames.cend(),
            [&tmp1](std::string const& f) { return normalize(f) == normalize(tmp1); });
        EXPECT_EQ(tmp1count, 1);

        // rename test
        FileUtils::renameFile(tmp2, tmp1);
        EXPECT_FALSE(FileUtils::fileExists(tmp1));
        EXPECT_TRUE(FileUtils::fileExists(tmp2));

        // read test
        std::istream *istr = FileUtils::openFile(tmp2);
        std::string yow;
        *istr >> yow;
        FileUtils::closeFile(istr);
        EXPECT_TRUE(yow == "yow");

        // delete test
        FileUtils::deleteFile(tmp2);
        EXPECT_FALSE(FileUtils::fileExists(tmp2));
        FileUtils::deleteDirectory(japanese_root_dir);
        EXPECT_FALSE(FileUtils::directoryExists(japanese_root_dir));
    }
}

// Don't run if we are WIN32
#if !defined(_WIN32) || defined(_WIN64)
TEST(FileUtilsTest, map)
{
    std::ostream *out;
    // This turns on sparse file support. Otherwise, we're going to make a huge
    // file that won't fit on many filesystems and an error will occur. If we
    // can't set the file to sparse, we just return.  UNIX filesystems I'm
    // aware of support sparse files without this mess.
#ifdef _WIN32
    Support::Tempfile temp(false);
    std::string filename = temp.filename();

    auto toNative = [](const std::string& in) -> std::wstring
    {
        // TODO: C++11 define convert with static thread_local
	std::wstring_convert<std::codecvt_utf8_utf16<uint16_t>, uint16_t> convert;
	auto s = convert.from_bytes(in);
	auto p = reinterpret_cast<wchar_t const*>(s.data());
	return std::wstring(p, p + s.size());
    };

    auto f = CreateFileW(toNative(filename).data(), GENERIC_READ | GENERIC_WRITE,
        0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD flags;
    GetVolumeInformationByHandleW(f, NULL, 0, NULL, NULL, &flags, NULL, 0);
    bool ok = false;
    if (flags & FILE_SUPPORTS_SPARSE_FILES)
    {
        DWORD tmp;
        ok = DeviceIoControl(f, FSCTL_SET_SPARSE, NULL, 0, NULL, 0, &tmp, NULL);
    }
    CloseHandle(f);
    if (!ok)
        return;
    out = FileUtils::openExisting(filename);
#else
    Support::Tempfile temp(true);
    std::string filename = temp.filename();

    out = FileUtils::createFile(filename);
#endif

    out->seekp(50000);
    *out << 1234;
    out->write("Test", 4);
    out->seekp(0x10FFFFFFFF);
    *out << 5678;
    out->write("Another.", 9);
    FileUtils::closeFile(out);

    auto ctx = FileUtils::mapFile(filename);
    assert(ctx.addr());
    char *c = reinterpret_cast<char *>(ctx.addr()) + 50000;

    EXPECT_EQ(*c++, '1');
    EXPECT_EQ(*c++, '2');
    EXPECT_EQ(*c++, '3');
    EXPECT_EQ(*c++, '4');
    EXPECT_EQ(*c++, 'T');
    EXPECT_EQ(*c++, 'e');
    EXPECT_EQ(*c++, 's');
    EXPECT_EQ(*c++, 't');

    c = reinterpret_cast<char *>(ctx.addr()) + 0x10FFFFFFFF;
    EXPECT_EQ(*c++, '5');
    EXPECT_EQ(*c++, '6');
    EXPECT_EQ(*c++, '7');
    EXPECT_EQ(*c++, '8');
    EXPECT_EQ(std::string(c), "Another.");
    FileUtils::unmapFile(ctx);
}
#endif // guard for 32-bit windows
