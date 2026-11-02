/* Reference solution: work out each element's reflection once, by peeling
 * bits off the low end of x and pushing them onto the reflection, then
 * sort (reflection, value) pairs with a two-level comparator. */
struct item {
    int refl, value;
};

static int cmp_item(const void *pa, const void *pb)
{
    const struct item *a = pa, *b = pb;

    if (a->refl != b->refl)
        return a->refl < b->refl ? -1 : 1;
    return (a->value > b->value) - (a->value < b->value);
}

int *sortByReflection(int *nums, int numsSize, int *returnSize)
{
    struct item *items = malloc((size_t)numsSize * sizeof *items);
    int *out = malloc((size_t)numsSize * sizeof *out);
    int i, x, r;

    for (i = 0; i < numsSize; i++) {
        x = nums[i];
        r = 0;
        while (x > 0) {
            r = r * 2 + (x & 1);
            x >>= 1;
        }
        items[i].refl = r;
        items[i].value = nums[i];
    }
    qsort(items, (size_t)numsSize, sizeof *items, cmp_item);
    for (i = 0; i < numsSize; i++)
        out[i] = items[i].value;
    free(items);
    *returnSize = numsSize;
    return out;
}
