Given a **dictionary of words** implemented as a **tree of characters**, we want
to check whether it contains a given input word. The dictionary may contain
**hundreds of thousands or even millions of words**, so optimizing the
implementation for performance is desirable.

The following rules can be applied gradually.
- **Zero constraint:** check whether a given word has been added to the
dictionary (no constraint applies here).
- **Substitution constraint:** same as above but substitutions are allowed. For
example if `word` is in the dictionary, `wood` can also be considered as part
of it. This costs 1 substitution: we have to transform the third letter `o`
into `r` to get a word in the dictionary.
- **Substitution/Insertion/Deletion constraint:** same as above but support
insertion and deletion as well. For example if `word` is in the dictionary,
    - `words` can also be considered as part of it. This costs 1 deletion
because we only need to remove the last character `s` to get a word in the
dictionary.
    - `way` can also be considered as part of it. We just need to replace the
second and third characters `a` and `y` by `o` and `r` respectively, then
add the missing `d` character to get a word in the dictionary. This costs 3
operations: 2 substitutions + 1 insertion + 0 deletion. Besides, another
workable but more expensive scenario would be to remove characters `a` and `y`
before adding the three missing ones, resulting in 5 operations in total.
There are other possible solutions but the first one is the most efficient.

Detailed comments on implementation can be found in source code.
