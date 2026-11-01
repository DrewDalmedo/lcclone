/* Slow and obviously right: for each digit, find the letter before it in
 * the alphabet, then walk forward through the alphabet one letter at a
 * time, as many steps as the digit says. */
char *replaceDigits(char *s)
{
    const char *alphabet = "abcdefghijklmnopqrstuvwxyz";
    size_t n = strlen(s), i;
    char *out = malloc(n + 1);
    int pos, steps;

    for (i = 0; i < n; i++) {
        if (i % 2 == 0) {
            out[i] = s[i];
            continue;
        }
        pos = 0;
        while (alphabet[pos] != s[i - 1])
            pos++;
        for (steps = 0; steps < s[i] - '0'; steps++)
            pos++;
        out[i] = alphabet[pos];
    }
    out[n] = '\0';
    return out;
}
