/* Slow and obviously right: try every highlight word[l..r] and look at
 * each letter it covers, to see that all of them are vowels and which
 * vowels appear. */
int countVowelSubstrings(char *word)
{
    int n = (int)strlen(word), count = 0, l, r, k, consonant;
    int seen[26];
    char c;

    for (l = 0; l < n; l++)
        for (r = l; r < n; r++) {
            memset(seen, 0, sizeof seen);
            consonant = 0;
            for (k = l; k <= r; k++) {
                c = word[k];
                if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u')
                    consonant = 1;
                seen[c - 'a'] = 1;
            }
            if (!consonant && seen['a' - 'a'] && seen['e' - 'a'] &&
                seen['i' - 'a'] && seen['o' - 'a'] && seen['u' - 'a'])
                count++;
        }
    return count;
}
