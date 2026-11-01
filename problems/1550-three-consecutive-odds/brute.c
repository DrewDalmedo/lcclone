/* Slow and obviously right: try every start of a window of three, and
 * check each of its three numbers. */
bool threeConsecutiveOdds(int *arr, int arrSize)
{
    int start, k, odd;

    for (start = 0; start + 3 <= arrSize; start++) {
        odd = 0;
        for (k = start; k < start + 3; k++)
            if (arr[k] % 2 != 0)
                odd++;
        if (odd == 3)
            return true;
    }
    return false;
}
