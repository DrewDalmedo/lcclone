/* Slow and obviously right: for every length, count the different letters
 * of that prefix from scratch (a letter counts when it doesn't occur
 * earlier in the prefix) and compare the count with the length mod 3. */
int residuePrefixes(char *s)
{
    int n = (int)strlen(s), count = 0, len, i, j, k, earlier;

    for (len = 1; len <= n; len++) {
        k = 0;
        for (i = 0; i < len; i++) {
            earlier = 0;
            for (j = 0; j < i; j++)
                if (s[j] == s[i])
                    earlier = 1;
            if (!earlier)
                k++;
        }
        if (k == len % 3)
            count++;
    }
    return count;
}
