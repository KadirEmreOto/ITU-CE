/*
 * Author: Kadir Emre Oto (150140032)
 * Lecture: Analysis of Algorithms I (Project 1)
 */

// Compile: g++ kod.cpp -o kod -O2 -std=c++11
// Run: ./kod -full -m hs-set-10k.txt out.txt

#include <chrono>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <iostream>

const int MAXN = 1e7 + 5;  // maximum card count


struct Card{
    int cost;
    char name[30];  // all strings stored as char arrays instead of std::string to speed-up 
    char className[20];
    char rarity[20];
    char set[30];
    char type[20];
};


class CardManager{
public:
    Card** cards;  // it holds pointers of Card Struct (to speed-up swap operation)
    Card** temp;  // for merge process
    int size;  // number of cards

    CardManager(const char* filename);
    ~CardManager();

    bool compare(Card* left, Card* right, int proc);
    int strcompare(const char* left, const char* right);

    void fullSort(int algo);
    void filterSort(int algo);
    void sortHelper(int algo, int proc);

    void mergeSort(Card** ar, int size, int proc);
    void insertionSort(Card** ar, int size, int proc);

    void load(const char* filename);
    void save(const char* filename);
};


CardManager::CardManager(const char* filename){ 
    cards = new Card*[MAXN];  // dynamic allocation for cards 
    temp = new Card*[MAXN];  // THEY CAN CAUSE MEMORY LEAK

    size = 0;  // set number of cards is zero
    load(filename);
}

CardManager::~CardManager(){
    for (int i=0; i < size; i++)
        delete cards[i];  // delete all allocated card structs

    delete [] cards;  // reallocation of pointers
    delete [] temp;
}

void CardManager::load(const char* filename){ // open input file and add cards 
    FILE *input = fopen(filename, "r");

    if (input == NULL){
        std::cerr << std::endl << "   File cannot be opened!" << std::endl << std::endl;
        return;
    }

    while (!feof(input)){  // read until end of file character
        cards[size] = new Card;
        fscanf(input, "%[^\t]s", cards[size]->name); fgetc(input);  // fgetc reads tab character
        
        if (feof(input)){
            delete cards[size];  
            break;
        }
        
        fscanf(input, "%[^\t]s", cards[size]->className); fgetc(input);
        fscanf(input, "%[^\t]s", cards[size]->rarity); fgetc(input);
        fscanf(input, "%[^\t]s", cards[size]->set); fgetc(input);
        fscanf(input, "%[^\t]s", cards[size]->type); fgetc(input);
        fscanf(input, "%d", &cards[size]->cost);
        fgetc(input); fgetc(input);  // "fgetc"s read \r\n characters

        size++;  // increase number of cards
    }

    fclose(input);
}

void CardManager::save(const char* filename){  // save all cards to a file
    FILE *output = fopen(filename, "w");

    if (output == NULL){
        std::cerr << std::endl << "   File cannot be opened!" << std::endl << std::endl;
        return;
    }

    for (int i=0; i < size; i++){
        fprintf(output, "%s\t", cards[i]->name);
        fprintf(output, "%s\t", cards[i]->className);
        fprintf(output, "%s\t", cards[i]->rarity);
        fprintf(output, "%s\t", cards[i]->set);
        fprintf(output, "%s\t", cards[i]->type);
        fprintf(output, "%d\r\n", cards[i]->cost);
    }

    fclose(output);
}

void CardManager::fullSort(int algo){
    sortHelper(algo, 1);  // proc = 1
}

void CardManager::filterSort(int algo){
    sortHelper(algo, 0);  // proc = 0
}

void CardManager::sortHelper(int algo, int proc){  // helper function for sorting
    // algo = {0: mergeSort, 1: insertionSort}; proc = {0: filterSort, 1: fullSort}

    auto begin = std::chrono::steady_clock::now();

    if (algo == 0)
        mergeSort(cards, size, proc);
    else
        insertionSort(cards, size, proc);

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds> 
                        (std::chrono::steady_clock::now() - begin);
    printf("\n   Time elapsed: %lld milliseconds\n\n", duration.count());
}

