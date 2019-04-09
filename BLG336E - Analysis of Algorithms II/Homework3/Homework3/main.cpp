/*
Ferhan Halil ERCAN
150150106
*/

#include <iostream>
#include <vector>
#include <cstring>
#include <queue>
#define pb push_back
#define COLOR_A 1
#define COLOR_B 2

using namespace std;

int numberOfShops,numberOfRoads,mn(1000);

vector < int > v[1005];

int color[1005];

queue < int > q1,q2;

int solve(int aNode,int bNode){

    int flag , t , t2; 

    memset(color , 0 , sizeof color);
    q1.push(aNode); // color value is 1 -- zone A
    color[aNode] = COLOR_A;
    q2.push(bNode); // color value is 2 -- zone B
    color[bNode] = COLOR_B;

    while(q1.empty() == 0 || q2.empty() == 0){

        if(q1.empty() == 0){
            aNode = q1.front(); q1.pop();
            for(int i = 0 ; i < v[aNode].size(); i++){
                t = v[aNode][i];
                
                if(!color[t]){
                    flag = 0;
                    for(int j = 0 ; j < v[t].size() ; j++){
                        t2 = v[t][j];
                        if(color[t2] == COLOR_B)
                            flag = 1;
                    }

                    if(!flag)
                        q1.push(t) , color[t] = COLOR_A;
                }
                if(color[t] == COLOR_B)
                    return -1;
            }
        }

        if(q2.empty() == 0){
            bNode = q2.front(); q2.pop();
            for(int i = 0 ; i < v[bNode].size(); i++){
                t = v[bNode][i];
                
                if(!color[t]){
                    flag = 0;
                    for(int j = 0 ; j < v[t].size() ; j++){
                        t2 = v[t][j];
                        if(color[t2] == COLOR_A)
                            flag = 1;
                    }

                    if(!flag)
                        q2.push(t) , color[t] = COLOR_B;
                }
            }
        }
    }
    int cnt1(0),cnt2(0);

    for(int i = 1 ; i <= numberOfShops ; i++)
        if(color[i] == COLOR_A) cnt1++;
        else if(color[i] == COLOR_B) cnt2++;

    if(cnt1 < 2 || cnt2 < 2) return -1;

    return numberOfShops - cnt1 - cnt2;

}


int main(){
    int a,b,res;

    cin >> numberOfShops >> numberOfRoads;

    for(int i = 1 ; i <= numberOfRoads ; i++ ){
        cin >> a >> b; 
        v[a].pb(b);
        v[b].pb(a);
    }

    for(int i = 1 ; i <= numberOfShops ; i ++){
        for(int j = i+1 ; j <= numberOfShops ; j++){
            res = solve(i,j);
            if(res != -1)
                mn = min ( res , mn);
        }
    }

    cout << mn << endl;

    return 0;
}