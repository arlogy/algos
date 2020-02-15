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

#ifndef DFA_STRING_DICT_H
#define DFA_STRING_DICT_H

#include "dfa_tree.hpp"

#include <functional>
#include <vector>

/// A dictionary of strings using dfa_tree<char>.
class dfa_string_dict
{
public:
    /// Return type for string matching algorithms.
    struct match_data {
        std::string algorithm; // name (or short description) of the matching algorithm used
        std::string source;    // string to match in tree
        bool success {false};  // has string been matched?
        std::string message;   // message regarding success or failure

        /// Convenient initialization function to avoid duplicates in source code.
        void set(const std::string &algorithm,
                 const std::string &source,
                 bool success,
                 const std::function<std::string ()> &message_success,
                 const std::function<std::string ()> &message_failure)
        {
            this->algorithm = algorithm;
            this->source = source;
            this->success = success;
            this->message = this->success ? message_success() : message_failure();

            // Note that we are using a lambda for the success/failure message
            // instead of passing the message string directly because lambdas
            // are more flexible and can encapsulate extra logic.
        }

        /// Convenient informative functions.
        std::string short_str() const
        {
            return "running " + algorithm
                 + " on \"" + source + "\" "
                 + (success ? "succeeded" : "failed")
            ;
        }
        std::string full_str() const { return short_str() + ": " + message; }
    };

public:
    explicit dfa_string_dict();

    /// Adds string to this dictionary. Note that the string won't be added in
    /// case it contains the tree_end_of_string_marker character.
    bool add_string(const std::string &str);

    /// Adds strings from file using add_string().
    bool add_strings_from_file(const std::string &filename);

    /// Clears this dictionary.
    void clear();

    /// Exact string matching algorithm.
    ///     - Least permissive.
    ///     - Fastest.
    ///     - See comments on complexity in *.cpp file.
    match_data match_string_exactly(const std::string &str) const;

    /// Substitution string matching algorithm.
    ///     - More permissive than match_string_exactly().
    ///     - Faster than match_string_levenshtein_distance() limited to
    ///       substitutions only (i.e. no insertion or deletion).
    ///     - See comments on complexity in *.cpp file.
    match_data match_string_allow_substitution(const std::string &str,
                                               unsigned int subst_max = 0) const;

    /// Levenshtein string matching algorithm.
    ///     - Most permissive: allows substitution, insertion and deletion of
    ///       characters.
    ///     - Slowest.
    ///     - See comments on complexity in *.cpp file.
    match_data match_string_levenshtein_distance(const std::string &str,
                                                 unsigned int edit_max = 0) const;

    void fetch_strings(std::vector<std::string> &out) const;
    void fetch_strings(const std::function<void (const std::string &)> &callback) const;
    static void fetch_strings(const dfa_tree<char>::node_t &node,
                              std::string &acc,
                              const std::function<void (const std::string &)> &callback);

    void print_strings(std::ostream &stream) const;
    void print_tree(std::ostream &stream) const;

public:
    static const char tree_end_of_string_marker;

private:
    dfa_tree<char> m_tree;
};

#endif // DFA_STRING_DICT_H
