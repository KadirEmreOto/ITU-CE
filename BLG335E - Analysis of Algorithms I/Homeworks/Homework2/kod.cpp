#include <chrono>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>

const int MAXN = 1e7 + 5;
using namespace std;


struct Residence{
    // if the value of variable is -1, it means it is null
    int population;
    int minage;
    int maxage;
    char gender;  // 'M' for male, 'F' for female
    int zipcode;
    char geo_id[15];

    Residence(char* line){
        // construct the structure from given char*, which contains values

        char* next = slide(line);
        population = line + 1 != next ? atoi(line) : -1;

        line = next;
        next = slide(line);
        minage = line + 1 != next ? atoi(line) : -1 ;

        line = next;
        next = slide(line);
        maxage = line + 1 != next ? atoi(line) : -1;

        line = next;
        next = slide(line);
        gender = line[0];

        line = next;
        next = slide(line);
        zipcode = line + 1 != next ? atoi(line) : -1;

        line = next;
        strcpy(geo_id, line);
    }

    char* slide(char* line){
        // take one char pointer, 
        // find first ',' and replace it with null
        // return the next pointer to founded position ','

        int i=0;
        while (line[i++] != ',');
        line[i-1] = 0;
        return line + i;
    }

    bool operator< (const Residence& other){
        // overload < operator for convenience

        if (population != other.population)
            return population < other.population;

        return strcmp(geo_id, other.geo_id) < 0;
    } 
};


class Population{
public:
    Residence** residences;     // it holds the *pointers* of Residence
    Residence** backup;         // for rewind operations

    int size;  // size of population, number of residences

    Population(const char* filename, int size);
    ~Population();

    void save(const char* filename);
    void rewind();

    long long int sort();
    void QuickSort(Residence** array, int length);
};


Population::Population(const char* filename, int limit){ // constructor 
    this->size = 0;
    char temp[100];  // buffer for reading the lines of input

    FILE *input = fopen(filename, "r");
    int b = fscanf(input, "%[^\r]s", temp); fgetc(input); fgetc(input);  // fgetc reads tab character
	
    this->residences = new Residence*[limit];  // dynamic allocation
    this->backup = new Residence*[limit];

    while (!feof(input)){  // read until end of file character
        int b = fscanf(input, "%[^\r]s", temp); fgetc(input); fgetc(input);  // fgetc reads tab character
        if (feof(input))
            break;

        Residence* r = new Residence(temp);  // create new Residence
        
        this->residences[this->size] = r;  // add it to population (array)
        this->backup[this->size] = r;  // add it also to backup for rewind operation

        this->size++;
        
        if (limit <= this->size)
            break;
    }

    fclose(input);
}


Population::~Population(){
    // reallocation operations 
    for (int i=0; i < this->size; i++)
        delete this->residences[i];  
    delete [] this->residences;
    delete [] this->backup;
}


long long int Population::sort(){
    // sorts the populations and returns the elapsed time
    auto begin = std::chrono::steady_clock::now();
    this->QuickSort(this->residences, this->size);
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds> 
                        (std::chrono::steady_clock::now() - begin);
    return duration.count();
}


void Population::QuickSort(Residence** array, int length){
    if (length < 2)
        return;
    // pivot is the first element of array
    int sep=1;  // the first position that elements which are smaller than pivot can be swapped
    // swap(array[0], array[rand() % length]);  // for sellection random pivot

    for (int i=1; i < length; i++)  // partition
        if (*(array[i]) < *(array[0]))
            swap(array[sep++], array[i]);
        
    swap(array[0], array[sep-1]);  // take the pivot right position
    QuickSort(array, sep-1);  // sort the left part
    QuickSort(array+sep, length-sep);  // sort the right part
}


void Population::save(const char* filename){
    FILE *output = fopen(filename, "w");

    if (output == NULL){
        std::cerr << std::endl << "   File cannot be opened!" << std::endl << std::endl;
        return;
    }

    fprintf(output, "population,minimum_age,maximum_age,gender,zipcode,geo_id\r\n");

    for (int i=0; i < size; i++){
        if (this->residences[i]->population != -1)
            fprintf(output, "%d,", this->residences[i]->population);
        else
            fprintf(output, ",");
        if (this->residences[i]->minage != -1)
            fprintf(output, "%d,", this->residences[i]->minage);
        else
            fprintf(output, ",");
        if (this->residences[i]->maxage != -1)
            fprintf(output, "%d,", this->residences[i]->maxage);
        else
            fprintf(output, ",");
        if (!this->residences[i]->gender)
            fprintf(output, ",");
        else            
            fprintf(output, "%s,", this->residences[i]->gender == 'f' ? "female": "male");
        if (this->residences[i]->zipcode != -1)
            fprintf(output, "%d,", this->residences[i]->zipcode);
        else
            fprintf(output, ",");
        fprintf(output, "%s\r\n", this->residences[i]->geo_id);
    }

    fclose(output);
}

void Population::rewind(){
    for (int i=0; i < this->size; i++)
        this->residences[i] = this->backup[i];
}


int main(int argc, const char * argv[]){
    int size = atoi(argv[1]);
    int testcount = 1000000 / size;
    long long int totalduration = 0;

    Population P("population_by_zip_2010.csv", size);
    // Population P("output.csv", size);

    for (int i = 0; i < testcount; i++){
        P.rewind();  // rewind the residences array, because it can be sorted 
        long long int res = P.sort();
        totalduration += res;
        printf("[%d/%d] Time elapsed: %lld milliseconds\n", i+1, testcount, res);
    }

    P.save("output.csv");
    printf("\n    Average Result: %.2f milliseconds\n\n", totalduration / double(testcount));
    // P.save("output2.out");

    return 0;
}
