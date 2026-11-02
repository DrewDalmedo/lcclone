/* Slow and obviously right: go through the four lines of business in
 * order, and within each one repeatedly pick the smallest code not taken
 * yet among the coupons that pass, comparing codes character by
 * character by hand. */
static int sameText(const char *a, const char *b)
{
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return *a == *b;
}

/* Is a before b: the first differing character is smaller, or a ends
 * first. */
static int before(const char *a, const char *b)
{
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return (unsigned char)*a < (unsigned char)*b;
}

static int passes(const char *c, const char *line, const char *want,
                  bool active)
{
    const char *p;

    if (!active || !sameText(line, want) || c[0] == '\0')
        return 0;
    for (p = c; *p; p++)
        if (!strchr("abcdefghijklmnopqrstuvwxyz"
                    "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_", *p))
            return 0;
    return 1;
}

char **validateCoupons(char **code, int codeSize, char **businessLine,
                       int businessLineSize, bool *isActive, int isActiveSize,
                       int *returnSize)
{
    static const char *lines[] = {"electronics", "grocery", "pharmacy",
                                  "restaurant"};
    char **out = malloc((size_t)codeSize * sizeof *out);
    int taken[100] = {0};
    int m = 0, r, i, best;

    (void)businessLineSize;
    (void)isActiveSize;
    for (r = 0; r < 4; r++)
        for (;;) {
            best = -1;
            for (i = 0; i < codeSize; i++)
                if (!taken[i] &&
                    passes(code[i], businessLine[i], lines[r], isActive[i]) &&
                    (best < 0 || before(code[i], code[best])))
                    best = i;
            if (best < 0)
                break;
            taken[best] = 1;
            out[m] = malloc(strlen(code[best]) + 1);
            strcpy(out[m], code[best]);
            m++;
        }
    *returnSize = m;
    return out;
}
