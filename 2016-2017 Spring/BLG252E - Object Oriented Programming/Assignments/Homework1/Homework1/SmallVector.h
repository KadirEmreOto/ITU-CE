/*
 *  File: SmallVector.h
 *  Name: Kadir Emre Oto
 *  Date: 28/03/2017
 *  Student ID: 150140032
 */

#ifndef SmallVector_h
#define SmallVector_h

int* copy_array_with_size(int* array, int length, int size, bool reverse=false);

class SmallVector{
private:
    int* static_array;
    int* dynamic_array;
    
    int size;
    int capacity;
    
    void expand();
    void shrink();
    
public:
    SmallVector();
    SmallVector(int* array, int length);
    SmallVector(const SmallVector& other);
    ~SmallVector();
    
    int& operator[](int index) const;
    void operator=(const SmallVector& other);
    SmallVector operator*(const int other);
    SmallVector operator+(SmallVector& other) const;
    
    int getSize() const;
    int getCapacity() const;
    SmallVector reverse();
    
    void push_back(int number);
    void push_back(int* array, int size);
    int pop_back();
};

#endif /* SmallVector_h */
