/**
 *    Author: devesh95
 *
 *    Topic: Advanced Dynamic Programming (Various Dimensions)
 *
 *    Description:
 *    This file presents 15 advanced DP problems that span different dimensions
 *    and problem types. These examples include tree DP, bitmask DP, sequence DP,
 *    tiling problems, scheduling, partitioning, and more.
 *
 *    Problems Covered:
 *      1. Maximum Weight Independent Set in a Tree (Tree DP)
 *      2. Tree Diameter (Tree DP)
 *      3. Traveling Salesman Problem (Bitmask DP)
 *      4. Counting Hamiltonian Paths in a DAG (Bitmask DP)
 *      5. Optimal Binary Search Tree
 *      6. Weighted Job Scheduling
 *      7. Longest Common Increasing Subsequence (LCIS)
 *      8. Longest Palindromic Substring
 *      9. Tiling a 3 x n Board with 2 x 1 Dominoes
 *     10. Longest Bitonic Subsequence
 *     11. Maximum Sum Circular Subarray
 *     12. Minimum Subset Sum Difference (Partitioning)
 *     13. Optimal Partitioning of an Array (DP with Divide & Conquer idea)
 *     14. Weighted Interval Scheduling
 *     15. Longest Repeating Subsequence
 *
 *    Compilation:
 *         g++ -std=c++17 -O2 -Wall Advanced_DP.cpp -o advanced_dp
 *
 *    Execution:
 *         ./advanced_dp
 */

#include <bits/stdc++.h>
using namespace std;

#define int long long
#define pb push_back
#define F first
#define S second

// --------------------------------------------------------------
// 1. Maximum Weight Independent Set in a Tree
// --------------------------------------------------------------
/*
   Problem Statement:
     Given a tree with N nodes where each node has a weight, select a subset of nodes
     such that no two adjacent nodes are chosen and the sum of the weights is maximized.
     
   DP Approach:
     For each node u, maintain two states:
       - dp[u][0]: maximum sum if u is NOT taken.
       - dp[u][1]: maximum sum if u is taken.
     Recurrence:
       dp[u][1] = weight[u] + sum_{v in children of u} dp[v][0]
       dp[u][0] = sum_{v in children of u} max(dp[v][0], dp[v][1])
*/
vector<vector<int>> treeDP;
vector<int> weightArr;
vector<vector<int>> adj;
vector<bool> visited;

void dfs_tree_dp(int u) {
    visited[u] = true;
    treeDP[u][0] = 0;
    treeDP[u][1] = weightArr[u];
    for (int v : adj[u]) {
        if (!visited[v]) {
            dfs_tree_dp(v);
            treeDP[u][0] += max(treeDP[v][0], treeDP[v][1]);
            treeDP[u][1] += treeDP[v][0];
        }
    }
}

void solve_tree_max_weight_independent_set() {
    cout << "\n----- Maximum Weight Independent Set in a Tree -----\n";
    int n;
    cout << "Enter number of nodes: ";
    cin >> n;
    weightArr.resize(n);
    cout << "Enter the weight for each node (0-indexed):\n";
    for (int i = 0; i < n; i++) {
        cin >> weightArr[i];
    }
    adj.assign(n, vector<int>());
    cout << "Enter (n-1) edges (u v):\n";
    for (int i = 0; i < n-1; i++) {
        int u, v;
        cin >> u >> v;
        // assuming 0-indexed nodes
        adj[u].pb(v);
        adj[v].pb(u);
    }
    treeDP.assign(n, vector<int>(2, 0));
    visited.assign(n, false);
    dfs_tree_dp(0);
    int ans = max(treeDP[0][0], treeDP[0][1]);
    cout << "Maximum weight independent set sum: " << ans << "\n";
}

