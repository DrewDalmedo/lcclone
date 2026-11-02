/* Slow and obviously right: find each letter's weight by looking it up in
 * the alphabet written backwards, then add that weight once for every
 * place up to and including the letter's own. */
int reverseDegree(char *s)
{
    const char *backwards = "zyxwvutsrqponmlkjihgfedcba";
    int total = 0, i, w, k;

    for (i = 0; s[i] != '\0'; i++) {
        for (w = 0; backwards[w] != s[i]; w++)
            ;
        for (k = 0; k <= i; k++)
            total += w + 1;
    }
    return total;
}
