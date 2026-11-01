/* Reference solution: one pass over each code builds its decimal value
 * and its length, and notes whether a letter was seen; the code is worth
 * its value if not, else its length. Keep the largest worth. Nine digits
 * at most, so the value fits in an int. */
int maximumValue(char **strs, int strsSize)
{
    int best = 0, i, len, value, letter, worth;
    const char *s;

    for (i = 0; i < strsSize; i++) {
        s = strs[i];
        value = 0;
        letter = 0;
        for (len = 0; s[len]; len++) {
            if (s[len] >= '0' && s[len] <= '9')
                value = value * 10 + (s[len] - '0');
            else
                letter = 1;
        }
        worth = letter ? len : value;
        if (worth > best)
            best = worth;
    }
    return best;
}
