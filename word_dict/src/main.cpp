/*
 This file is part of https://github.com/arlogy/algos.

 MIT License

 Copyright (c) 2020 https://github.com/arlogy

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

#include "main_utils.hpp"

#include "path.hpp"

int main()
{
    word_dict dict;

    std::cout << title_str("Add sample words") << std::endl;
    add_sample_words(dict);
    std::cout << std::endl;

    std::cout << title_str("Print words as values") << std::endl;
    dict.print_words(std::cout);
    std::cout << std::endl;

    std::cout << title_str("Print words as tree") << std::endl;
    dict.print_tree(std::cout);
    std::cout << std::endl;

    std::cout << title_str("Match sample words") << std::endl;
    match_sample_words(dict);
    std::cout << std::endl;

    std::cout << title_str("Add & Match words from a large file") << std::endl;
    dict.clear();
    add_and_match_words_from_resource_file(dict, path::parent(__FILE__));
    std::cout << std::endl;

    std::cout << title_str("Finished running algorithms on sample data") << std::endl;
    std::cout << msg_prefix2
              << "now examine the output from the beginning to get an overview "
                 "of the word-matching algorithms"
              << std::endl;
    std::cout << std::endl;

    return 0;
}
