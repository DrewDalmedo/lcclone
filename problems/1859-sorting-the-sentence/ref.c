/* Reference solution: walk along s once. Each word ends in its place
 * digit, so file where the word starts and how many letters it has under
 * that place. Then copy the words out place by place, one space apart. */
char *sortSentence(char *s)
{
    const char *start[9], *p = s, *word;
    int len[9], k = 0, n = 0, place;
    char *out = malloc(strlen(s) + 1);

    while (*p) {
        word = p;
        while (*p && *p != ' ')
            p++;
        place = p[-1] - '1';
        start[place] = word;
        len[place] = (int)(p - word) - 1;
        k++;
        if (*p)
            p++;
    }
    for (place = 0; place < k; place++) {
        if (place > 0)
            out[n++] = ' ';
        memcpy(out + n, start[place], (size_t)len[place]);
        n += len[place];
    }
    out[n] = '\0';
    return out;
}
