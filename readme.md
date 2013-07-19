
## Levenshtein-based approximate string matching for large-ish datasets

When working with real-world data, I've often encountered small-scale versions of the record linkage problem:  how to connect different data sets representing the same entity when no exact tie-point is available on which to perform a join.  This frequently happens when related information is stored and maintained in separate systems by separate stakeholders.

Both commercial and open-source tools are available for Oracle and MySQL to perform record linkage operations, and cloud-based MDM services are available from companies like Exorbyte.  But for the typical analyst crunching numbers in Excel or Access, no such tools are available.

### Limitations of common techniques

[Levenshtein algorithms](http://en.wikipedia.org/wiki/Levenshtein_distance) are commonly used for approximate string comparison.  The basic algorithm has time complexity O(i*j) and space complexity of O(n^2), and must be run (n1 * n2) times to compare two datasets with record counts n1 and n2, resulting in worst-case time complexity of O((i*j)(n1*n2)) for a record linkage operation.

Levenshtein algorithm can be substantially improved by by using dynamic backtracking with memoization (see [Fuzzy_Compare.cpp](https://github.com/stephenjlovell/fuzzy-matching/blob/master/Fuzzy_Compare.cpp)) for a basic implementation).  The improved algorithm still must run (n1 * n2) times.

### Further reductions in time complexity

Rather than matching each query string against each search string, a dictionary of unique search strings can be first compressed into a [Directed Acyclic Word Graph](http://en.wikipedia.org/wiki/Deterministic_acyclic_finite_state_automaton) (DAWG, aka deterministic acyclic finite state automaton).

The matching algorithm then traverses the DAWG depth-first, comparing it against the search term.  Traversal can be performed either recursively (levenshtein::Dawg::Search) or via an explicit queue (levenshtein::Dawg::Fuzzy_Match)

### sources

[fuzzy string matching II - matching word lists](http://fiber-space.de/wordpress/2011/01/07/fuzzy-string-matching-ii-matching-wordlists/)

[Compressing Dictionaries with a DAWG](http://stevehanov.ca/blog/index.php?id=115)

[Fast and Easy Levenshtein Distance Using a Trie](http://stevehanov.ca/blog/index.php?id=114)

[Levenshtein Distance, in three flavors](http://people.cs.pitt.edu/~kirk/cs1501/Pruhs/Spring2006/assignments/editdistance/Levenshtein%20Distance.htm)