/* Reference solution: sort the indices by score, highest first; the k-th
 * index in that order (from 0) is in place k + 1. Each label is written into
 * a malloc'ed string of its own. */
struct entry {
    int score;
    int index;
};

static int by_score_desc(const void *a, const void *b)
{
    int x = ((const struct entry *)a)->score;
    int y = ((const struct entry *)b)->score;

    return (x < y) - (x > y);
}

char **findRelativeRanks(int *score, int scoreSize, int *returnSize)
{
    static const char *medals[] = {"Gold Medal", "Silver Medal", "Bronze Medal"};
    struct entry *order = malloc((size_t)scoreSize * sizeof *order);
    char **out = malloc((size_t)scoreSize * sizeof *out);
    char buf[16];
    const char *label;
    int k;

    for (k = 0; k < scoreSize; k++) {
        order[k].score = score[k];
        order[k].index = k;
    }
    qsort(order, (size_t)scoreSize, sizeof *order, by_score_desc);
    for (k = 0; k < scoreSize; k++) {
        if (k < 3) {
            label = medals[k];
        } else {
            snprintf(buf, sizeof buf, "%d", k + 1);
            label = buf;
        }
        out[order[k].index] = malloc(strlen(label) + 1);
        strcpy(out[order[k].index], label);
    }
    free(order);
    *returnSize = scoreSize;
    return out;
}
