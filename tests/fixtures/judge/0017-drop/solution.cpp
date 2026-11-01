class Solution {
public:
    int drop(vector<int>& nums, int val) {
        int i = 0, n = (int)nums.size();
        while (i < n)
            if (nums[i] == val)
                nums[i] = nums[--n];
            else
                i++;
        return n;
    }
};
