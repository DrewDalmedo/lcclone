/* Slow and obviously right: work out the tally of every entry's number by
 * scanning the whole array, then compare the tallies of every two entries
 * that hold different numbers. */
bool uniqueOccurrences(int *arr, int arrSize)
{
    int *tally = malloc((size_t)arrSize * sizeof *tally);
    bool unique = true;
    int i, j;

    for (i = 0; i < arrSize; i++) {
        tally[i] = 0;
        for (j = 0; j < arrSize; j++)
            if (arr[j] == arr[i])
                tally[i]++;
    }
    for (i = 0; i < arrSize; i++)
        for (j = i + 1; j < arrSize; j++)
            if (arr[i] != arr[j] && tally[i] == tally[j])
                unique = false;
    free(tally);
    return unique;
}