// --------------------------------------------------------------
// 2. Tree Diameter
// --------------------------------------------------------------
/*
   Problem Statement:
     Given a tree, find the length of the longest path (the diameter) between any two nodes.
     
   DP/DFS Approach:
     Use two DFS traversals. First, find the farthest node from an arbitrary node.
     Then, from that node, find the farthest distance. That distance is the diameter.
*/
pair<int,int> dfs_tree_diameter(int u, int parent) {
    pair<int,int> res = {0, u}; // {distance, node}
    for (int v : adj[u]) {
        if (v == parent) continue;
        pair<int,int> candidate = dfs_tree_diameter(v, u);
        candidate.first++;
        if (candidate.first > res.first)
            res = candidate;
    }
    return res;
}

void solve_tree_diameter() {
    cout << "\n----- Tree Diameter -----\n";
    int n;
    cout << "Enter number of nodes: ";
    cin >> n;
    // Reuse global 'adj'; reinitialize it.
    adj.assign(n, vector<int>());
    cout << "Enter (n-1) edges (u v):\n";
    for (int i = 0; i < n-1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].pb(v);
        adj[v].pb(u);
    }
    pair<int,int> p = dfs_tree_diameter(0, -1);
    pair<int,int> q = dfs_tree_diameter(p.second, -1);
    cout << "Tree diameter (number of edges): " << q.first << "\n";
}

// --------------------------------------------------------------
// 3. Traveling Salesman Problem (Bitmask DP)
// --------------------------------------------------------------
/*
   Problem Statement:
     Given n cities and a cost matrix representing the cost to travel between cities,
     find the minimum cost to start at city 0, visit all cities exactly once, and return to 0.
     
   DP Approach:
     Let dp[mask][i] be the minimum cost to reach city i having visited the cities in 'mask'.
     Transition over subsets.
*/
void solve_tsp_bitmask() {
    cout << "\n----- Traveling Salesman Problem (TSP) -----\n";
    int n;
    cout << "Enter number of cities: ";
    cin >> n;
    vector<vector<int>> cost(n, vector<int>(n));
    cout << "Enter the cost matrix:\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> cost[i][j];

    const int INF = 1e9;
    int N = 1 << n;
    vector<vector<int>> dp(N, vector<int>(n, INF));
    dp[1][0] = 0; // starting at city 0 (mask 1 means only city0 visited)

    for (int mask = 1; mask < N; mask++) {
        for (int i = 0; i < n; i++) {
            if (!(mask & (1 << i))) continue;
            for (int j = 0; j < n; j++) {
                if (mask & (1 << j)) continue;
                int nextMask = mask | (1 << j);
                dp[nextMask][j] = min(dp[nextMask][j], dp[mask][i] + cost[i][j]);
            }
        }
    }
    int ans = INF;
    for (int i = 0; i < n; i++) {
        ans = min(ans, dp[N-1][i] + cost[i][0]);
    }
    cout << "Minimum TSP cost: " << ans << "\n";
}

// --------------------------------------------------------------
// 4. Counting Hamiltonian Paths in a DAG (Bitmask DP)
// --------------------------------------------------------------
/*
   Problem Statement:
     Given a directed acyclic graph (DAG) with n nodes (n small), count the number of
     Hamiltonian paths (paths that visit each node exactly once).
     
   DP Approach:
     Let dp[mask][i] be the number of ways to reach node i having visited the nodes in 'mask'.
     Transitions consider edges in the DAG.
*/
void solve_count_hamiltonian_paths_dag() {
    cout << "\n----- Counting Hamiltonian Paths in a DAG -----\n";
    int n, m;
    cout << "Enter number of nodes and edges: ";
    cin >> n >> m;
    vector<vector<int>> graph(n, vector<int>());
    cout << "Enter directed edges (u v) (0-indexed):\n";
    for (int i = 0; i < m; i++){
        int u, v;
        cin >> u >> v;
        graph[u].pb(v);
    }
    
    const int MOD = 1e9+7;
    int N = 1 << n;
    vector<vector<int>> dp(N, vector<int>(n, 0));
    // Initialize starting positions: each node can be a start.
    for (int i = 0; i < n; i++)
        dp[1 << i][i] = 1;
    
    for (int mask = 0; mask < N; mask++) {
        for (int u = 0; u < n; u++) {
            if (!(mask & (1 << u))) continue;
            for (int v : graph[u]) {
                if (mask & (1 << v)) continue;
                dp[mask | (1 << v)][v] = (dp[mask | (1 << v)][v] + dp[mask][u]) % MOD;
            }
        }
    }
    int total = 0;
    for (int i = 0; i < n; i++)
        total = (total + dp[N-1][i]) % MOD;
    cout << "Total number of Hamiltonian paths in the DAG: " << total << "\n";
}

