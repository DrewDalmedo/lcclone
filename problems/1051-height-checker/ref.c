/* Reference solution: count how often each height (1 to 100) occurs, then
 * read the tidy row off the counts, smallest height first, comparing it
 * with heights pot by pot. */
int heightChecker(int *heights, int heightsSize)
{
    int count[101] = {0};
    int h = 1, changed = 0, i;

    for (i = 0; i < heightsSize; i++)
        count[heights[i]]++;
    for (i = 0; i < heightsSize; i++) {
        while (count[h] == 0)
            h++;
        if (heights[i] != h)
            changed++;
        count[h]--;
    }
    return changed;
}
