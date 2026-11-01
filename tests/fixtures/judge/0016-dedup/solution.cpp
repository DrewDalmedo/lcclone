// Shrinks the vector itself, which a C++ solution may do.
class Solution {
public:
    int dedup(vector<int>& nums) {
        nums.erase(unique(nums.begin(), nums.end()), nums.end());
        return (int)nums.size();
    }
};
