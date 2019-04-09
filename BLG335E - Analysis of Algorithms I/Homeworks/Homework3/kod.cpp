/*
 * Author: Kadir Emre Oto (150140032)
 * Lecture: Analysis of Algorithms I (Project 3)
 */

// Compile: g++ kod.cpp -o kod -O2 -std=c++11
// Run: ./kod 


#include <chrono>
#include <iostream>
#include <vector>
#include <cmath>
#define A 1.6180339887498949  // the golden ratio number

using namespace std;
const int M = 131071;  // maximum input size


inline int myhash(int key){
	return M * (key * A - int(key * A));
}


inline int probing(int h, long i){
	return (h + 7*i + 3LL*i*i) % M;
}


class Dictionary{
public:
	pair<int, char>* table;  // hold the key, value pair 

	Dictionary();
	~Dictionary();

	int insertion(int key, char value);
	char lookup(int key);

	void block_insertion(const char* filename);
	void block_lookup(const char* filename, const char* outname);
};


Dictionary::Dictionary(){
	table = new pair<int, char>[M];
}


Dictionary::~Dictionary(){
	delete [] table;
}


int Dictionary::insertion(int key, char value){
	// adds the element to dictionary and  returns the number of collision 
	long i;
	int h = myhash(key);

	for (i=0; table[h].first != 0 && table[h].first != key; h = probing(h, ++i));

	table[h].first = key;
	table[h].second = value;
	return i;
}


char Dictionary::lookup(int key){
	int h = probing(myhash(key), 0);

	for (long i=0; table[h].first != key && table[h].first != 0; h = probing(h, ++i));

	return table[h].second;
}


void Dictionary::block_insertion(const char* filename){
	FILE *input = fopen(filename, "r");

	char value;
	int page, line, index, i=0, key;

	double collisions[4] = {0}, col;  // collisions holds the number of collisions 

	auto begin = std::chrono::system_clock::now();
	while (!feof(input)){
		fscanf(input, " %d%d%d", &page, &line, &index); 
		fgetc(input);
		value = fgetc(input);
		fgetc(input);

		key = page * 10000 + line * 100 + index;
		col = insertion(key, value);

		if (i < 1000)
			collisions[0] += col;
		if (i < 10000)
			collisions[1] += col;
		if (i < 100000)
			collisions[2] += col;

		collisions[3] += col;
		i++;
	}
	std::chrono::duration<double> duration = std::chrono::system_clock::now() - begin;
    
    printf("    DICTIONARY\n");
    printf("    Insertion finished after %.2f seconds \n\n", duration.count());

    printf("    Average number of collisions (first 1,000)    | %.3f \n", collisions[0] / 1000.);
    printf("    Average number of collisions (first 10,000)   | %.3f \n", collisions[1] / 10000.);
    printf("    Average number of collisions (first 100,000)  | %.3f \n", collisions[2] / 100000.);
    printf("    Average number of collisions (overall)        | %.3f \n", collisions[3] / i);
}


void Dictionary::block_lookup(const char* filename, const char* outname){
	FILE *input = fopen(filename, "r");
	FILE *output = fopen(outname, "w");

	int page, line, index, i=0, key;	
	char value;

	auto begin = std::chrono::system_clock::now();

	while (!feof(input)){
		fscanf(input, " %d%d%d", &page, &line, &index); 
		key = page * 10000 + line * 100 + index;
		value = lookup(key);

		fprintf(output, "%d\t%d\t%d\t%c\n", page, line, index, value);
	}

	std::chrono::duration<double> duration = std::chrono::system_clock::now() - begin;
    
    printf("\n    Lookup finished after %.2f seconds \n\n", duration.count());
}


class List
{
public:
	vector<pair<int, char> > table;  // hold the key, value pair  

	void insertion(int key, char value);
	char lookup(int key);

	void block_insertion(const char* filename);
	void block_lookup(const char* filename, const char* outname);
};


void List::insertion(int key, char value){
	table.push_back(pair<int, char>(key, value));
}


char List::lookup(int key){
	int i;
	for (i=0; i < table.size() && table[i].first != key; i++);
	return table[i].second;
}


void List::block_insertion(const char* filename){
	FILE *input = fopen(filename, "r");

	char value;
	int page, line, index, key;

	auto begin = std::chrono::system_clock::now();
	while (!feof(input)){
		fscanf(input, " %d%d%d", &page, &line, &index); 
		fgetc(input);
		value = fgetc(input);
		fgetc(input);

		key = page * 10000 + line * 100 + index;
		insertion(key, value);
	}
	std::chrono::duration<double> duration = std::chrono::system_clock::now() - begin;
    
    printf("    LIST\n");
    printf("    Insertion finished after %.2f seconds \n", duration.count());
}


void List::block_lookup(const char* filename, const char* outname){
	FILE *input = fopen(filename, "r");
	FILE *output = fopen(outname, "w");

	int page, line, index, i=0, key;	
	char value;

	auto begin = std::chrono::system_clock::now();

	while (!feof(input)){
		fscanf(input, " %d%d%d", &page, &line, &index); 
		key = page * 10000 + line * 100 + index;
		value = lookup(key);

		fprintf(output, "%d\t%d\t%d\t%c\n", page, line, index, value);
	}

	std::chrono::duration<double> duration = std::chrono::system_clock::now() - begin;
    
    printf("    Lookup finished after %.2f seconds \n\n", duration.count());
}



int main(int argc, char const *argv[]){
	Dictionary d;
	d.block_insertion("ds-set-input.txt");
	d.block_lookup("ds-set-lookup.txt", "output-dict.txt");

	List l;
	l.block_insertion("ds-set-input.txt");
	l.block_lookup("ds-set-lookup.txt", "output-list.txt");

	return 0;
}