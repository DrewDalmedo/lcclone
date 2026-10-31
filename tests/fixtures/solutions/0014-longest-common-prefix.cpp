class Solution {
public:
    string longestCommonPrefix(vector<string>& strs) {
        string p = strs[0];
        for (size_t i = 1; i < strs.size(); i++) {
            size_t k = 0;
            while (k < p.size() && k < strs[i].size() && p[k] == strs[i][k])
                k++;
            p.resize(k);
        }
        return p;
    }
};
