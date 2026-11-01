/* Reference solution: a stamp for both strings exists exactly when
 * str1 + str2 equals str2 + str1 (compared letter by letter, without
 * building them), and then the longest one is the prefix of str1 whose
 * length is the gcd of the two lengths. */
char *gcdOfStrings(char *str1, char *str2)
{
    size_t n1 = strlen(str1), n2 = strlen(str2), a = n1, b = n2, t, i;
    char *out, x, y;

    for (i = 0; i < n1 + n2; i++) {
        x = i < n1 ? str1[i] : str2[i - n1];
        y = i < n2 ? str2[i] : str1[i - n2];
        if (x != y) {
            out = malloc(1);
            out[0] = '\0';
            return out;
        }
    }
    while (b != 0) {
        t = a % b;
        a = b;
        b = t;
    }
    out = malloc(a + 1);
    memcpy(out, str1, a);
    out[a] = '\0';
    return out;
}
