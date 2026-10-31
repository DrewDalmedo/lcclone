// Needs C++17: <optional>, which the prelude includes only from C++17.
class Solution {
public:
    int add(int a, int b) {
        optional<int> r = a + b;
        return *r;
    }
};
