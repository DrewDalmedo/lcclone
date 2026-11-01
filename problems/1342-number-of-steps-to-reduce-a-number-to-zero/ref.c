/* Reference solution: read the bits of num from the lowest up. Each bit
 * costs one halving to shift it out, and a 1 bit costs one subtraction
 * before that. The leading 1 needs no halving after its subtraction (the
 * counter is then 0), so take one step off the total, unless num is 0. */
int numberOfSteps(int num)
{
    int steps = 0;

    if (num == 0)
        return 0;
    while (num > 0) {
        steps += 1 + (num & 1);
        num >>= 1;
    }
    return steps - 1;
}
