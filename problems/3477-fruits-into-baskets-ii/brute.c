/* Straight from the rule, with other bookkeeping: keep the capacities of
 * the empty baskets in a list, in their order along the row. Each batch
 * walks the list from the front and removes the first basket that is big
 * enough, closing the gap; a batch that finds none is counted. */
int numOfUnplacedFruits(int *fruits, int fruitsSize, int *baskets,
                        int basketsSize)
{
    int *empty = malloc((size_t)basketsSize * sizeof *empty);
    int count = basketsSize, left_out = 0, i, j, t;

    for (j = 0; j < basketsSize; j++)
        empty[j] = baskets[j];
    for (i = 0; i < fruitsSize; i++) {
        for (j = 0; j < count && empty[j] < fruits[i]; j++)
            ;
        if (j == count) {
            left_out++;
            continue;
        }
        for (t = j; t + 1 < count; t++)
            empty[t] = empty[t + 1];
        count--;
    }
    free(empty);
    return left_out;
}
