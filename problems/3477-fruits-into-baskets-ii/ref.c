/* Reference solution: for each batch in turn, scan the baskets from the
 * left for the first one that is not taken and is big enough, and mark
 * it taken. A batch that finds none is counted. O(n^2) steps. */
int numOfUnplacedFruits(int *fruits, int fruitsSize, int *baskets,
                        int basketsSize)
{
    bool *taken = calloc((size_t)basketsSize, sizeof *taken);
    int left_out = 0, i, j;

    for (i = 0; i < fruitsSize; i++) {
        for (j = 0; j < basketsSize; j++)
            if (!taken[j] && baskets[j] >= fruits[i])
                break;
        if (j < basketsSize)
            taken[j] = true;
        else
            left_out++;
    }
    free(taken);
    return left_out;
}
