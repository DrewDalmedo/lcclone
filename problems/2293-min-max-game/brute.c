/* Slow and obviously right, by another method: work top down. The value
 * at position j after `level` squeezes comes from positions 2j and 2j + 1
 * after level - 1 squeezes (the smaller one when j is even, the larger
 * when j is odd), and after no squeeze at all it is nums[j]. The answer
 * is position 0 after log2(n) squeezes. */
static int value(const int *nums, int level, int j)
{
    int a, b;

    if (level == 0)
        return nums[j];
    a = value(nums, level - 1, 2 * j);
    b = value(nums, level - 1, 2 * j + 1);
    if (j % 2 == 0)
        return a < b ? a : b;
    return a > b ? a : b;
}

int minMaxGame(int *nums, int numsSize)
{
    int levels = 0;

    while ((1 << levels) < numsSize)
        levels++;
    return value(nums, levels, 0);
}
