/* Reference solution: two tables, one from each character of s to its
 * code character in t and one back. A pair (s[i], t[i]) must agree with
 * both tables, or else be new to both. NUL never occurs, so 0 marks a
 * character with no entry yet. */
bool isIsomorphic(char *s, char *t)
{
    unsigned char code[128] = {0}, back[128] = {0};
    size_t i;

    for (i = 0; s[i] != '\0'; i++) {
        unsigned char a = (unsigned char)s[i], b = (unsigned char)t[i];

        if (code[a] == 0 && back[b] == 0) {
            code[a] = b;
            back[b] = a;
        } else if (code[a] != b || back[b] != a) {
            return false;
        }
    }
    return true;
}
