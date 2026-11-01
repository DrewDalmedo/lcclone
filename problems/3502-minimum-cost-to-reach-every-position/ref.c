/* Reference solution: one paid trade is enough. To reach place i, pay the
 * cheapest person at places 0..i and then trade backwards for free; and
 * any route into places 0..i must pay one of those people first. So
 * answer[i] is the smallest of cost[0..i], a running minimum. */
int *minCosts(int *cost, int costSize, int *returnSize)
{
    int *answer = malloc((size_t)costSize * sizeof *answer);
    int best = cost[0], i;

    for (i = 0; i < costSize; i++) {
        if (cost[i] < best)
            best = cost[i];
        answer[i] = best;
    }
    *returnSize = costSize;
    return answer;
}
