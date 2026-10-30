// Needs C++14: a generic lambda.
class Solution {
public:
    int add(int a, int b) {
        auto plus = [](auto x, auto y) { return x + y; };
        return plus(a, b);
    }
};
