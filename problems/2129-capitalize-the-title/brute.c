/* Slow and obviously right: for each letter, find the word around it by
 * scanning out to the spaces (or ends) on both sides, then pick its case
 * from that word's length and whether the letter starts the word. */
char *capitalizeTitle(char *title)
{
    int n = (int)strlen(title), i, lo, hi;
    char *out = malloc((size_t)n + 1), c;

    for (i = 0; i < n; i++) {
        c = title[i];
        if (c != ' ') {
            lo = i;
            while (lo > 0 && title[lo - 1] != ' ')
                lo--;
            hi = i;
            while (hi + 1 < n && title[hi + 1] != ' ')
                hi++;
            if (c >= 'A' && c <= 'Z')
                c = (char)(c - 'A' + 'a');
            if (i == lo && hi - lo + 1 >= 3)
                c = (char)(c - 'a' + 'A');
        }
        out[i] = c;
    }
    out[n] = '\0';
    return out;
}
