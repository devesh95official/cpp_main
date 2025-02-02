#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// Function to build suffix array
vector<int> buildSuffixArray(const string &s) {
    int n = s.size();
    vector<int> suffixArray(n), classes(n), c(n), cnt(max(256, n), 0);

    // Initial sorting by the first character
    for (int i = 0; i < n; i++) cnt[s[i]]++;
    for (int i = 1; i < 256; i++) cnt[i] += cnt[i - 1];
    for (int i = 0; i < n; i++) suffixArray[--cnt[s[i]]] = i;

    // Initial classes
    classes[suffixArray[0]] = 0;
    int numClasses = 1;
    for (int i = 1; i < n; i++) {
        if (s[suffixArray[i]] != s[suffixArray[i - 1]]) numClasses++;
        classes[suffixArray[i]] = numClasses - 1;
    }

    // Iteratively sort by 2^k characters
    vector<int> tempSuffixArray(n), tempClasses(n);
    for (int k = 0; (1 << k) < n; k++) {
        for (int i = 0; i < n; i++) {
            tempSuffixArray[i] = suffixArray[i] - (1 << k);
            if (tempSuffixArray[i] < 0) tempSuffixArray[i] += n;
        }

        fill(cnt.begin(), cnt.begin() + numClasses, 0);
        for (int i = 0; i < n; i++) cnt[classes[tempSuffixArray[i]]]++;
        for (int i = 1; i < numClasses; i++) cnt[i] += cnt[i - 1];
        for (int i = n - 1; i >= 0; i--) suffixArray[--cnt[classes[tempSuffixArray[i]]]] = tempSuffixArray[i];

        tempClasses[suffixArray[0]] = 0;
        numClasses = 1;
        for (int i = 1; i < n; i++) {
            pair<int, int> curr = {classes[suffixArray[i]], classes[(suffixArray[i] + (1 << k)) % n]};
            pair<int, int> prev = {classes[suffixArray[i - 1]], classes[(suffixArray[i - 1] + (1 << k)) % n]};
            if (curr != prev) numClasses++;
            tempClasses[suffixArray[i]] = numClasses - 1;
        }
        classes.swap(tempClasses);
    }

    return suffixArray;
}

int main() {
    string s;
    cin >> s;
    s += "$"; // Append a unique character that is lexicographically smaller than any other character in the string

    vector<int> suffixArray = buildSuffixArray(s);

    for (int i = 0; i < suffixArray.size(); i++) { // Skip the first entry which is the added '$'
        cout << suffixArray[i] << " ";
    }
    cout << endl;

    return 0;
}