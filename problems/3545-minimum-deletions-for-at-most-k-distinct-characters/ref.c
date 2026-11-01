/* Reference solution: count each letter, sort the 26 counts from largest
 * to smallest, keep the k largest letters and skip everything else, so
 * the answer is the sum of the counts from position k on. */
int minDeletion(char *s, int k)
{
    int cnt[26] = {0};
    int i, j, t, skipped = 0;
    const char *p;

    for (p = s; *p; p++)
        cnt[*p - 'a']++;
    for (i = 1; i < 26; i++) {
        t = cnt[i];
        for (j = i; j > 0 && cnt[j - 1] < t; j--)
            cnt[j] = cnt[j - 1];
        cnt[j] = t;
    }
    for (i = k; i < 26; i++)
        skipped += cnt[i];
    return skipped;
}
