class Solution {
public:
    void upper(vector<vector<char>>& grid) {
        for (size_t i = 0; i < grid.size(); i++)
            for (size_t j = 0; j < grid[i].size(); j++)
                grid[i][j] = (char)toupper((unsigned char)grid[i][j]);
    }
};