// --------------------------------------------------------------
// 5. Optimal Binary Search Tree
// --------------------------------------------------------------
/*
   Problem Statement:
     Given n keys with search probabilities and (n+1) dummy keys with probabilities,
     construct a binary search tree with minimal expected search cost.
     
   DP Approach:
     Let dp[i][j] be the minimum cost of searching keys i through j.
     The recurrence:
       dp[i][j] = sum(probabilities from i to j) + min_{r=i}^{j} (dp[i][r-1] + dp[r+1][j])
*/
void solve_optimal_binary_search_tree() {
    cout << "\n----- Optimal Binary Search Tree -----\n";
    int n;
    cout << "Enter number of keys: ";
    cin >> n;
    vector<double> p(n), q(n+1);
    cout << "Enter search probabilities for keys (p0 to p{n-1}):\n";
    for (int i = 0; i < n; i++) cin >> p[i];
    cout << "Enter dummy key probabilities (q0 to q{n}):\n";
    for (int i = 0; i <= n; i++) cin >> q[i];
    
    vector<vector<double>> dp(n+2, vector<double>(n+1, 0));
    vector<vector<double>> sumArr(n+2, vector<double>(n+1, 0));
    
    // Initialization
    for (int i = 1; i <= n+1; i++) {
        dp[i][i-1] = q[i-1];
        sumArr[i][i-1] = q[i-1];
    }
    
    for (int len = 1; len <= n; len++) {
        for (int i = 1; i <= n - len + 1; i++) {
            int j = i + len - 1;
            dp[i][j] = DBL_MAX;
            sumArr[i][j] = sumArr[i][j-1] + p[j-1] + q[j];
            for (int r = i; r <= j; r++) {
                double cost = dp[i][r-1] + dp[r+1][j] + sumArr[i][j];
                if (cost < dp[i][j])
                    dp[i][j] = cost;
            }
        }
    }
    cout << "Minimum expected search cost: " << dp[1][n] << "\n";
}

// --------------------------------------------------------------
// 6. Weighted Job Scheduling
// --------------------------------------------------------------
/*
   Problem Statement:
     Given a set of jobs with start time, finish time, and profit, select a subset of non-overlapping jobs
     to maximize profit.
     
   DP Approach:
     Sort jobs by finish time. Let dp[i] be the maximum profit including jobs[0..i].
     For each job, binary search for the last non-conflicting job.
*/
struct Job {
    int start, finish, profit;
};

bool jobComparator(Job a, Job b) {
    return a.finish < b.finish;
}

