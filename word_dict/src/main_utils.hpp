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

#ifndef MAIN_UTILS_H
#define MAIN_UTILS_H

#include "word_dict.hpp"

#include "timer.hpp"

#include <iostream>

const std::string &title_prefix = "--- ";
const std::string &title_suffix = " ---";
const std::string &msg_prefix1  = "[-] ";
const std::string &msg_prefix2  = "    ";

std::string title_str(const std::string &str)
{
    return title_prefix + str + title_suffix;
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
        std::cout << msg_prefix2
                  << "unable to add words from file " << filename
                  << std::endl;
        return false;
    }
    std::cout << msg_prefix2
              << "words successfully added from file " << filename
              << std::endl;
    return true;
}

void match_words(
    const word_dict &dict,
    const std::vector<std::string> &words,
    unsigned int cost_max
)
{
    const unsigned int cost_min = 0;

    timer tm;
    for(const std::string &word : words) {
        std::cout << std::endl;

        tm.reset();
        std::cout << msg_prefix1
                  << dict.match_word_exactly(word).full_descr()
                  << " "
                  << tm.elapsed_time_str()
                  << std::endl;

        for(unsigned int i = cost_min; i <= cost_max; i++) {
            tm.reset();
            std::cout << (i == cost_min ? msg_prefix1 : msg_prefix2)
                      << dict.match_word_allow_substitution(word, i).full_descr()
                      << " "
                      << tm.elapsed_time_str()
                      << std::endl;
        }

        for(unsigned int i = cost_min; i <= cost_max; i++) {
            tm.reset();
            std::cout << (i == cost_min ? msg_prefix1 : msg_prefix2)
                      << dict.match_word_levenshtein_distance(word, i).full_descr()
                      << " "
                      << tm.elapsed_time_str()
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

void add_and_match_words_from_resource_file(
    word_dict &dict,
    const std::string &dir_path
)
{
    if(!add_words_from_file(dict, dir_path + "/../resource/words.txt")) {
        return;
    }

    match_words(dict, {
        "s-sq-i--rp-ne",   // knowing that sesquiterpene     is one of the words in the dictionary
        "woolen*sto?k-ed", // knowing that woolen-stockinged is one of the words in the dictionary
        "o.bathering",     // knowing that woolgathering     is one of the words in the dictionary
        "0123456789",
        "abcdefghij",
    }, 9);
}

#endif // MAIN_UTILS_H
