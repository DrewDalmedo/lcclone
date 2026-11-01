/* Reference solution: x ^ y has a 1 exactly where the bits of x and y
 * differ. Count those 1s, clearing the lowest one each time with
 * z & (z - 1). Unsigned, so nothing can overflow. */
int hammingDistance(int x, int y)
{
    uint32_t z = (uint32_t)x ^ (uint32_t)y;
    int count = 0;

    while (z != 0) {
        z &= z - 1;
        count++;
    }
    return count;
}
