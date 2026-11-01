/* Reference solution: walk s once with a counter of brackets still open,
 * up at each '(' and down at each ')', and keep its highest value. */
int maxDepth(char *s)
{
    int open = 0, best = 0;

    for (; *s != '\0'; s++) {
        if (*s == '(') {
            open++;
            if (open > best)
                best = open;
        } else if (*s == ')') {
            open--;
        }
    }
    return best;
}
