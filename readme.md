
## Levenshtein-based approximate string matching for large-ish datasets

When working with real-world data, analysts often encounter a small-scale version of the record linkage problem:  how to connect data from different sources that represent the same real-world entities, but where no exact tie-point connects the two datasets.  This frequently happens when related information is stored and maintained in separate systems by separate stakeholders.

Both commercial and open-source tools are available for Oracle and MySQL to perform record linkage operations, and cloud-based MDM services are available from companies like Exorbyte.  But for the typical analyst crunching numbers in Excel or Access, no such tools are available.

### Possible solutions

Levenshtein algorithms are commonly used for approximate string comparison, but these algorithms are too computationally expensive for compairing all possible record pairs between two sets.  

To make this work for record linkage, I've implemented a dynamic programming algorithm in vba (modFuzzyCompare) that employs backtracking with memoization to calculate Levenshtein edit distance between two strings.  This approach reduces computation cost by at least a factor of 10 compared to the standard implementation.  

The matching algorithm is wrapped in a new object class (RecordLinker ).  This provides an interface for passing parameters to the algorithm from a UI.

Additional cost savings could be obtained by creating a dictionary trie  for each dataset representing the domain of all strings in the dataset. 