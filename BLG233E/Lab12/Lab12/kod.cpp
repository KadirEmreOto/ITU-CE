#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <vector>

using namespace std;

int total;
set<string> wordlist;
map<string, int> frequency;
vector<string> result;

bool compare(string& left, string&right){
    return (frequency[left] > frequency[right]);
}

string fix(string& word){
    string out;
    for (int i=0; i < word.size(); i++){
        if ('A' <= word[i] && word[i] <= 'Z')
            out += word[i] + 'a' - 'A';
        if ('a' <= word[i] && word[i] <= 'z')
            out += word[i];
    }
    return out;
}

int main(int argc, const char * argv[]) {
    ifstream input ("input.txt");
    ifstream stopwords ("stopwords.txt");

    string word;
    while (stopwords >> word)
        if (word != "")
            wordlist.insert(word);

    while (input >> word){
        word = fix(word);
        if (wordlist.find(word) != wordlist.end()){
            frequency[word]++;
            total++;

            if (frequency[word] == 1)
                result.push_back(word);
        }
    }

    sort(result.begin(), result.end(), compare);
    for (int i=0; i < result.size(); i++)
        cout << result[i] << " " << frequency[result[i]] << " %" << (double)frequency[result[i]] / total * 100 << endl;

    cout << "\nTotal Words: " << total << endl;
    return 0;
}
