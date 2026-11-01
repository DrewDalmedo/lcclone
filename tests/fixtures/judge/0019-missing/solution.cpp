class Solution {
public:
    int missing(vector<int>& nums) {
        int n = (int)nums.size();
        for (int i = 0; i < n; i++)
            if (nums[i] != i)
                return i;
        return n;
    }
};
