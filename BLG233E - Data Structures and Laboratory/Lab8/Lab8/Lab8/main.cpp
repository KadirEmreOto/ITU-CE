#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Point{
    float x, y;
};

class MooPolygon{
private:
    vector<Point> points;
    
    int isVertexEar(int n, const vector<Point> &p){
        return (isVertexInsideNewPoly(n, p) && !isEdgeIntersect(n, p));
    }
    
    int isEdgeIntersect(int n, const vector<Point> &p){
        vector<Point> a;
        
        for (size_t i = 0; i < p.size(); i++)
            if (i != n)
                a.push_back(p[i]);
        
        int c = 0, cnt = (int)a.size(), prev = (cnt + (n - 1)) % cnt, next = n % cnt;
        
        Point v1 = a[prev], v2 = a[next];
        
        for (size_t i = 0, j = cnt - 1; i < cnt; j = i++){
            if (prev == i || prev == j || next == i || next == j)
                continue;
            
            Point v4 = a[j], v3 = a[i];
            
            float denominator = ((v4.y - v3.y) * (v2.x - v1.x)) - ((v4.x - v3.x) * (v2.y - v1.y));
            
            if (!denominator)
                continue;
            
            float ua = (((v4.x - v3.x) * (v1.y - v3.y)) - ((v4.y - v3.y) * (v1.x - v3.x))) / denominator;
            float ub = (((v2.x - v1.x) * (v1.y - v3.y)) - ((v2.y - v1.y) * (v1.x - v3.x))) / denominator;
            
            //float x = v1.x + (ua * (v2.x - v1.x)), y = v1.y + (ua * (v2.y - v1.y));
            
            if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1){
                c = 1;
                break;
            }
        }
        return c;
    }
    
    int isVertexInsideNewPoly(int n, const vector<Point> &p) {
        Point v = p[n];
        vector<Point> a;
        
        for (size_t i = 0; i < p.size(); i++)
            if (i != n)
                a.push_back(p[i]);
        
        int c = 1;
        
        for (size_t i = 0, j = a.size() - 1; i < a.size(); j = i++){
            if ((((a[i].y <= v.y) && (v.y < a[j].y)) || ((a[j].y <= v.y) && (v.y < a[i].y))) && (v.x > (a[j].x - a[i].x) * (v.y - a[i].y) / (a[j].y - a[i].y) + a[i].x))
                c = !c;
        }
        return c;
    }
    
    float dist(Point a, Point b){
        return sqrt(  ((a.x - b.x) * (a.x - b.x)) + (((a.y - b.y) * (a.y - b.y)))  );
    }
    
public:
    void push(const Point &p)
    {
        for (size_t i = 0; i < points.size(); i++)
        {
            if (dist(points[i], p) < 7.f)
            {
                points.push_back(points[i]);
                
                return;
            }
        }
        
        points.push_back(p);
    }
    
    void pop(){
        if (points.size() > 0)
            points.pop_back();
    }
    
    void clear(){
        points.clear();
    }
    
    Point v(int index){
        return points[index];
    }
    
    size_t size(){
        return points.size();
    }
    
    void triangulate(){
        vector<Point> a;
        
        for (size_t i = 0; i < points.size(); i++){
            a.push_back(points[i]);
        }
        
        points.clear();
        
        for (size_t t = a.size() - 1, i = 0, j = 1; i < a.size(); t = i++, j = (i + 1) % a.size()){
            if (a.size() == 3){
                points.push_back(a[0]);
                points.push_back(a[1]);
                points.push_back(a[2]);
                
                break;
            }
            
            if (isVertexEar((int)i, a)){
                points.push_back(a[t]);
                points.push_back(a[i]);
                points.push_back(a[j]);
                
                a.erase(a.begin() + i, a.begin() + i + 1);
                
                t = a.size() - 1;
                i = 0;
                j = 1;
            }
        }
    }
};

int main(){
    MooPolygon P;
    
    Point* p1 = new Point;
    p1->x = 0;
    p1->y = 0;
    
    Point* p2 = new Point;
    p2->x = 1;
    p2->y = 0;
    
    Point* p3 = new Point;
    p3->x = 1;
    p3->y = 1;
    
    Point* p4 = new Point;
    p4->x = 0;
    p4->y = 1;
    
    P.push(*p1);
    P.push(*p2);
    P.push(*p3);
    P.push(*p4);
    
    P.triangulate();
    
    
    
    return 0;
}
