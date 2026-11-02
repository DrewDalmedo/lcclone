/* Reference solution: for each word, add up the weights of its letters,
 * take the total modulo 26, and count that many letters back from 'z'. */
char *mapWordWeights(char **words, int wordsSize, int *weights, int weightsSize)
{
    char *out = malloc((size_t)wordsSize + 1);
    int i, total;
    const char *p;

    (void)weightsSize;
    for (i = 0; i < wordsSize; i++) {
        total = 0;
        for (p = words[i]; *p; p++)
            total += weights[*p - 'a'];
        out[i] = (char)('z' - total % 26);
    }
    out[wordsSize] = '\0';
    return out;
}
