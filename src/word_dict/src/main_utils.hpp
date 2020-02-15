/*
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

#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H

#include "word_dict.hpp"

#include "timer.hpp"

#include <iostream>

const std::string &head_prefix     = "--- ";
const std::string &head_suffix     = " ---";
const std::string &content_prefix1 = "[-] ";
const std::string &content_prefix2 = "    ";

std::string head_title(const std::string &str)
{
    return head_prefix + str + head_suffix;
}

void add_words(word_dict &dict, const std::vector<std::string> &words)
{
    for(const std::string &word : words) {
        std::cout << "adding \"" << word << "\" "
                  << (dict.add_word(word) ? "succeeded" : "failed")
                  << std::endl;
    }
}

bool add_words_from_file(word_dict &dict, const std::string &filename)
{
    if(!dict.add_words_from_file(filename)) {
        std::cout << content_prefix2
                  << "unable to add words from file " << filename
                  << std::endl;
        return false;
    }
    std::cout << content_prefix2
              << "words successfully added from file " << filename
              << std::endl;
    return true;
}

void match_words(const word_dict &dict, const std::vector<std::string> &words, unsigned int max)
{
    const unsigned int min = 0;

    lib::timer timer;
    for(const std::string &word : words) {
        std::cout << std::endl;

        timer.reset();
        std::cout << content_prefix1
                  << dict.match_word_exactly(word).full_str() << " " << timer.elapsed_time_str()
                  << std::endl;

        for(unsigned int i = min; i <= max; i++) {
            timer.reset();
            std::cout << (i == min ? content_prefix1 : content_prefix2)
                      << dict.match_word_allow_substitution(word, i).full_str() << " " << timer.elapsed_time_str()
                      << std::endl;
        }

        for(unsigned int i = min; i <= max; i++) {
            timer.reset();
            std::cout << (i == min ? content_prefix1 : content_prefix2)
                      << dict.match_word_levenshtein_distance(word, i).full_str() << " " << timer.elapsed_time_str()
                      << std::endl;
        }
    }
}

void add_sample_words(word_dict &dict)
{
    add_words(dict, {
        "",
        "a",
        "b",
        std::string(1, word_dict::end_of_word_marker()), // KO
        "aba",
        "abb",
        "aaaa",
        "aaaa", // OK but won't be duplicated in tree
    });
}

void match_sample_words(const word_dict &dict)
{
    match_words(dict, {
        "",
        "$",
        "abc",
        "aaaa",
        "zzzz",
    }, 4);
}

void add_sample_words_from_file_and_try_matching(word_dict &dict, const std::string &dir_path)
{
    if(!add_words_from_file(dict, dir_path + "/../../../rsrc/words.txt")) {
        return;
    }

    match_words(dict, {
        "woolen*sto?k-ed", // knowing that woolen-stockinged is in the dictionary for example
        "o.bathering", // knowing that woolgathering is in the dictionary for example
        "0123456789",
        "abcdefghij",
    }, 9);
}

#endif // MAIN_UTILS_H
