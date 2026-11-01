/* Slow and obviously right: for each letter of the note, cut out the first
 * unused equal letter of magazine; fail when there is none left. */
bool canConstruct(char *ransomNote, char *magazine)
{
    size_t n = strlen(ransomNote), m = strlen(magazine), i, k;
    bool *used = calloc(m + 1, sizeof *used), ok = true;

    for (i = 0; i < n && ok; i++) {
        for (k = 0; k < m && (used[k] || magazine[k] != ransomNote[i]); k++)
            ;
        if (k == m)
            ok = false;
        else
            used[k] = true;
    }
    free(used);
    return ok;
}
