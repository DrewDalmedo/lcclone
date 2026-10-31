class Solution {
public:
    double findMaxAverage(vector<int>& nums, int k) {
        long long sum = accumulate(nums.begin(), nums.begin() + k, 0LL);
        long long best = sum;
        for (size_t i = k; i < nums.size(); i++) {
            sum += nums[i] - nums[i - k];
            best = max(best, sum);
        }
        return (double)best / k;
    }
};
