/* Reference solution: walk along num, adding the digits at even indices
 * (the light squares) to a running difference and subtracting those at
 * odd indices (the dark ones); num is balanced when it ends at 0. */
bool isBalanced(char *num)
{
    int diff = 0, i;

    for (i = 0; num[i]; i++)
        diff += i % 2 == 0 ? num[i] - '0' : -(num[i] - '0');
    return diff == 0;
}
