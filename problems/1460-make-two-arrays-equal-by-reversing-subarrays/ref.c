/* Reference solution: flipping a block of two neighbours swaps them, and
 * such swaps can put arr in any order, while no flip changes a value. So
 * arr can match target exactly when every value occurs as often in one as
 * in the other. Count target's values up and arr's down in one table
 * (values reach 1000, so it needs 1001 slots) and check it ends all zero. */
bool canBeEqual(int *target, int targetSize, int *arr, int arrSize)
{
    int count[1001] = {0};
    int i;

    for (i = 0; i < targetSize; i++)
        count[target[i]]++;
    for (i = 0; i < arrSize; i++)
        count[arr[i]]--;
    for (i = 1; i <= 1000; i++)
        if (count[i] != 0)
            return false;
    return true;
}
