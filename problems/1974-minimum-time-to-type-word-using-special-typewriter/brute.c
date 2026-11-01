/* Slow and obviously right: for each letter, turn the dial one notch at a
 * time forwards until the marker reaches it, then again backwards, count
 * the notches both ways and take the fewer, then print. */
int minTimeToType(char *word)
{
    int total = 0, at = 0, i, pos, fwd, back;

    for (i = 0; word[i] != '\0'; i++) {
        int want = word[i] - 'a';

        for (pos = at, fwd = 0; pos != want; fwd++)
            pos = (pos + 1) % 26;
        for (pos = at, back = 0; pos != want; back++)
            pos = (pos + 25) % 26;
        total += (fwd < back ? fwd : back) + 1;
        at = want;
    }
    return total;
}
