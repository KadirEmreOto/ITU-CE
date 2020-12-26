#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;
const int inf = 1e9 + 7;
long double total;


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
    //total += sqrt(diff);
    return diff;
}

bool Point::operator < (const Point& other) const{
    return this->x < other.x;
}

bool Point::compare(const Point* left, const Point* right){
    return *left < *right;
}

long long solve(Point** array, int size){
    if (size <= 3){
        long long dist = inf;
    
        for (int i=0; i < size; i++)
            for (int j=i+1; j < size; j++)
                dist = min(*array[i] - *array[j], dist);
        
        return dist;
    }
    
    else{
        long long dist = min(solve(array, size/2), solve(array + size/2, size - size/2));
        double mid = (array[size/2]->x + array[size/2 - 1]->x) / 2.0;
        
        for (int i=size/2 - 1; 0 <= i && (mid - array[i]->x)*(mid - array[i]->x) < dist; i--)
            for (int j=size/2; j < size && (array[j]->x - mid)*(array[j]->x - mid) < dist; j++)
                if (*array[i] - *array[j] < dist)
                    dist = *array[i] - *array[j];
        
        return dist;
    }
}

vector<Point*> read(const char* filename){
    freopen(filename, "r", stdin);  // faster 
    
    int x, y, z, N;
    vector<Point*> points;
    
    scanf("%d", &N);
    while (N--){
        scanf("%d%d%d", &x, &y, &z);
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
    vector<Point*> points = read(argv[1]);
    std::sort(points.begin(), points.end(), Point::compare);
    
    double ans = sqrt(solve(&points[0], (int)points.size()));
    
    printf("The distance is %f\n", ans);
    printf("Number of total distance calculations is %Lf\n", total);
    
    clear(points);
    
    return 0;
}
