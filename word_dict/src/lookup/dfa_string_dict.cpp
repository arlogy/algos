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

#include "dfa_string_dict.h"

#include "dfa_tree_utils.hpp"

#include <algorithm>
#include <fstream>
#include <stack>

// a special character which cannot be added as a string to the dictionary
const char dfa_string_dict::tree_end_of_string_marker {'$'};

dfa_string_dict::dfa_string_dict()
{
}

bool dfa_string_dict::add_string(const std::string &str)
{
    if(str.find(dfa_string_dict::tree_end_of_string_marker) != std::string::npos) {
        return false; // string must not contain tree_end_of_string_marker
    }

    dfa_tree<char>::node_t *node = &m_tree.root();
    for(const char c : str + dfa_string_dict::tree_end_of_string_marker) {
        node = &node->set_child(c);
    }
    return true;
}

bool dfa_string_dict::add_strings_from_file(const std::string &filename)
{
    std::ifstream file(filename);
    if(!file.is_open()) {
        return false;
    }

    std::string line;
    while(getline(file, line)) {
        add_string(line);
    }

    file.close();
    return true;
}

void dfa_string_dict::clear()
{
    m_tree.clear();
}

dfa_string_dict::match_result dfa_string_dict::match_string_exactly(
    const std::string &str
) const
{
    // Logic: we keep reading characters from the character tree until success
    //        (all characters in the given string have been read, including the
    //        tree_end_of_string_marker) or failure (at least one character
    //        cannot be read).

    typedef unsigned int uint;

    const std::string &s = str + dfa_string_dict::tree_end_of_string_marker;
    uint s_nb_chars_read = 0;
    const uint s_len = s.length();

    auto *node = &m_tree.root();
    do {
        node = node->child_ptr(s.at(s_nb_chars_read));
        if(node) {
            s_nb_chars_read++;
        }
    }
    while(node && s_nb_chars_read < s_len);

    dfa_string_dict::match_result match;
    match.setData(
        "exact-match",
        str,
        s_nb_chars_read == s_len,
        [&]() { return "\"" + s + "\" matched successfully"; },
        [&]() { return "\"" + s + "\" failed to match at '"
                     + s.at(s_nb_chars_read) + "' after reading \""
                     + s.substr(0, s_nb_chars_read) + "\" successfully"; }
    );
    return match;
}

dfa_string_dict::match_result dfa_string_dict::match_string_allow_substitution(
    const std::string &str,
    unsigned int subst_max
) const
{
    // Logic: we compute the number of substitutions required to reach each node
    //        in the character tree, yielding success when a string matching the
    //        given substitution criteria is found, or failure when no such
    //        string exists. Computation starts at the root node and is
    //        initialized with a substitution count of 0.

    typedef unsigned int uint;

    const std::string &s = str + dfa_string_dict::tree_end_of_string_marker;
    const uint s_len = s.length();
    bool s_matched {false};
    std::string s_matched_string;
    uint s_matched_string_cost {0};

    // Set the first tree node to visit.
    std::stack<
            std::tuple<const dfa_tree<char>::node_t*, std::string, uint, uint>
            > unvisited_nodes;
    unvisited_nodes.push(
        std::make_tuple(&m_tree.root(), "", 0, 0)
    );

    // Start visiting.
    while(!s_matched && !unvisited_nodes.empty()) {
        // Select one tree node to visit.
        const dfa_tree<char>::node_t* prev_node;
        std::string prev_read_string;
        uint prev_nb_chars_read;
        uint prev_subst_cost;
        std::tie(prev_node,
                 prev_read_string,
                 prev_nb_chars_read,
                 prev_subst_cost) = unvisited_nodes.top();
        unvisited_nodes.pop();

        const char expected_char = s.at(prev_nb_chars_read);

        // Visit the selected tree node.
        for(auto it = prev_node->begin(); it != prev_node->end(); it++) {
            const std::string &curr_read_string = prev_read_string + it->first;
            const uint curr_nb_chars_read = prev_nb_chars_read + 1;

            // Decide whether a substitution is required.
            if(expected_char == it->first) {
                if(curr_nb_chars_read == s_len) {
                    if(prev_subst_cost <= subst_max) {
                        s_matched = true;
                        s_matched_string = curr_read_string;
                        s_matched_string_cost = prev_subst_cost;
                        break;
                    }
                }
                else {
                    if(curr_nb_chars_read < s_len) {
                        unvisited_nodes.push(
                            std::make_tuple(
                                &it->second,
                                curr_read_string,
                                curr_nb_chars_read,
                                prev_subst_cost // 0 substitution needed
                            )
                        );
                    }
                }
            }
            else {
                if(curr_nb_chars_read < s_len) {
                    unvisited_nodes.push(
                        std::make_tuple(
                            &it->second,
                            curr_read_string,
                            curr_nb_chars_read,
                            prev_subst_cost + 1 // 1 substitution needed
                        )
                    );
                }
            }
        }
    }

    dfa_string_dict::match_result match;
    match.setData(
        "subst-match(" + std::to_string(subst_max) + ")",
        str,
        s_matched,
        [&]() { return "\"" + s + "\" matched successfully with \""
                      + s_matched_string + "\" using "
                      + std::to_string(s_matched_string_cost) + " substs"; },
        [&]() { return "\"" + s + "\" failed to match"; }
    );
    return match;
}