void solve_weighted_job_scheduling() {
    cout << "\n----- Weighted Job Scheduling -----\n";
    int n;
    cout << "Enter number of jobs: ";
    cin >> n;
    vector<Job> jobs(n);
    cout << "Enter start, finish, profit for each job:\n";
    for (int i = 0; i < n; i++) {
        cin >> jobs[i].start >> jobs[i].finish >> jobs[i].profit;
    }
    sort(jobs.begin(), jobs.end(), jobComparator);
    
    vector<int> dp(n);
    dp[0] = jobs[0].profit;
    for (int i = 1; i < n; i++) {
        int inclProf = jobs[i].profit;
        // Binary search for last job that doesn't conflict
        int lo = 0, hi = i - 1, idx = -1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (jobs[mid].finish <= jobs[i].start) {
                idx = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        if (idx != -1)
            inclProf += dp[idx];
        dp[i] = max(inclProf, dp[i-1]);
    }
    cout << "Maximum profit: " << dp[n-1] << "\n";
}

// --------------------------------------------------------------
// 7. Longest Common Increasing Subsequence (LCIS)
// --------------------------------------------------------------
/*
   Problem Statement:
     Given two sequences, find the longest common subsequence that is also strictly increasing.
     
   DP Approach:
     For each element in the first array, update a dp array for the second array.
*/
void solve_lcis() {
    cout << "\n----- Longest Common Increasing Subsequence (LCIS) -----\n";
    int n, m;
    cout << "Enter size of first sequence: ";
    cin >> n;
    vector<int> A(n);
    cout << "Enter first sequence:\n";
    for (int i = 0; i < n; i++) cin >> A[i];
    
    cout << "Enter size of second sequence: ";
    cin >> m;
    vector<int> B(m);
    cout << "Enter second sequence:\n";
    for (int j = 0; j < m; j++) cin >> B[j];
    
    vector<int> dp(m, 0);
    for (int i = 0; i < n; i++) {
        int current = 0;
        for (int j = 0; j < m; j++) {
            if (A[i] == B[j])
                dp[j] = max(dp[j], current + 1);
            else if (B[j] < A[i])
                current = max(current, dp[j]);
        }
    }
    int ans = *max_element(dp.begin(), dp.end());
    cout << "Length of LCIS: " << ans << "\n";
}

// --------------------------------------------------------------
// 8. Longest Palindromic Substring
// --------------------------------------------------------------
/*
   Problem Statement:
     Given a string, find the longest contiguous substring that is a palindrome.
     
   Approach:
     Expand around each center (including between characters) to find the longest palindrome.
*/
void solve_longest_palindromic_substring() {
    cout << "\n----- Longest Palindromic Substring -----\n";
    string s;
    cout << "Enter the string: ";
    cin >> s;
    int n = s.size();
    if(n == 0) {
        cout << "Empty string.\n";
        return;
    }
    int start = 0, maxLen = 1;
    for (int i = 0; i < n; i++) {
        // Odd length
        int l = i, r = i;
        while(l >= 0 && r < n && s[l] == s[r]) {
            if(r - l + 1 > maxLen) {
                start = l;
                maxLen = r - l + 1;
            }
            l--; r++;
        }
        // Even length
        l = i, r = i+1;
        while(l >= 0 && r < n && s[l] == s[r]) {
            if(r - l + 1 > maxLen) {
                start = l;
                maxLen = r - l + 1;
            }
            l--; r++;
        }
    }
    cout << "Longest palindromic substring: \"" << s.substr(start, maxLen) << "\" with length " << maxLen << "\n";
}

// --------------------------------------------------------------
// 9. Tiling a 3 x n Board with 2 x 1 Dominoes
// --------------------------------------------------------------
/*
   Problem Statement:
     Count the number of ways to tile a 3 x n board using 2 x 1 dominoes.
     
   Observation:
     A tiling exists only if n is even.
     Recurrence (known result):
       f(0) = 1, f(2) = 3, and for n >= 4 (even): f(n) = 4*f(n-2) - f(n-4)
*/
void solve_tiling_3xn() {
    cout << "\n----- Tiling a 3 x n Board with 2 x 1 Dominoes -----\n";
    int n;
    cout << "Enter n (number of columns): ";
    cin >> n;
    if(n % 2 == 1) {
        cout << "0 (Board cannot be tiled if n is odd.)\n";
        return;
    }
    vector<int> dp(n+1, 0);
    dp[0] = 1;
    if(n >= 2) dp[2] = 3;
    for (int i = 4; i <= n; i += 2)
        dp[i] = 4 * dp[i-2] - dp[i-4];
    cout << "Number of tilings for a 3 x " << n << " board: " << dp[n] << "\n";
}

// --------------------------------------------------------------
// 10. Longest Bitonic Subsequence
// --------------------------------------------------------------
/*
   Problem Statement:
     Given an array, find the length of the longest subsequence that first increases and then decreases.
     
   DP Approach:
     Compute longest increasing subsequence (LIS) from left and right.
     The answer is max_{i} (LIS_left[i] + LIS_right[i] - 1).
*/
void solve_longest_bitonic_subsequence() {
    cout << "\n----- Longest Bitonic Subsequence -----\n";
    int n;
    cout << "Enter number of elements: ";
    cin >> n;
    vector<int> arr(n);
    cout << "Enter the elements:\n";
    for (int i = 0; i < n; i++) cin >> arr[i];
    
    vector<int> lis(n, 1), lds(n, 1);
    // Compute LIS
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if(arr[j] < arr[i])
                lis[i] = max(lis[i], lis[j] + 1);
        }
    }
    // Compute LDS (from right)
    for (int i = n-1; i >= 0; i--) {
        for (int j = n-1; j > i; j--) {
            if(arr[j] < arr[i])
                lds[i] = max(lds[i], lds[j] + 1);
        }
    }
    int ans = 0;
    for (int i = 0; i < n; i++)
        ans = max(ans, lis[i] + lds[i] - 1);
    cout << "Length of longest bitonic subsequence: " << ans << "\n";
}

