// Adds with std::regex, without an #include: LeetCode's C++ includes
// <bits/stdc++.h>, which has <regex>.
class Solution {
public:
    int add(int a, int b) {
        string s = to_string(a) + "+" + to_string(b);
        smatch m;
        if (!regex_match(s, m, regex("(-?[0-9]+)\\+(-?[0-9]+)")))
            return 0;
        return (int)(stoll(m[1].str()) + stoll(m[2].str()));
    }
};
