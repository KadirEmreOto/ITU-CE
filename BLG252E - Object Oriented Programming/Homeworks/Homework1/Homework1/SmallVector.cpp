/*
 *  File: SmallVector.cpp
 *  Name: Kadir Emre Oto
 *  Date: 28/03/2017
 *  Student ID: 150140032
 */

#include "SmallVector.h"

int* copy_array_with_size(int* array, int length, int size, bool reverse){
    int* out = new int [size];

    for (int i=0; i < length; i++){
        if (!reverse)
            out[i] = array[i];
        else
            out[i] = array[length - i - 1];
    }

    return out;
}

SmallVector::SmallVector(){
    size = 0;
    capacity = 2;
    static_array = new int [32];
    dynamic_array = new int [capacity];
}

SmallVector::SmallVector(int* array, int length){
    size = 0;
    capacity = 2;
    static_array = new int [32];
    dynamic_array = new int [capacity];

    for (int i=0; i < length; i++)
        this->push_back(array[i]);
}

SmallVector::SmallVector(const SmallVector& other){
    size = other.size;
    capacity = other.capacity;

    static_array = new int [32];
    dynamic_array = new int [capacity];

    for (int i=0; i < other.getSize(); i++)
        this->push_back(other[i]);
}

SmallVector::~SmallVector(){
    delete [] static_array;
    delete [] dynamic_array;
}

int& SmallVector::operator[](int index) const{
    if (index < 0)
        index += getSize();

    if (index < 32)
        return static_array[index];

    else
        return dynamic_array[index - 32];
}

SmallVector SmallVector::operator+(SmallVector& other) const{
    SmallVector out;

    for (int i=0; i < this->getSize(); i++)
        out.push_back((*this)[i]);

    for (int i=0; i < other.getSize(); i++)
        out.push_back(other[i]);

    return out;
}

void SmallVector::operator=(const SmallVector& other){
    if (this->getSize() > 0){
        delete [] static_array;
        delete [] dynamic_array;

        size = 0;
        capacity = 2;
        static_array = new int [32];
        dynamic_array = new int [capacity];
    }

    for (int i=0; i < other.getSize(); i++)
        this->push_back(other[i]);
}

SmallVector SmallVector::operator*(const int times){
    SmallVector out;

    for (int i=0; i < times; i++)
        for (int j=0; j < getSize(); j++)
            out.push_back((*this)[j]);

    return out;
}

int SmallVector::getSize() const{
    return size;
}

int SmallVector::getCapacity() const{
    return capacity + 32;
}

void SmallVector::expand(){
    int* temp = copy_array_with_size(dynamic_array, size - 32, capacity * 2);

    delete [] dynamic_array;
    dynamic_array = temp;

    capacity *= 2;
}

void SmallVector::shrink(){
    int* temp = copy_array_with_size(dynamic_array, size - 32, capacity / 2);

    delete [] dynamic_array;
    dynamic_array = temp;

    capacity /= 2;
}

void SmallVector::push_back(int number){
    if (size < 32)
        static_array[size++] = number;

    else if (size - 32 < capacity)
        dynamic_array[(size++) - 32] = number;

    else{
        expand();
        dynamic_array[(size++) - 32] = number;
    }
}

void SmallVector::push_back(int* array, int size){
    for (int i=0; i < size; i++)
    	push_back(array[i]);
}

int SmallVector::pop_back(){
    int out;

    if (size <= 32)
        out = static_array[--size];

    else{
        out = dynamic_array[(--size) - 32];
        if (size - 32 < capacity / 2 && capacity > 2)
            shrink();
    }

    return out;
}

SmallVector SmallVector::reverse(){
    SmallVector out;

    for (int i=0; i < getSize(); i++)
        out.push_back((*this)[-i-1]);

    return out;
}