dfa_string_dict::match_result dfa_string_dict::match_string_levenshtein_distance(
    const std::string &str,
    unsigned int edit_max
) const
{
    // Logic: we compute the Levenshtein distance from all strings in the
    //        character tree to the given string, yielding success when we reach
    //        a string with an edit cost lower or equal to the given limit, or
    //        failure when no such string exists.
    //
    // For example, the matrix used to compute the Levenshtein distance from
    // "tea" to "meat", denoted Lev("tea", "meat"), is filled as shown below.
    // Moreover, apart from the first figure, which corresponds to the
    // initialization step, each cell is computed as follows:
    //     Cell[i][j] = min(
    //         Cell[i-1][j] + 1,
    //         Cell[i][j-1] + 1,
    //         Cell[i-1][j-1] + k (where k is either 0 or 1, depending whether
    //                             letters at i and j are identical)
    //     )
    // Additional details about the Levenshtein distance can be found online.
    //
    //     m e a t         m e a t         m e a t         m e a t
    //  |0|1|2|3|4|     |0|1|2|3|4|     |0|1|2|3|4|     |0|1|2|3|4|
    // t|1| | | | |    t|1|1|2|3|3|    t|1|1|2|3|3|    t|1|1|2|3|3|
    // e|2| | | | |    e|2| | | | |    e|2|2|1|2|3|    e|2|2|1|2|3|
    // a|3| | | | |    a|3| | | | |    a|3| | | | |    a|3|3|2|1|2|
    //
    // So Lev("tea", "meat") = bottom-right value in matrix = 2.
    //
    // Notice that as we fill the matrix vertically (initialization excluded),
    // only the last row changes and its values depend on those in the previous
    // row. In clearer terms, computing Lev("tea", "meat") depends on
    // Lev("te", "meat"), which in turn depends on Lev("t", "meat"), which in
    // turn depends on Lev("", "meat").
    //
    // This is how we will compute the Levenshtein distance from any string in
    // the character tree to the given string, starting from the first character
    // in the tree down to the leaf nodes.

    typedef unsigned int uint;
    typedef std::vector<uint> uint_vector;

    const std::string &s = str + dfa_string_dict::tree_end_of_string_marker;
    bool s_matched {false};
    std::string s_matched_string;
    uint s_matched_string_cost {0};

    uint_vector s_lev_row; // first row in Levenshtein distance matrix
    const uint s_lev_row_size = s.length() + 1;
    for(uint i=0; i<s_lev_row_size; i++) {
        s_lev_row.push_back(i);
    }

    // Set the first tree node to visit.
    std::stack<
            std::tuple<const dfa_tree<char>::node_t*, std::string, uint_vector>
            > unvisited_nodes;
    unvisited_nodes.push(
        std::make_tuple(&m_tree.root(), "", s_lev_row)
    );

    // Start visiting.
    while(!s_matched && !unvisited_nodes.empty()) {
        // Select one tree node to visit.
        const dfa_tree<char>::node_t* prev_node;
        std::string prev_read_string;
        uint_vector prev_lev_row;
        std::tie(prev_node,
                 prev_read_string,
                 prev_lev_row) = unvisited_nodes.top();
        unvisited_nodes.pop();

        const uint prev_lev_row_size = prev_lev_row.size();

        // Visit the selected tree node.
        for(auto it = prev_node->begin(); it != prev_node->end(); it++) {
            const std::string &curr_read_string = prev_read_string + it->first;

            // Compute current row in Levenshtein distance matrix.
            uint_vector curr_lev_row;
            curr_lev_row.push_back(prev_lev_row.at(0) + 1);
            for(uint i = 1; i < prev_lev_row_size; i++) {
                curr_lev_row.push_back(
                    std::min({
                        curr_lev_row.at(i-1) + 1, // insertion cost
                        prev_lev_row.at(i) + 1, // deletion cost
                        prev_lev_row.at(i-1) + (it->first == s.at(i-1) ? 0 : 1), // substitution cost
                    })
                );
            }

            // Check if we have reached a string matching the given edit distance criteria.
            const uint curr_lev_row_goal_cost = curr_lev_row.at(curr_lev_row.size()-1);
            if(curr_lev_row_goal_cost <= edit_max
            && it->first == dfa_string_dict::tree_end_of_string_marker) {
                s_matched = true;
                s_matched_string = curr_read_string;
                s_matched_string_cost = curr_lev_row_goal_cost;
            }

            // Save the tree node for later if the maximum edit cost has not
            // been exceeded. Indeed, next time we will be adding either 0 or
            // 1 to the costs in the current row of the computed Levenshtein
            // distance matrix.
            const uint curr_lev_row_min_cost = *std::min_element(
                curr_lev_row.begin(),
                curr_lev_row.end()
            );
            if(curr_lev_row_min_cost <= edit_max) {
                unvisited_nodes.push(
                    std::make_tuple(
                        &it->second,
                        curr_read_string,
                        curr_lev_row
                    )
                );
            }

            // Break early on match.
            if(s_matched) {
                break;
            }
        }
    }

    dfa_string_dict::match_result match;
    match.setData(
        "leven-match(" + std::to_string(edit_max) + ")",
        str,
        s_matched,
        [&]() { return "\"" + s + "\" matched successfully with \""
                     + s_matched_string + "\" using "
                     + std::to_string(s_matched_string_cost) + " edits"; },
        [&]() { return "\"" + s + "\" failed to match"; }
    );
    return match;
}

