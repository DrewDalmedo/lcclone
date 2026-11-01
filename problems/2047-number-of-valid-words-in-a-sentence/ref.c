/* Reference solution: one pass over the sentence. Each chunk is read
 * against the pattern letters, then optionally a hyphen and more letters,
 * then optionally one closing mark; it is a valid word when the pattern
 * uses up the whole chunk. */
static int is_letter(char c)
{
    return c >= 'a' && c <= 'z';
}

static int is_mark(char c)
{
    return c == '!' || c == '.' || c == ',';
}

/* Whether the n characters at t form a valid word. */
static int valid(const char *t, int n)
{
    int i = 0;

    if (i < n && is_letter(t[i])) {
        while (i < n && is_letter(t[i]))
            i++;
        if (i < n && t[i] == '-') {
            i++;
            if (i == n || !is_letter(t[i]))
                return 0;
            while (i < n && is_letter(t[i]))
                i++;
        }
    }
    if (i < n && is_mark(t[i]))
        i++;
    return i == n;
}

int countValidWords(char *sentence)
{
    int count = 0, start, i = 0;

    while (sentence[i] != '\0') {
        if (sentence[i] == ' ') {
            i++;
            continue;
        }
        start = i;
        while (sentence[i] != '\0' && sentence[i] != ' ')
            i++;
        count += valid(sentence + start, i - start);
    }
    return count;
}
