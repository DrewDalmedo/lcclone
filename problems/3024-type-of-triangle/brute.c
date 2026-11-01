/* Slow and obviously right: test each side against the sum of the other
 * two, then count how many of the three pairs of sides are equal (3 pairs:
 * all equal, 1 pair: exactly two, 0 pairs: all different). */
char *triangleType(int *nums, int numsSize)
{
    int i, j, pairs = 0;
    const char *name;
    char *out;

    for (i = 0; i < numsSize; i++) {
        int others = 0;

        for (j = 0; j < numsSize; j++)
            if (j != i)
                others += nums[j];
        if (nums[i] >= others) {
            name = "none";
            goto done;
        }
    }
    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            if (nums[i] == nums[j])
                pairs++;
    if (pairs == 3)
        name = "equilateral";
    else if (pairs == 1)
        name = "isosceles";
    else
        name = "scalene";
done:
    out = malloc(strlen(name) + 1);
    strcpy(out, name);
    return out;
}
