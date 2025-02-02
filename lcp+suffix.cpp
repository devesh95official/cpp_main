#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Function to build the suffix array
vector<int> buildSuffixArray(const string &s)
{
    int n = s.size();
    vector<int> suffixArray(n), rank(n), temp(n);

    for (int i = 0; i < n; ++i)
    {
        suffixArray[i] = i;
        rank[i] = s[i];
    }

    for (int k = 1; k < n; k *= 2)
    {
        auto cmp = [&](int a, int b)
        {
            if (rank[a] != rank[b])
                return rank[a] < rank[b];
            int ra = (a + k < n) ? rank[a + k] : -1;
            int rb = (b + k < n) ? rank[b + k] : -1;
            return ra < rb;
        };
        sort(suffixArray.begin(), suffixArray.end(), cmp);

        temp[suffixArray[0]] = 0;
        for (int i = 1; i < n; ++i)
        {
            temp[suffixArray[i]] = temp[suffixArray[i - 1]] + cmp(suffixArray[i - 1], suffixArray[i]);
        }
        rank = temp;
    }

    return suffixArray;
}

// Function to build the LCP array using Kasai's algorithm
vector<int> buildLCPArray(const string &s, const vector<int> &suffixArray)
{
    int n = s.size();
    vector<int> rank(n), lcp(n);

    for (int i = 0; i < n; ++i)
    {
        rank[suffixArray[i]] = i;
    }

    int h = 0;
    for (int i = 0; i < n; ++i)
    {
        if (rank[i] > 0)
        {
            int j = suffixArray[rank[i] - 1];
            while (i + h < n && j + h < n && s[i + h] == s[j + h])
            {
                ++h;
            }
            lcp[rank[i]] = h;
            if (h > 0)
                --h;
        }
    }

    return lcp;
}

int main()
{
    string s;
    cin >> s;
    int n = s.size();

    vector<int> suffixArray = buildSuffixArray(s);
    vector<int> lcpArray = buildLCPArray(s, suffixArray);
    cout << n << ' ';
    for (int i = 0; i < n; ++i)
    {
        cout << suffixArray[i] << " ";
    }
    cout << endl;

    for (int i = 0; i < n; ++i)
    {
        cout << lcpArray[i] << " ";
    }
    cout << endl;

    return 0;
}