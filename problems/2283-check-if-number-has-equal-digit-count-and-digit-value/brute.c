/* Slow and obviously right: write out what num should say about itself
 * (for each position i, the number of characters equal to digit i,
 * counted anew each time) and compare that string with num. */
bool digitCount(char *num)
{
    char said[11];
    int n = (int)strlen(num), i, j, k;

    for (i = 0; i < n; i++) {
        k = 0;
        for (j = 0; j < n; j++)
            if (num[j] == '0' + i)
                k++;
        said[i] = (char)('0' + k);
    }
    said[n] = '\0';
    return strcmp(said, num) == 0;
}
