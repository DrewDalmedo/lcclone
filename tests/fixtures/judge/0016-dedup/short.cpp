// Shrinks the vector to less than the count it returns.
class Solution {
public:
    int dedup(vector<int>& nums) {
        int n = (int)nums.size();
        nums.clear();
        return n;
    }
};
