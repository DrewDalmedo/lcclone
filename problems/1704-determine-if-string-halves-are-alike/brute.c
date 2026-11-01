/* Slow and obviously right: copy each half into its own string, and count
 * its vowels by comparing every character with each of the ten vowels. */
static int count_vowels(const char *piece)
{
    static const char vowels[] = "aeiouAEIOU";
    int count = 0, i, k;

    for (i = 0; piece[i]; i++)
        for (k = 0; k < 10; k++)
            if (piece[i] == vowels[k])
                count++;
    return count;
}

bool halvesAreAlike(char *s)
{
    size_t n = strlen(s), half = n / 2;
    char *left = malloc(half + 1), *right = malloc(half + 1);
    int same;

    memcpy(left, s, half);
    left[half] = '\0';
    memcpy(right, s + half, half);
    right[half] = '\0';
    same = count_vowels(left) == count_vowels(right);
    free(left);
    free(right);
    return same;
}
