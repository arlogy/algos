Given a **dictionary of words** built as a **tree of characters**, we want to
check whether this dictionary contains a given input word. The following rules
can be gradually taken into account, including that the dictionary could
contain **hundreds of thousands (to millions?) of words**. So optimizing the
source code would be ideal for better performance.

- **Zero constraint:** tells whether a given word has been added to the
dictionary (no constraint applies here).
- **Substitution constraint:** same as above but substitutions are allowed. For
example if `word` is in the dictionary, `wood` can also be considered as part
of it. This costs 1 substitution: we have to transform the third letter `o`
into `r` to get a word in the dictionary.
- **Substitution/Insertion/Deletion constraint:** same as above but supports
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
