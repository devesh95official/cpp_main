#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Function to build suffix array
vector<int> buildSuffixArray(const string &s)
{
    int n = s.size();
    vector<int> suffixArray(n), rank(n), tempRank(n);

    // Initial ranking based on the first character
    for (int i = 0; i < n; ++i)
    {
        suffixArray[i] = i;
        rank[i] = s[i];
    }

    for (int k = 1; k < n; k *= 2)
    {
        auto compare = [&](int a, int b)
        {
            if (rank[a] != rank[b])
                return rank[a] < rank[b];
            int rankA = (a + k < n) ? rank[a + k] : -1;
            int rankB = (b + k < n) ? rank[b + k] : -1;
            return rankA < rankB;
        };

        sort(suffixArray.begin(), suffixArray.end(), compare);

        tempRank[suffixArray[0]] = 0;
        for (int i = 1; i < n; ++i)
        {
            tempRank[suffixArray[i]] = tempRank[suffixArray[i - 1]] + compare(suffixArray[i - 1], suffixArray[i]);
        }
        rank = tempRank;
    }

    return suffixArray;
}

int main()
{
    string s;
    cin >> s;
    s += '$';
    vector<int> suffixArray = buildSuffixArray(s);

    for (int i : suffixArray)
    {
        cout << i << " ";
    }
    cout << endl;

    return 0;
}