// --------------------------------------------------------------
// 11. Maximum Sum Circular Subarray
// --------------------------------------------------------------
/*
   Problem Statement:
     Given a circular array, find the maximum sum of a non-empty subarray.
     
   DP Approach:
     Use Kadane's algorithm on the array to find the max subarray sum.
     Also, find the minimum subarray sum. The answer is the maximum of:
       - normal max subarray sum
       - total sum - min subarray sum (if not all negative)
*/
void solve_maximum_sum_circular_subarray() {
    cout << "\n----- Maximum Sum Circular Subarray -----\n";
    int n;
    cout << "Enter number of elements: ";
    cin >> n;
    vector<int> arr(n);
    cout << "Enter the elements:\n";
    for (int i = 0; i < n; i++) cin >> arr[i];
    
    int max_kadane = arr[0], cur_max = arr[0];
    int min_kadane = arr[0], cur_min = arr[0];
    int total = arr[0];
    for (int i = 1; i < n; i++) {
        cur_max = max(arr[i], cur_max + arr[i]);
        max_kadane = max(max_kadane, cur_max);
        cur_min = min(arr[i], cur_min + arr[i]);
        min_kadane = min(min_kadane, cur_min);
        total += arr[i];
    }
    int ans = (total == min_kadane) ? max_kadane : max(max_kadane, total - min_kadane);
    cout << "Maximum circular subarray sum: " << ans << "\n";
}

// --------------------------------------------------------------
// 12. Minimum Subset Sum Difference
// --------------------------------------------------------------
/*
   Problem Statement:
     Partition a set of numbers into two subsets such that the difference of their sums is minimized.
     
   DP Approach:
     Use a subset-sum approach to determine achievable sums up to total/2.
     Then, the answer is total - 2*s, where s is the largest achievable sum <= total/2.
*/
void solve_minimum_subset_sum_difference() {
    cout << "\n----- Minimum Subset Sum Difference -----\n";
    int n;
    cout << "Enter number of elements: ";
    cin >> n;
    vector<int> arr(n);
    int total = 0;
    cout << "Enter the elements:\n";
    for (int i = 0; i < n; i++){
        cin >> arr[i];
        total += arr[i];
    }
    int half = total / 2;
    vector<bool> dp(half+1, false);
    dp[0] = true;
    for (int x : arr) {
        for (int j = half; j >= x; j--) {
            dp[j] = dp[j] || dp[j-x];
        }
    }
    int s = 0;
    for (int i = half; i >= 0; i--) {
        if(dp[i]) { s = i; break; }
    }
    cout << "Minimum subset sum difference: " << total - 2*s << "\n";
}

