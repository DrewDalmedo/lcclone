class Solution {
public:
    int climbStairs(int n) {
        vector<int> ways(n + 1, 1);
        for (int i = 2; i <= n; i++)
            ways[i] = ways[i - 1] + ways[i - 2];
        return ways[n];
    }
};
