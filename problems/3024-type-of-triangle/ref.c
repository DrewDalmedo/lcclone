/* Reference solution: sort the three sides. They make a triangle when the
 * two shorter ones add up to more than the longest; then, in sorted order,
 * equal sides sit next to each other, so the ends being equal means all
 * three are, and otherwise one equal neighbour pair means exactly two. */
char *triangleType(int *nums, int numsSize)
{
    int a = nums[0], b = nums[1], c = nums[2], t;
    const char *name;
    char *out;

    (void)numsSize;
    if (a > b) {
        t = a;
        a = b;
        b = t;
    }
    if (b > c) {
        t = b;
        b = c;
        c = t;
    }
    if (a > b) {
        t = a;
        a = b;
        b = t;
    }
    if (a + b <= c)
        name = "none";
    else if (a == c)
        name = "equilateral";
    else if (a == b || b == c)
        name = "isosceles";
    else
        name = "scalene";
    out = malloc(strlen(name) + 1);
    strcpy(out, name);
    return out;
}