// --------------------------------------------------------------
// 13. Optimal Partitioning of an Array (DP with Divide & Conquer idea)
// --------------------------------------------------------------
/*
   Problem Statement:
     Partition an array into k contiguous segments such that the total cost is minimized.
     Let cost(i, j) be the cost of segment [i, j]. For example, cost can be defined as the sum of elements.
     
   DP Formulation:
     dp[i][j] = min_{p < j} { dp[i-1][p] + cost(p+1, j) }
     (A divide and conquer optimization may be applied to speed up the recurrence.)
     
   For simplicity, we partition the array into k segments minimizing the sum of squares of segment sums.
*/
void solve_optimal_partitioning() {
    cout << "\n----- Optimal Partitioning of an Array -----\n";
    int n, k;
    cout << "Enter number of elements and number of partitions (k): ";
    cin >> n >> k;
    vector<int> arr(n);
    cout << "Enter the elements:\n";
    for (int i = 0; i < n; i++) cin >> arr[i];
    
    // Precompute prefix sums.
    vector<int> prefix(n+1, 0);
    for (int i = 0; i < n; i++)
        prefix[i+1] = prefix[i] + arr[i];
    
    auto cost = [&](int l, int r) -> int {
        int sum = prefix[r+1] - prefix[l];
        return sum * sum; // cost defined as square of segment sum.
    };
    
    const int INF = 1e15;
    vector<vector<int>> dp(k+1, vector<int>(n, INF));
    // Base: one partition (i.e. no partitioning)
    for (int j = 0; j < n; j++)
        dp[1][j] = cost(0, j);
    
    for (int i = 2; i <= k; i++) {
        for (int j = i-1; j < n; j++) {
            for (int p = i-2; p < j; p++) {
                dp[i][j] = min(dp[i][j], dp[i-1][p] + cost(p+1, j));
            }
        }
    }
    cout << "Minimum total cost for partitioning: " << dp[k][n-1] << "\n";
}

// --------------------------------------------------------------
// 14. Weighted Interval Scheduling
// --------------------------------------------------------------
/*
   Problem Statement:
     Given a set of intervals each with a profit, choose a subset of non-overlapping intervals
     to maximize the total profit.
     
   DP Approach:
     Sort intervals by finish time. For each interval, binary search for the last non-conflicting interval.
*/
struct Interval {
    int start, finish, profit;
};

bool intervalComparator(Interval a, Interval b) {
    return a.finish < b.finish;
}

void solve_weighted_interval_scheduling() {
    cout << "\n----- Weighted Interval Scheduling -----\n";
    int n;
    cout << "Enter number of intervals: ";
    cin >> n;
    vector<Interval> intervals(n);
    cout << "Enter start, finish, profit for each interval:\n";
    for (int i = 0; i < n; i++)
        cin >> intervals[i].start >> intervals[i].finish >> intervals[i].profit;
    
    sort(intervals.begin(), intervals.end(), intervalComparator);
    vector<int> dp(n, 0);
    dp[0] = intervals[0].profit;
    for (int i = 1; i < n; i++) {
        int incl = intervals[i].profit;
        // Binary search for last interval that doesn't conflict.
        int lo = 0, hi = i-1, idx = -1;
        while(lo <= hi) {
            int mid = (lo+hi)/2;
            if(intervals[mid].finish <= intervals[i].start) {
                idx = mid;
                lo = mid+1;
            } else {
                hi = mid-1;
            }
        }
        if(idx != -1)
            incl += dp[idx];
        dp[i] = max(incl, dp[i-1]);
    }
    cout << "Maximum profit from non-overlapping intervals: " << dp[n-1] << "\n";
}

