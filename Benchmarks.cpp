#include "Benchmarks.h"
#include <string.h>
#include <malloc.h>


int Distance::Minimum (int a, int b, int c)
{
int mi;

  mi = a;
  if (b < mi) {
    mi = b;
  }
  if (c < mi) {
    mi = c;
  }
  return mi;

}

//**************************************************
// Get a pointer to the specified cell of the matrix
//************************************************** 

int *Distance::GetCellPointer (int *pOrigin, int col, int row, int nCols)
{
  return pOrigin + col + (row * (nCols + 1));
}

//*****************************************************
// Get the contents of the specified cell in the matrix 
//*****************************************************

int Distance::GetAt (int *pOrigin, int col, int row, int nCols)
{
int *pCell;

  pCell = GetCellPointer (pOrigin, col, row, nCols);
  return *pCell;

}

//*******************************************************
// Fill the specified cell in the matrix with the value x
//*******************************************************

void Distance::PutAt (int *pOrigin, int col, int row, int nCols, int x)
{
int *pCell;

  pCell = GetCellPointer (pOrigin, col, row, nCols);
  *pCell = x;

}

//*****************************
// Compute Levenshtein distance
//*****************************

int Distance::LD (char const *s, char const *t)
{
int *d; // pointer to matrix
int n; // length of s
int m; // length of t
int i; // iterates through s
int j; // iterates through t
char s_i; // ith character of s
char t_j; // jth character of t
int cost; // cost
int result; // result
int cell; // contents of target cell
int above; // contents of cell immediately above
int left; // contents of cell immediately to left
int diag; // contents of cell immediately above and to left
int sz; // number of cells in matrix

  // Step 1	

  n = strlen (s);
  m = strlen (t);
  if (n == 0) {
    return m;
  }
  if (m == 0) {
    return n;
  }
  sz = (n+1) * (m+1) * sizeof (int);
  d = (int *) malloc (sz);

  // Step 2

  for (i = 0; i <= n; i++) {
    PutAt (d, i, 0, n, i);
  }

  for (j = 0; j <= m; j++) {
    PutAt (d, 0, j, n, j);
  }

  // Step 3

  for (i = 1; i <= n; i++) {

    s_i = s[i-1];

    // Step 4

    for (j = 1; j <= m; j++) {

      t_j = t[j-1];

      // Step 5

      if (s_i == t_j) {
        cost = 0;
      }
      else {
        cost = 1;
      }

      // Step 6 

      above = GetAt (d,i-1,j, n);
      left = GetAt (d,i, j-1, n);
      diag = GetAt (d, i-1,j-1, n);
      cell = Minimum (above + 1, left + 1, diag + cost);
      PutAt (d, i, j, n, cell);
    }
  }

  // Step 7

  result = GetAt (d, n, m, n);
  free (d);
  return result;
	
}


bool Distance::FuzzyCompare_Init(std::string str1, std::string str2, int k){
	int N1, N2, intMax;
	int Visited[255][255] = {};
	N1 = str1.length();
	N2 = str2.length();
	if (N1>N2) intMax = N1; else intMax = N2;

	return FuzzyCompare_Main(Visited,str1,str2,N1,N2,k,0,0);
};

bool Distance::FuzzyCompare_Main(int Visited[][255], std::string str1, std::string str2, int N1, int N2, int k, int i, int j){
	while (true){
		if ((N1 - i <= k) && (N2 - j <= k)) return true; 
		if ((i >= N1) || (j >= N2)) return false;
		if (str1[i] == str2[j]){
			Visited[i][j] = k;
			i++;
			j++;
			continue;
		} else {
			if (k == 0) return false;
			if(!FuzzyCompare_Memo(Visited,i+1,j,k-1)){ if(FuzzyCompare_Main(Visited,str1,str2,N1,N2,k-1,i+1,j)) return true;};
			if(!FuzzyCompare_Memo(Visited,i,j+1,k-1)){ if(FuzzyCompare_Main(Visited,str1,str2,N1,N2,k-1,i,j+1)) return true;};
			if(!FuzzyCompare_Memo(Visited,i+1,j+1,k-1)){ if(FuzzyCompare_Main(Visited,str1,str2,N1,N2,k-1,i+1,j+1)) return true;};
			return false;
		};
	};
};

bool Distance::FuzzyCompare_Memo(int Visited[][255], int i, int j, int k){ if(Visited[i][j]<=k) return false; else return true;};
