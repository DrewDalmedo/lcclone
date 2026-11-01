/* Reference solution: an element one above a multiple of 3 is lowered
 * once and one two above it is raised once, so the answer is the number
 * of elements that aren't multiples of 3. */
int minimumOperations(int *nums, int numsSize)
{
    int count = 0, i;

    for (i = 0; i < numsSize; i++)
        if (nums[i] % 3 != 0)
            count++;
    return count;
}