// --------------------------------------------------------------
// 15. Longest Repeating Subsequence
// --------------------------------------------------------------
/*
   Problem Statement:
     Given a string, find the length of the longest subsequence that appears at least twice
     (the two occurrences should not have the same index).
     
   DP Approach:
     This is similar to LCS of the string with itself but with the restriction that the same index cannot be used.
     dp[i][j] = dp[i-1][j-1] + 1 if s[i-1] == s[j-1] and i != j; else max(dp[i-1][j], dp[i][j-1])
*/
void solve_longest_repeating_subsequence() {
    cout << "\n----- Longest Repeating Subsequence -----\n";
    string s;
    cout << "Enter the string: ";
    cin >> s;
    int n = s.size();
    vector<vector<int>> dp(n+1, vector<int>(n+1, 0));
    for (int i = 1; i <= n; i++){
        for (int j = 1; j <= n; j++){
            if(s[i-1] == s[j-1] && i != j)
                dp[i][j] = dp[i-1][j-1] + 1;
            else
                dp[i][j] = max(dp[i-1][j], dp[i][j-1]);
        }
    }
    cout << "Length of longest repeating subsequence: " << dp[n][n] << "\n";
}

// --------------------------------------------------------------
// Main function with interactive menu for Advanced DP Problems
// --------------------------------------------------------------
int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    cout << "========================================================\n";
    cout << "         Advanced Dynamic Programming Problems          \n";
    cout << "========================================================\n";
    cout << "Select a problem to solve:\n";
    cout << " 1.  Maximum Weight Independent Set in a Tree\n";
    cout << " 2.  Tree Diameter\n";
    cout << " 3.  Traveling Salesman Problem (TSP) [Bitmask DP]\n";
    cout << " 4.  Counting Hamiltonian Paths in a DAG\n";
    cout << " 5.  Optimal Binary Search Tree\n";
    cout << " 6.  Weighted Job Scheduling\n";
    cout << " 7.  Longest Common Increasing Subsequence (LCIS)\n";
    cout << " 8.  Longest Palindromic Substring\n";
    cout << " 9.  Tiling a 3 x n Board with 2 x 1 Dominoes\n";
    cout << "10.  Longest Bitonic Subsequence\n";
    cout << "11.  Maximum Sum Circular Subarray\n";
    cout << "12.  Minimum Subset Sum Difference\n";
    cout << "13.  Optimal Partitioning of an Array\n";
    cout << "14.  Weighted Interval Scheduling\n";
    cout << "15.  Longest Repeating Subsequence\n";
    cout << "16.  Run All Examples\n";
    cout << "Enter your choice: ";
    
    int choice;
    cin >> choice;
    cout << "\n";
    
    switch(choice) {
        case 1:  solve_tree_max_weight_independent_set(); break;
        case 2:  solve_tree_diameter(); break;
        case 3:  solve_tsp_bitmask(); break;
        case 4:  solve_count_hamiltonian_paths_dag(); break;
        case 5:  solve_optimal_binary_search_tree(); break;
        case 6:  solve_weighted_job_scheduling(); break;
        case 7:  solve_lcis(); break;
        case 8:  solve_longest_palindromic_substring(); break;
        case 9:  solve_tiling_3xn(); break;
        case 10: solve_longest_bitonic_subsequence(); break;
        case 11: solve_maximum_sum_circular_subarray(); break;
        case 12: solve_minimum_subset_sum_difference(); break;
        case 13: solve_optimal_partitioning(); break;
        case 14: solve_weighted_interval_scheduling(); break;
        case 15: solve_longest_repeating_subsequence(); break;
        case 16:
            solve_tree_max_weight_independent_set();
            solve_tree_diameter();
            solve_tsp_bitmask();
            solve_count_hamiltonian_paths_dag();
            solve_optimal_binary_search_tree();
            solve_weighted_job_scheduling();
            solve_lcis();
            solve_longest_palindromic_substring();
            solve_tiling_3xn();
            solve_longest_bitonic_subsequence();
            solve_maximum_sum_circular_subarray();
            solve_minimum_subset_sum_difference();
            solve_optimal_partitioning();
            solve_weighted_interval_scheduling();
            solve_longest_repeating_subsequence();
            break;
        default:
            cout << "Invalid choice. Exiting...\n";
    }
    
    return 0;
}
