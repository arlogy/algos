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

#ifndef DFA_TREE_H
#define DFA_TREE_H

#include <map>

/// A node with possible connections to child nodes. Designed for use with the
/// dfa_tree tree implementation available below.
template<typename T>
class dfa_tree_node
{
private:
    typedef std::map<T, dfa_tree_node> map_t; // map type for this nodes's children

public:
    explicit dfa_tree_node() {}

    /// Returns a possibly null pointer to a chid of this node.
    const dfa_tree_node* child_ptr(const T &input) const
    {
        try { return &m_children.at(input); } catch(std::out_of_range &) {}
        return nullptr;
    }
    dfa_tree_node* child_ptr(const T &input)
    {
        const auto &this_const_ref = *this;
        return const_cast<dfa_tree_node*>(this_const_ref.child_ptr(input));
    }

    /// Inserts and returns this node's child for the given input. The child
    /// node is inserted only once.
    dfa_tree_node& set_child(const T &input) { return m_children[input]; }

    /// Removes the child node for the given input and returns success/failure.
    bool unset_child(const T &input) { return m_children.erase(input) == 1; }

    /// Other functions to query information about this nodes's children.
    size_t number_of_children() const { return m_children.size(); }
    bool has_children() const { return !m_children.empty(); }

    /// Removes this node's children.
    void clear() { m_children.clear(); }

    /// std::map::begin() functions to iterate over this node's children.
    typename map_t::iterator begin() { return m_children.begin(); }
    typename map_t::const_iterator begin() const { return m_children.begin(); }

    /// std::map::end() functions to iterate over this node's children.
    typename map_t::iterator end() { return m_children.end(); }
    typename map_t::const_iterator end() const { return m_children.end(); }

private:
    map_t m_children;
};

/// A tree inspired from deterministic finite automatons (DFAs). It is a tree
/// such that:
///     - each node can have arbitrary number of children.
///     - from a node and given an input one node at most can be reached.
/// Pros:
///     - memory-efficient.
///     - access any child node in logarithmic time at most (due to children
///       being stored in std::map).
/// Cons:
///     - not versatile (limited to top->bottom tree traversal only).
///     - tree traversal does not preserve the order in which nodes are inserted
///       (order is defined by std::map).
template<typename T>
class dfa_tree
{
public:
    typedef dfa_tree_node<T> node_t; // node type

public:
    explicit dfa_tree() {}

    /// Returns the root node of this tree.
    node_t& root() { return m_root; }
    const node_t& root() const { return m_root; }

    /// Removes the children of the root node in this tree.
    void clear() { m_root.clear(); }

private:
    node_t m_root;
};

#endif // DFA_TREE_H
