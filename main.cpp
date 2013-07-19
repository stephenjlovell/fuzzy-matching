#include "dawg.h"
#include "Benchmarks.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <set>
#include <vector>

int main(){
	std::set<std::string> test_results, benchmark_results, recursive_benchmark_results;
	std::vector<std::string> test_results_2;
	std::clock_t start;
	double t1, t2, t3, t4, t5;
	int k;
	int result_count= 0;
	long i = 0;
	std::string str;
	std::string test_1= "pusher";
	std::ifstream inFile;
	inFile.open("Input_File.txt");

	//input vector must be in alphanumeric order
	std::vector<std::string, std::allocator<std::string>> vecDictionary;
	std::cout << "Reading dictionary..." << std::endl;
	while(!inFile.fail() && i<=80000){
		std::getline(inFile,str);
		vecDictionary.push_back(str);
		i++;
	}

	std::cout << "Building DAWG..." << std::endl;
	start = std::clock();
	levenshtein::Dawg DAWG = levenshtein::Dawg(vecDictionary);
	// what needs to change in initialization of vecDictionary in order to be able to pass this parameter 
	// by reference, as in Dawg(&vecDictionary) 
	DAWG.finish();
	t1 = (std::clock()-start) / double(CLOCKS_PER_SEC);

	std::cout<< "DAWG creation took " << t1 << " seconds for a " << i << "-word dictionary." << std::endl;  // 20.757 s for 79,443 words 	
	std::cout<< "DAWG required " << double(sizeof(DAWG)) / 1000.0 << " kb of memory." << std::endl;  // 0.088 kb for 79,443 words
	std::cout<< "DAWG required " << DAWG.Count_Nodes() << " total nodes." << std::endl; 
	std::cout<< std::endl << "enter value for k: ";
	std::cin >> k;
	
	std::cout << std::endl;
	result_count=0;
	//start the clock
	start = std::clock();
	test_results = DAWG.Search(test_1,k);
	t2 = (std::clock()-start) / double(CLOCKS_PER_SEC);
	std::cout << std::endl << "Search Results:" << std::endl << std::endl;
	for(std::string result : test_results){
		std::cout << ++result_count << "\t" << result << std::endl;
	}
	std::cout << "Recursive DAWG search: " << t2 << "  Per word: " << t2 / (double(i)) << std::endl;

	std::cout << std::endl;
	result_count=0;
	//start the clock
	start = std::clock();
	test_results_2 = DAWG.Fuzzy_Match(test_1,k);
	t5 = (std::clock()-start) / double(CLOCKS_PER_SEC);
	for(std::string result : test_results_2){
		std::cout << ++result_count << "\t" << result << std::endl;
	}
	std::cout << "Deque-based bounded DFS: " << t5 << "  Per word: " << t5 / (double(i)) << std::endl;

	std::cout << std::endl;
	result_count=0;
	//start the clock
	start = std::clock();
	Distance* Standard_Algo = new Distance;
	
	for(std::string entry : vecDictionary){
		int L = Standard_Algo->LD(test_1.c_str(),entry.c_str());
		if (L <= k){
			benchmark_results.emplace(entry);
		}
	}
	t3 = (std::clock()-start) / double(CLOCKS_PER_SEC);
	for(std::string result : benchmark_results){
		std::cout << ++result_count << "\t" << result << std::endl;
	}

	std::cout << "best C++ levenshtein algorithm: " << t3 << "  Per word: " << t3 / (double(i)) << std::endl;
	delete Standard_Algo;

	Distance* Recursive_benchmark = new Distance;
	
	std::cout << std::endl;
	result_count=0;
	//start the clock
	start = std::clock();
	
	for(std::string entry : vecDictionary){
		if (Recursive_benchmark->FuzzyCompare_Init(test_1,entry,k)){
			recursive_benchmark_results.emplace(entry);
		}
	}
	t4 = (std::clock()-start) / double(CLOCKS_PER_SEC);
	for(std::string result : recursive_benchmark_results){ 
		std::cout << ++result_count << "\t" << result << std::endl;
	}

	std::cout << "Recursive Levenshtein-based matching: " << t4 << "  Per word: " << t4 / (double(i)) << std::endl;
	delete Recursive_benchmark;

 	return 0;
  // Unhandled exception at 0x00EA4EDF in Levenshtein_Automata.exe: 0xC0000005: Access violation reading location 0xFEEEFEEE.
  // Could be an issue with destructor Dawg::~Dawg trying to delete this->root
}



