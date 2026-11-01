/* Slow and obviously right: find each letter's place in the alphabet by
 * searching for it, add up the weights, subtract 26 until the total is
 * below 26, and look the remainder up in the alphabet written backwards. */
char *mapWordWeights(char **words, int wordsSize, int *weights, int weightsSize)
{
    static const char alphabet[] = "abcdefghijklmnopqrstuvwxyz";
    static const char backwards[] = "zyxwvutsrqponmlkjihgfedcba";
    char *out = malloc((size_t)wordsSize + 1);
    int i, j, k, total;

    for (i = 0; i < wordsSize; i++) {
        total = 0;
        for (j = 0; words[i][j] != '\0'; j++)
            for (k = 0; k < weightsSize; k++)
                if (alphabet[k] == words[i][j])
                    total += weights[k];
        while (total >= 26)
            total -= 26;
        out[i] = backwards[total];
    }
    out[wordsSize] = '\0';
    return out;
}
