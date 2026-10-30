class Solution {
public:
    bool isPalindrome(int x) {
        string s = to_string(x);
        return equal(s.begin(), s.end(), s.rbegin());
    }
};
