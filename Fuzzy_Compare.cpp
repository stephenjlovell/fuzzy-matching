#include <string>
#include <iostream>

//Global variables (for debugging purposes):
long FCalls = 0;

// Function Prototypes:
bool FuzzyCompare_Init(std::string str1, std::string str2, double dblRelevance);
bool FuzzyCompare_Main(int Visited[][255], std::string str1, std::string str2, int N1, int N2, int k, int i, int j);
int FuzzyCompare_K(int intMaxLen, double dblRelevance);
bool FuzzyCompare_Memo(int Visited[][255], int i, int j, int k);

// Function Implementation:

int main(){
	int Visited[255][255] = {};
	bool boolTest;

	for(double r=0.0;r<=1.0;r+=0.05){  
		//Diplay whether algorithm returns a match for different normalized "relevance" values r.
		boolTest = FuzzyCompare_Init("This is a string","This is a long string",r);
		std::cout << r << "   " << boolTest << std::endl;
	};
	return 0;
};

bool FuzzyCompare_Init(std::string str1, std::string str2, double dblRelevance){
	int N1, N2, intMax, k;
	int Visited[255][255] = {};
	N1 = str1.length();
	N2 = str2.length();
	FCalls=0;
	if (N1>N2) intMax = N1; else intMax = N2;

	k = FuzzyCompare_K(intMax,dblRelevance);

	return FuzzyCompare_Main(Visited,str1,str2,N1,N2,k,0,0);
};

bool FuzzyCompare_Main(int Visited[][255], std::string str1, std::string str2, int N1, int N2, int k, int i, int j){
	FCalls++;
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

int FuzzyCompare_K(int intMaxLen, double dblRelevance){return int(double(intMaxLen) * (1.0 - dblRelevance)+0.5);};

bool FuzzyCompare_Memo(int Visited[][255], int i, int j, int k){ if(Visited[i][j]<=k) return false; else return true;};