inline int CardManager::strcompare(const char* left, const char* right){
    // return -1 if left is smaller, 0 if left equals to right, 1 if right is smaller
    int l_size = strlen(left);
    int r_size = strlen(right);

    int l_cur=0, r_cur = 0;  // cursors for strings (they are needed to ignore punctuations)

    while (l_cur < l_size && r_cur < r_size){
        // ignore some punctuations: '\'' and '-'
        for (; l_cur < l_size && (left[l_cur] == '\'' || left[l_cur] == '-'); l_cur++);
        for (; r_cur < r_size && (right[r_cur] == '\'' || right[r_cur] == '-'); r_cur++);

        // following if-else block is for case-insensitive comparison
        // it can be done with tolower function too, but this way is faster  

        if (left[l_cur] > 'Z'){  // if 'left' is lower
            if (right[r_cur] > 'Z'){  // also 'right' is lower
                if (left[l_cur] != right[r_cur]) 
                    return left[l_cur] < right[r_cur] ? -1 : 1;
            }

            else{  // 'right' is upper, so we have to make 'left' upper too
                if (left[l_cur] - 32 != right[r_cur])  
                    return (left[l_cur] - 32 < right[r_cur]) ? -1 : 1;
            }
        }

        else{  // left is upper
            if (right[r_cur] > 'Z'){  // but right is lower
                if (left[l_cur] + 32 != right[r_cur])  // make 'left' lower and compare
                    return left[l_cur] + 32 < right[r_cur] ? -1 : 1;
            }

            else{  // right is upper
                if (left[l_cur] != right[r_cur])
                    return (left[l_cur] < right[r_cur]) ? -1 : 1;
            }
        }

        l_cur++;
        r_cur++;
    }

    if (l_size == l_cur && r_size == r_cur)  // it means strings are identical
        return 0;

    return r_cur < r_size ? -1 : 1;  // if right didn't come to end it means left is smaller, vice versa
}

inline bool CardManager::compare(Card* left, Card* right, int proc){
    // return true if left is smaller, else false
    if (proc){  // proc = 1 -> fullSort
        int cmp = strcmp(left->className, right->className);

        if (cmp < 0)
            return true;

        else if (cmp == 0){
            if (left->cost < right->cost)
                return true;

            else if (left->cost == right->cost)
                return strcompare(left->name, right->name) < 0;
        }
        return false;
    }

    else  // proc = 0 -> filterSort
        return strcmp(left->type, right->type) < 0;
}

void CardManager::mergeSort(Card** ar, int size, int proc){
    if (size > 1){
        mergeSort(ar, size >> 1, proc);  // sort first part
        mergeSort(ar + (size >> 1), size - (size >> 1), proc);  // sort second part

        // and then merge these two parts
        int i = 0;  // traverse first part
        int j = size >> 1;  // traverse second part

        int index = 0;  // traverse temp array

        while (index < size){ 
            if (j == size || (i < size / 2 && compare(ar[i], ar[j], proc)))
                temp[index++] = ar[i++];

            else
                temp[index++] = ar[j++];
        }

        for (int i=0; i < size; i++)  // bring all items back to the actual array
            ar[i] = temp[i];
    }
}

void CardManager::insertionSort(Card** ar, int size, int proc){
    for (int i=1; i < size; i++)  // for all indices from 1 to size
        for (int j=i-1; 0 <= j && compare(ar[j+1], ar[j], proc); j--)  // slide until it comes to the right position
            std::swap(ar[j], ar[j+1]);
}

int main(int argc, const char * argv[]) {
    if (argc != 5){
        std::cerr << "Command-line options are incompatible" << std::endl;
        return -1;
    }

    int algo = 0;  // default mergeSort

    if (strcmp(argv[2], "-i") == 0)
        algo = 1;

    else if (strcmp(argv[2], "-m") != 0){
        std::cerr << "Command-line options are incompatible" << std::endl;
        return -1;
    }

    CardManager manager(argv[3]);

    if (strcmp(argv[1], "-full") == 0)
        manager.fullSort(algo);

    else if ((strcmp(argv[1], "-filter") == 0))
        manager.filterSort(algo);

    else{
        std::cerr << "Command-line options are incompatible" << std::endl;
        return -1;
    }

    manager.save(argv[4]);
    return 0;
}
