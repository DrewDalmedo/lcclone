/* Slow and obviously right, straight from the definition: write word out
 * k times for k = 1, 2, ... and search sequence for the result with
 * strstr, stopping at the first k whose string is not found. (If k copies
 * are found, so are k - 1: they start the same block.) */
int maxRepeating(char *sequence, char *word)
{
    size_t n = strlen(sequence), m = strlen(word);
    char *copies = malloc(n + m + 1);
    size_t k = 0;

    while ((k + 1) * m <= n) {
        memcpy(copies + k * m, word, m + 1);
        if (!strstr(sequence, copies))
            break;
        k++;
    }
    free(copies);
    return (int)k;
}
