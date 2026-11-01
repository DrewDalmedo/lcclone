/* Words over the first few letters of the alphabet, and x one of those
 * letters or the letter just after them, which no word has. A word is
 * sometimes a copy of an earlier one. */
void gen(int small)
{
    static char words[50][51];
    char *ptrs[50], letters[27];
    int n = small ? (int)rnd(1, 6) : 50, maxlen = small ? 5 : 50;
    int a = small ? (int)rnd(1, 4) : (int)rnd(4, 26), i, k, len;

    for (i = 0; i < a; i++)
        letters[i] = (char)('a' + i);
    letters[a] = '\0';
    for (k = 0; k < n; k++) {
        if (k > 0 && rnd_chance(20)) {
            strcpy(words[k], words[rnd(0, k - 1)]);
        } else {
            len = (int)rnd(1, maxlen);
            for (i = 0; i < len; i++)
                words[k][i] = rnd_char(letters);
            words[k][len] = '\0';
        }
        ptrs[k] = words[k];
    }
    put_strings(ptrs, n);
    put_char((char)('a' + rnd(0, a < 26 ? a : 25)));
}
