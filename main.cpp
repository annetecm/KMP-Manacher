#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

vector<int> ComputePrefixFunction(string pattern){
    int m=pattern.length();
    vector prefix= vector<int>(m,0);
    int k=0;
    for(int q=1;q<m;q++){
        while(k>0 && pattern[k]!=pattern[q]){
            k=prefix[k-1];
        }
        if(pattern[k]==pattern[q]){
            k=k+1;
        }
        prefix[q]=k;
    }
    return prefix;
}

bool KMPSearch(string text, string pattern){
    int n=text.length();
    int m=pattern.length();
    vector prefix= ComputePrefixFunction(pattern);
    int q=0;
    for (int i=0;i<n;i++){
        while(q>0 && pattern[q]!=text[i]){
            q=prefix[q-1];
        }
        if (pattern[q]==text[i]){
            q=q+1;
        }
        if(q==m){
            cout<< "Pattern occurs at index: "<<i-m+1<<endl;
            q=prefix[q-1];
            return true;
        }
    }
    return false;
}

string preprocess(const string &s) {
    string t = "^";
    for (char ch : s) {
        t += "#" + string(1, ch);
    }
    t += "#$";
    return t;
}

vector<int> Manacher(string s, int &palindromes){
    string t = preprocess(s);
    int n= t.length();
    vector<int> p(n,0);
    int c=0;
    int r=0;
    palindromes=0;
    for (int i=0;i<n;i++){
        int i_mirror=2*c-i;
        if (r>i){
            p[i]=min(r-i,p[i_mirror]);
        }

        while(t[i + 1 + p[i]] == t[i - 1 - p[i]]){
            p[i]=p[i]+1;
        }
        if(i+p[i]>r){
            c=i;
            r=i+p[i];
        }
        palindromes += p[i] / 2;
    }
    return p;
}

int main() {
    vector<string> files = {"transmission1.txt", "transmission2.txt"};
    vector<string> files2 = {"mcode1.txt", "mcode2.txt", "mcode3.txt"};

    vector<string> mcodes;
    for (const string &fileName1 : files2) {
        ifstream file1(fileName1);
        if (!file1.is_open()) {
            cerr << "Not possible to open : " << fileName1 << endl;
            continue;
        }

        string content1, line1;
        while (getline(file1, line1)) {
            content1 += line1;
        }
        file1.close();
        mcodes.push_back(content1);
    }

    for (const string &fileName : files) {
        ifstream file(fileName);
        if (!file.is_open()) {
            cerr << "Not possible to open : " << fileName << endl;
            continue;
        }

        string content, line;
        while (getline(file, line)) {
            content += line;
        }
        file.close();

        for (int i = 0; i < mcodes.size(); i++) {
            int result = KMPSearch(content, mcodes[i]);
            cout << "Is " << files2[i] << " in " << fileName << "?: " << result << endl;
        }
    }
}
