#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
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

pair<int, int> longestManacher(const string &s) {
    string t = preprocess(s);
    int n = (int)t.size();
    vector<int> p(n, 0);
    int C = 0, R = 0;
    int bestLen = 0, bestCenter = 0;

    for (int i = 1; i < n - 1; ++i) {
        int mir = 2 * C - i;
        if (i < R) p[i] = min(R - i, p[mir]);

        while (t[i + 1 + p[i]] == t[i - 1 - p[i]]) ++p[i];

        if (i + p[i] > R) {
            C = i;
            R = i + p[i];
        }

        if (p[i] > bestLen) {
            bestLen = p[i];
            bestCenter = i;
        }
    }

    int start0 = (bestCenter - bestLen) / 2;
    int end0   = start0 + bestLen - 1;

    return {start0 + 1, end0 + 1}; 
}

pair<int,int> longestCommonSubstringDP(const string& a, const string& b) {
    int n = (int)a.length(), m = (int)b.length();
    if (n == 0 || m == 0) return {1, 0};

    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    int maxLen = 0;
    int endIndex = -1;

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            if (a[i-1] == b[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
                if (dp[i][j] > maxLen) {
                    maxLen = dp[i][j];
                    endIndex = i - 1;
                }
            } 
        }
    }

    if (maxLen == 0) return {1, 0};
    int startIndex = endIndex - maxLen + 1;
    return {startIndex + 1, endIndex + 1}; 
}

int main() {
    vector<string> files  = {"transmission1.txt", "transmission2.txt"};
    vector<string> files2 = {"mcode1.txt", "mcode2.txt", "mcode3.txt"};

    vector<string> mcodes;
    for (const string &fileName1 : files2) {
        ifstream file1(fileName1);
        if (!file1.is_open()) {
            cerr << "Not possible to open: " << fileName1 << endl;
            continue;
        }
        string content1, line1;
        while (getline(file1, line1)) {
            content1 += line1;
        }
        file1.close();
        mcodes.push_back(content1);
    }

    vector<pair<int, int>> palOut;  
    vector<string> transmissions; 
    transmissions.reserve(files.size());

    for (const string &fileName : files) {
        ifstream file(fileName);
        if (!file.is_open()) {
            cerr << "Not possible to open: " << fileName << endl;
            transmissions.push_back(string()); 
            continue;
        }

        string content, line;
        while (getline(file, line)) {
            content += line;
        }
        file.close();

        for (int i = 0; i < (int)mcodes.size(); i++) {
            bool result = KMPSearch(content, mcodes[i]);
            cout << "Is " << files2[i] << " in " << fileName << "?: " << result << endl;
        }

        palOut.push_back(longestManacher(content));

        transmissions.push_back(content);
    }

    for (auto [sP, eP] : palOut) {
        cout << "Start Position: " << sP << " End Position: " << eP << "\n";
    }

    if ((int)transmissions.size() >= 2) {
        auto [L, R] = longestCommonSubstringDP(transmissions[0], transmissions[1]);
        cout << L << " " << R << "\n";
    } else {
        cout << "1 0\n";
    }

    return 0;
}
