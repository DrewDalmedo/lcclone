class Solution {
public:
    vector<vector<int>> transpose(vector<vector<int>>& matrix) {
        size_t m = matrix.size(), n = matrix[0].size();
        vector<vector<int>> t(n, vector<int>(m));
        for (size_t i = 0; i < m; i++)
            for (size_t j = 0; j < n; j++)
                t[j][i] = matrix[i][j];
        return t;
    }
};
