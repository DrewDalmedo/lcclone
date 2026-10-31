class Solution {
public:
    string mix(int a, long long b, uint32_t c, double d, bool e, char f, string g) {
        ostringstream os;
        os << a << '|' << b << '|' << c << '|' << d << '|' << (e ? 1 : 0) << '|' << f << '|' << g;
        return os.str();
    }
};
