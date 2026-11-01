/* Reference solution: add up the letters from left to right, but subtract
 * a letter that comes just before one of greater value. */
static int letter_value(char c)
{
    switch (c) {
    case 'I': return 1;
    case 'V': return 5;
    case 'X': return 10;
    case 'L': return 50;
    case 'C': return 100;
    case 'D': return 500;
    case 'M': return 1000;
    default: return 0;
    }
}

int romanToInt(char *s)
{
    int total = 0, v, i;

    for (i = 0; s[i] != '\0'; i++) {
        v = letter_value(s[i]);
        if (s[i + 1] != '\0' && v < letter_value(s[i + 1]))
            total -= v;
        else
            total += v;
    }
    return total;
}
