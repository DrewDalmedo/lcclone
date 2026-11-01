/* Slow and obviously right: for each letter of message, find where it
 * first occurs in key and count the different letters before that spot;
 * if there are k of them, it stands for the letter k places after 'a'. */
char *decodeMessage(char *key, char *message)
{
    size_t len = strlen(message), i;
    char *out = malloc(len + 1);
    int seen[26], first, k, p, q;

    for (i = 0; i < len; i++) {
        if (message[i] == ' ') {
            out[i] = ' ';
            continue;
        }
        first = 0;
        while (key[first] != message[i])
            first++;
        memset(seen, 0, sizeof seen);
        for (p = 0; p < first; p++)
            if (key[p] != ' ')
                seen[key[p] - 'a'] = 1;
        k = 0;
        for (q = 0; q < 26; q++)
            k += seen[q];
        out[i] = (char)('a' + k);
    }
    out[len] = '\0';
    return out;
}
