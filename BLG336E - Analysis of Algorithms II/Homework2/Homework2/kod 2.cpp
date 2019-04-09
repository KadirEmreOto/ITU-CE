#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;
const int inf = 1e9 + 7;

struct Point {
    int x, y, z;
    
    Point();
    Point(int x, int y, int z);
    
    long long operator - (const Point& other) const;
    bool operator < (const Point& other) const;
    static bool compare (const Point* left, const Point* right);
};

Point::Point(){
    x = y = z = 0;
}

Point::Point(int x, int y, int z){
    this->x = x;
    this->y = y;
    this->z = z;
}

long long Point::operator - (const Point& other) const{
    long long diff = (other.x - x) * (other.x - x);
    diff += (other.y - y) * (other.y - y);
    diff += (other.z - z) * (other.z - z);
    
    return diff;
}

bool Point::operator < (const Point& other) const{
    return this->x < other.x;
}

bool Point::compare(const Point* left, const Point* right){
    return *left < *right;
}

pair<Point*, Point*> findClosest(Point** array, int size){
    if (size <= 10){
        pair<Point*, Point*> ans;
        long long dist = inf;
    
        for (int i=0; i < size; i++)
            for (int j=i+1; j < size; j++)
                if (*array[i] - *array[j] < dist){
                    dist = *array[i] - *array[j];
                    ans.first = array[i];
                    ans.second = array[j];
                }
        
        return ans;
    }
    
    else{
        pair<Point*, Point*> left = findClosest(array, size/2);
        pair<Point*, Point*> right = findClosest(array + size/2, size - size/2);
        
        pair<Point*, Point*> ans = left;
        
        long long dist = *(left.first) - *(left.second);
        double mid = (array[size/2]->x + array[size/2 - 1]->x) / 2.0;
        
        if (*(right.first) - *(right.second) < dist){
            dist = *(right.first) - *(right.second);
            ans = right;
        }
        
        for (int i=size/2 - 1; 0 <= i && (mid - array[i]->x)*(mid - array[i]->x) < dist; i--)
            for (int j=size/2; j < size && (array[j]->x - mid)*(array[j]->x - mid) < dist; j++)
                if (*array[i] - *array[j] < dist){
                    dist = *array[i] - *array[j];
                    ans.first = array[i];
                    ans.second = array[j];
                }
        
        return ans;
    }
}

vector<Point*> read(const char* filename){
    ifstream input(filename);
    int x, y, z, N;
    vector<Point*> points;
    
    input >> N;
    while (input >> x >> y >> z){
        Point* p = new Point(x, y, z);
        points.push_back(p);
    }
    
    return points;
}

void clear(vector<Point*> points){
    for (auto point: points)
        delete point;
}


int main(int argc, const char * argv[]) {
    vector<Point*> points = read("data/data1000.txt");
    std::sort(points.begin(), points.end(), Point::compare);
    
    pair<Point*, Point*> ans = findClosest(&points[0], (int)points.size());
    
    printf("(%d, %d, %d) - (%d, %d, %d) = %f \n", ans.first->x, ans.first->y, ans.first->z,
           ans.second->x, ans.second->y, ans.second->z, sqrt(*(ans.first) - *(ans.second)));
    
    clear(points);
    
    return 0;
}
