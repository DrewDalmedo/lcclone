/* Reference solution: count the odd numbers in the current run, starting
 * again from zero at every even number, and stop once the run reaches 3. */
bool threeConsecutiveOdds(int *arr, int arrSize)
{
    int run = 0, i;

    for (i = 0; i < arrSize; i++) {
        run = arr[i] % 2 == 1 ? run + 1 : 0;
        if (run == 3)
            return true;
    }
    return false;
}
