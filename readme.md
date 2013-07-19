
## Levenshtein-based approximate string matching for large-ish datasets

When working with real-world data, I've often encountered small-scale versions of the record linkage problem:  how to connect data from different sources that represent the same real-world entities, but where no exact tie-point connects the two datasets.  This frequently happens when related information is stored and maintained in separate systems by separate stakeholders.

Both commercial and open-source tools are available for Oracle and MySQL to perform record linkage operations, and cloud-based MDM services are available from companies like Exorbyte.  But for the typical analyst crunching numbers in Excel or Access, no such tools are available.

### Limitations of common techniques

[Levenshtein algorithms](http://en.wikipedia.org/wiki/Levenshtein_distance) are commonly used for approximate string comparison, but these algorithms are too time and space complex for compairing all possible record pairs n1 * n2. 

Levenshtein algorithm can be substantially improved by by using dynamic backtracking with memoization (see Fuzzy_Compare.cpp for a basic implementation).

Even with the faster algo