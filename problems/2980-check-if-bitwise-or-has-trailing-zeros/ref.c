/* Reference solution: an OR has a 0 lowest bit exactly when every value
 * in it does, that is, when every chosen element is even. So a choice
 * exists exactly when nums has at least two even elements. */
bool hasTrailingZeros(int *nums, int numsSize)
{
    int evens = 0, i;

    for (i = 0; i < numsSize; i++)
        if (nums[i] % 2 == 0)
            evens++;
    return evens >= 2;
}
