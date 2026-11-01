/* Reference solution: keep the coupons that pass the three checks, each
 * with the rank of its line of business (0 to 3), sort them by rank and
 * then by code with strcmp (byte order), and copy the codes out. */
struct coupon {
    int rank;
    const char *code;
};

static int byRankThenCode(const void *a, const void *b)
{
    const struct coupon *x = a, *y = b;

    if (x->rank != y->rank)
        return x->rank < y->rank ? -1 : 1;
    return strcmp(x->code, y->code);
}

static bool goodCode(const char *s)
{
    if (!*s)
        return false;
    for (; *s; s++)
        if (!((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z') ||
              (*s >= '0' && *s <= '9') || *s == '_'))
            return false;
    return true;
}

char **validateCoupons(char **code, int codeSize, char **businessLine,
                       int businessLineSize, bool *isActive, int isActiveSize,
                       int *returnSize)
{
    static const char *lines[] = {"electronics", "grocery", "pharmacy",
                                  "restaurant"};
    struct coupon *kept = malloc((size_t)codeSize * sizeof *kept);
    char **out;
    int m = 0, i, r;

    (void)businessLineSize;
    (void)isActiveSize;
    for (i = 0; i < codeSize; i++) {
        if (!isActive[i] || !goodCode(code[i]))
            continue;
        for (r = 0; r < 4 && strcmp(businessLine[i], lines[r]) != 0; r++)
            ;
        if (r < 4) {
            kept[m].rank = r;
            kept[m].code = code[i];
            m++;
        }
    }
    qsort(kept, (size_t)m, sizeof *kept, byRankThenCode);
    out = malloc((size_t)(m > 0 ? m : 1) * sizeof *out);
    for (i = 0; i < m; i++) {
        size_t len = strlen(kept[i].code) + 1;

        out[i] = malloc(len);
        memcpy(out[i], kept[i].code, len);
    }
    free(kept);
    *returnSize = m;
    return out;
}
