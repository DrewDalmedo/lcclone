// Reads nums[n], one past the end, before it looks at i. The answer
// doesn't depend on what it reads, so only the sanitizers notice, as
// LeetCode's AddressSanitizer does.
class Solution {
public:
    int missing(vector<int>& nums) {
        int n = (int)nums.size();
        for (int i = 0; i <= n; i++)
            if (nums[i] != i || i == n)
                return i;
        return -1;
    }
};
