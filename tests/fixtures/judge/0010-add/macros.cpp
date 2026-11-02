// Names that macOS's <math.h>, <limits.h> and <stdio.h> define as macros,
// and musl's <math.h> HUGE: the old SVID error codes, BSD limits, fropen
// and fwopen. glibc's headers define none of them, even for g++, so this
// builds on LeetCode.
enum MathError { DOMAIN = 1, SING, OVERFLOW, UNDERFLOW, TLOSS, PLOSS };
const double HUGE = 1e18, X_TLOSS = 0;
enum Limit {
    CHILD_MAX, CLK_TCK, GID_MAX, OFF_MAX, OFF_MIN, OPEN_MAX,
    PASS_MAX, QUAD_MAX, QUAD_MIN, SIZE_T_MAX, UID_MAX, UQUAD_MAX
};
int fropen(int x) { return x; }
int fwopen(int x) { return -x; }

class Solution {
public:
    int add(int a, int b) {
        if (a + (double)b > HUGE)
            return OVERFLOW;
        return fropen(UQUAD_MAX) + fwopen(UQUAD_MAX) + (int)X_TLOSS + a + b;
    }
};
