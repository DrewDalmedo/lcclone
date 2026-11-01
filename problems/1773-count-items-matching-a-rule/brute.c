/* Slow and obviously right, straight from the definition: a row passes
 * when one of its three columns has ruleKey as its title and ruleValue as
 * its word, both compared letter by letter. */
static int same(const char *s, const char *t)
{
    while (*s && *s == *t) {
        s++;
        t++;
    }
    return *s == *t;
}

int countMatches(char ***items, int itemsSize, int *itemsColSize,
                 char *ruleKey, char *ruleValue)
{
    static const char *titles[3] = {"type", "color", "name"};
    int count = 0, i, c, passes;

    for (i = 0; i < itemsSize; i++) {
        passes = 0;
        for (c = 0; c < itemsColSize[i]; c++)
            if (same(titles[c], ruleKey) && same(items[i][c], ruleValue))
                passes = 1;
        count += passes;
    }
    return count;
}
