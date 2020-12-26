#include <chrono>
#include <iostream>
#include <vector>

using namespace std;
const double A = 0.618;
const int M = 131071;


int myhash(int key){
	return M * (key * A - int(key * A));
}


int probing(int h, int i){
	return (h + 7*i + 3*long(i)*i) % M;
}


class Dictionary{
public:
	pair<int, char>* table;

	Dictionary();
	~Dictionary();

	int insertion(int key, char value);
	char lookup(int key);

	void block_insertion(const char* filename);
	void block_lookup(const char* filename);
};


Dictionary::Dictionary(){
	table = new pair<int, char>[M];
}


Dictionary::~Dictionary(){
	delete [] table;
}


int Dictionary::insertion(int key, char value){
	int h = myhash(key), p = probing(h, 0); long i;
	for (i=0; table[p].first != 0 && table[p].first != key; p = probing(h, ++i));

	table[p].first = key;
	table[p].second = value;
	return i;
}


char Dictionary::lookup(int key){
	int h = myhash(key), p = probing(h, 0); long i;
	for (i=0; table[p].first != key && table[p].first != 0; p = probing(h, ++i));

	return table[p].second;
}


void Dictionary::block_insertion(const char* filename){
	FILE *input = fopen(filename, "r");

	char value;
	int page, line, index, i=0, key;

	double collisions[4] = {0}, col;

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

    printf("    Average number of collisions (first 1,000)    | %.2f \n", collisions[0] / 1000.);
    printf("    Average number of collisions (first 10,000)   | %.2f \n", collisions[1] / 10000.);
    printf("    Average number of collisions (first 100,000)  | %.2f \n", collisions[2] / 100000.);
    printf("    Average number of collisions (overall)        | %.2f \n", collisions[3] / i);
}


void Dictionary::block_lookup(const char* filename){
	FILE *input = fopen(filename, "r");
	FILE *output = fopen("output.txt", "w");

	int page, line, index, i=0, key;	
	char value;
	int prepage = 0;
	int preline = 0;

	auto begin = std::chrono::system_clock::now();

	while (!feof(input)){
		fscanf(input, " %d%d%d", &page, &line, &index); 
		key = page * 10000 + line * 100 + index;
		value = lookup(key);

		if (prepage != page || preline != line)
			fprintf(output, "\n");	

		fputc(value, output);	

		prepage = page;
		preline = line;
	}

	std::chrono::duration<double> duration = std::chrono::system_clock::now() - begin;
    
    printf("\n    Lookup finished after %.2f seconds \n\n", duration.count());
    
}


class List
{
public:
	vector<pair<int, char>> table;

	void insertion(int key, char value);
	char lookup(int key);

	void block_insertion(const char* filename);
	void block_lookup(const char* filename);
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


void List::block_lookup(const char* filename){
	FILE *input = fopen(filename, "r");
	FILE *output = fopen("output2.txt", "w");

	int page, line, index, i=0, key;	
	char value;
	int prepage = 0;
	int preline = 0;

	auto begin = std::chrono::system_clock::now();

	while (!feof(input)){
		fscanf(input, " %d%d%d", &page, &line, &index); 
		key = page * 10000 + line * 100 + index;
		value = lookup(key);

		if (prepage != page || preline != line)
			fprintf(output, "\n");

		fputc(value, output);	

		prepage = page;
		preline = line;
	}

	std::chrono::duration<double> duration = std::chrono::system_clock::now() - begin;
    
    printf("    Lookup finished after %.2f seconds \n\n", duration.count());
}



int main(int argc, char const *argv[]){
	Dictionary d;
	d.block_insertion("ds-sets/ds-set-input.txt");
	d.block_lookup("ds-sets/ds-set-lookup.txt");

	List l;
	l.block_insertion("ds-sets/ds-set-input.txt");
	l.block_lookup("ds-sets/ds-set-lookup.txt");

	return 0;
}