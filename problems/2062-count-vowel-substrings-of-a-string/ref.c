/* Reference solution: one pass. Keep where the current run of vowels
 * began and where each vowel was last seen. A good highlight ending at r
 * may start anywhere from the run's start up to the earliest of the five
 * last positions, so add that many (none when some vowel was last seen
 * before the run began, or not at all). */
int countVowelSubstrings(char *word)
{
    static const char vowels[] = "aeiou";
    int last[5] = {-1, -1, -1, -1, -1};
    int start = 0, count = 0, r, v, first;
    const char *p;

    for (r = 0; word[r] != '\0'; r++) {
        p = strchr(vowels, word[r]);
        if (p == NULL) {
            start = r + 1;
            continue;
        }
        last[p - vowels] = r;
        first = last[0];
        for (v = 1; v < 5; v++)
            if (last[v] < first)
                first = last[v];
        if (first >= start)
            count += first - start + 1;
    }
    return count;
}
