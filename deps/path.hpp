/*
 MIT License

 Copyright (c) 2022 https://github.com/arlogy

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/

#ifndef PATH_H
#define PATH_H

#include <string>

namespace lib
{
/// Path utility class which would not be needed when using boost::filesystem or
/// C++17 std::filesystem for example.
class path
{
public:
    path() = delete;

    /// parent("file.txt") -> file.txt
    /// parent("dir1/dir2/file.txt") -> dir1/dir2
    /// parent("dir1\\dir2\\file.txt") -> dir1\dir2
    static std::string parent(const std::string& path)
    {
        size_t last_separator_pos = path.find_last_of("/\\");
        return path.substr(0, last_separator_pos);
    }
};
}

#endif // PATH_H