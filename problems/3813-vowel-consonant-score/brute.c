/* Slow and obviously right: count the vowels by looking each letter up in
 * "aeiou", the consonants as the other letters, and find the score straight
 * from its definition by raising q while (q + 1) * C still fits in V. */
int vowelConsonantScore(char *s)
{
    int v = 0, c = 0, q = 0, i;

    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            if (strchr("aeiou", s[i]) != NULL)
                v++;
            else
                c++;
        }
    }
    if (c == 0)
        return 0;
    while ((q + 1) * c <= v)
        q++;
    return q;
}
