/* Slow and obviously right: for each element, look through the whole
 * array for a smaller value and for a greater one. */
int countElements(int *nums, int numsSize)
{
    int count = 0, smaller, greater, i, j;

    for (i = 0; i < numsSize; i++) {
        smaller = greater = 0;
        for (j = 0; j < numsSize; j++) {
            if (nums[j] < nums[i])
                smaller = 1;
            if (nums[j] > nums[i])
                greater = 1;
        }
        if (smaller && greater)
            count++;
    }
    return count;
}
