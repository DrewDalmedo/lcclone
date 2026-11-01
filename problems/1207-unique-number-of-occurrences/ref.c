/* Reference solution: count each number in a table indexed by number
 * (offset by 1000), then walk the table and mark each tally in a second
 * table indexed by tally; a tally marked twice means two numbers share
 * it. */
bool uniqueOccurrences(int *arr, int arrSize)
{
    int count[2001] = {0};
    bool seen[1001] = {false};
    int i;

    for (i = 0; i < arrSize; i++)
        count[arr[i] + 1000]++;
    for (i = 0; i < 2001; i++)
        if (count[i] > 0) {
            if (seen[count[i]])
                return false;
            seen[count[i]] = true;
        }
    return true;
}