void dfa_string_dict::gather_strings(std::vector<std::string> &out) const
{
    out.clear();
    gather_strings([&out](const std::string &str) {
        out.push_back(str);
    });
}

void dfa_string_dict::gather_strings(
    const std::function<void (const std::string &)> &callback
) const
{
    std::string acc;
    gather_strings(m_tree.root(), acc, callback);
}

void dfa_string_dict::gather_strings(
    const dfa_tree<char>::node_t &node,
    std::string &acc,
    const std::function<void (const std::string &)> &callback
)
{
    // The algorithm below is recursive but we don't care because this function
    // is provided for debugging purpose only (e.g. print relatively small tree
    // content).

    for(auto it = node.begin(); it != node.end(); it++) {
        acc += it->first;
        if(it->second.has_children()) {
            dfa_string_dict::gather_strings(it->second, acc, callback);
        }
        else {
            callback(acc);
        }
        acc.pop_back(); // remove tree_end_of_string_marker
    }
}

void dfa_string_dict::print_strings(std::ostream &stream) const
{
    gather_strings([&stream](const std::string &str) {
        stream << str << std::endl;
    });
}

void dfa_string_dict::print_tree(std::ostream &stream) const
{
    dfa_tree_utils::print_tree_bracketed(m_tree, stream);
    stream << std::endl;
}
