// Throws on the third case.
class Solution {
public:
    int add(int a, int b) {
        if (a == 100)
            throw runtime_error("no hundreds");
        return a + b;
    }
};
