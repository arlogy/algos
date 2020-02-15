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

#ifndef WORD_DICT_H
#define WORD_DICT_H

#include "dfa_string_dict.h"

/// Dictionary of words (strings): a simple wrapper focusing on key functions of
/// the dfa_string_dict implementation.
class word_dict
{
public:
    explicit word_dict() {}

    bool add_word(const std::string &word) { return m_dict.add_string(word); }
    bool add_words_from_file(const std::string &filename)
    {
        return m_dict.add_strings_from_file(filename);
    }

    void clear() { m_dict.clear(); }

    dfa_string_dict::match_data match_word_exactly(const std::string &word) const
    {
        return m_dict.match_string_exactly(word);
    }
    dfa_string_dict::match_data match_word_allow_substitution(const std::string &word,
                                                              unsigned int subst_max = 0) const
    {
        return m_dict.match_string_allow_substitution(word, subst_max);
    }
    dfa_string_dict::match_data match_word_levenshtein_distance(const std::string &word,
                                                                unsigned int edit_max = 0) const
    {
        return m_dict.match_string_levenshtein_distance(word, edit_max);
    }

    void print_words(std::ostream &stream) const { m_dict.print_strings(stream); }
    void print_tree(std::ostream &stream) const { m_dict.print_tree(stream); }

    static char end_of_word_marker() { return dfa_string_dict::tree_end_of_string_marker; }

private:
    dfa_string_dict m_dict;
};

#endif // WORD_DICT_H
