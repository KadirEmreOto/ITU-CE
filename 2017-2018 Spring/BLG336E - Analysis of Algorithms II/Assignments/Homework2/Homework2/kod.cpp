/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Compilation: g++ kod.cpp -o kod -O2 -std=c++11
 * Running Command: ./kod data/data1000.txt
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <algorithm>  // for sort

using namespace std;
const int inf = 1e9 + 7;
int counter;


struct Point {
    long long x, y, z, cmp;
    
    Point();
    Point(int x, int y, int z);
    
    long long operator - (const Point& other) const;
    bool operator < (const Point& other) const;
    static bool compare (const Point* left, const Point* right);
};

Point::Point(){  // default constructor
    x = y = z = 0;
}

Point::Point(int x, int y, int z){
    this->x = x;
    this->y = y;
    this->z = z;
    this->cmp = z;  // to set the axis easily to sort and compare
}

long long Point::operator - (const Point& other) const{  // euclidian distance
    long long diff = (other.x - x) * (other.x - x);
    diff += (other.y - y) * (other.y - y);
    diff += (other.z - z) * (other.z - z);
    
    counter++;
    return diff;
}

bool Point::operator < (const Point& other) const{
    return this->cmp < other.cmp;
}

bool Point::compare(const Point* left, const Point* right){  // for std::sort
    return *left < *right;
}

long long solve(Point** array, int size){  // returns the distance between the closest two points
    if (size <= 10){  // if size is smaller than 10, just brute-forces it: O(N^2)
        long long dist = inf;
        
        for (int i=0; i < size; i++)
            for (int j=i+1; j < size; j++)
                dist = min(*array[i] - *array[j], dist);
        
        return dist;
    }
    
    else{  // divide and conquer method
        long long dist = min(solve(array, size/2), solve(array + size/2, size - size/2));  // divide
        double mid = (array[size/2]->cmp + array[size/2 - 1]->cmp) / 2.0;
        
        for (int i=size/2 - 1; 0 <= i && (mid - array[i]->cmp)*(mid - array[i]->cmp) < dist; i--)  // conquer
            for (int j=size/2; j < size && (array[j]->cmp - mid)*(array[j]->cmp - mid) < dist; j++)
                dist = min(dist, *array[i] - *array[j]);  // set the answer
        
        return dist;
    }
}

vector<Point*> read(const char* filename){
    freopen(filename, "r", stdin);  // faster that ifstream
    
    int x, y, z, N;
    scanf("%d", &N);  // read the N number, representing the number of points
    vector<Point*> points(N);
    
    while (N--){
        scanf("%d%d%d", &x, &y, &z);  // read three point
        points[N] = new Point(x, y, z);
    }
    
    return points;
}

void clear(vector<Point*> points){  // frees the localed points in read function
    for (auto point: points)
        delete point;
}

int main(int argc, const char * argv[]) {
    vector<Point*> points = read(argv[1]);
    std::sort(points.begin(), points.end(), Point::compare);
    
    auto begin = std::chrono::steady_clock::now();
    
    double ans = sqrt(solve(&points[0], (int)points.size()));
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>
    (std::chrono::steady_clock::now() - begin);
    
    printf("The distance is %f\n", ans);
    printf("Number of total distance calculations is %d\n", counter);
    printf("Running time: %lld milliseconds\n", duration.count());
    
    clear(points);
    
    return 0;
}

