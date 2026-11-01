/* Reference solution: a monotonic stack. Scan left to right, keeping the
 * items that have no partner yet; their tags rise strictly from the bottom
 * of the stack to the top. Item j is the partner of every item on top whose
 * tag is at least prices[j], so pop those and settle them, then push j.
 * Whatever is left at the end keeps its full tag. */
int *finalPrices(int *prices, int pricesSize, int *returnSize)
{
    int *out = malloc((size_t)pricesSize * sizeof *out);
    int *stack = malloc((size_t)pricesSize * sizeof *stack);
    int top = 0, j;

    for (j = 0; j < pricesSize; j++) {
        out[j] = prices[j];
        while (top > 0 && prices[stack[top - 1]] >= prices[j]) {
            top--;
            out[stack[top]] = prices[stack[top]] - prices[j];
        }
        stack[top++] = j;
    }
    free(stack);
    *returnSize = pricesSize;
    return out;
}
