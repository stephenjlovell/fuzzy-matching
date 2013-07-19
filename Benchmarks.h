#ifndef _BENCHMARKS_H_
#define _BENCHMARKS_H_

#include <string>

class Distance  // source http://people.cs.pitt.edu/~kirk/cs1501/Pruhs/Spring2006/assignments/editdistance/Levenshtein%20Distance.htm
{
  public:
    int LD (char const *s, char const *t);
	bool FuzzyCompare_Init(std::string str1, std::string str2, int k);
  private:
    int Minimum (int a, int b, int c);
    int *GetCellPointer (int *pOrigin, int col, int row, int nCols);
    int GetAt (int *pOrigin, int col, int row, int nCols);
    void PutAt (int *pOrigin, int col, int row, int nCols, int x);
	bool FuzzyCompare_Main(int Visited[][255], std::string str1, std::string str2, int N1, int N2, int k, int i, int j);
	bool FuzzyCompare_Memo(int Visited[][255], int i, int j, int k);
}; 

#endif