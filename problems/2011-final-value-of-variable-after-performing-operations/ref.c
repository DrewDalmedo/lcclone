/* Reference solution: the middle character of every statement is the sign
 * of its step ('+' or '-'), whichever side X is on. */
int finalValueAfterOperations(char **operations, int operationsSize)
{
    int x = 0, i;

    for (i = 0; i < operationsSize; i++)
        x += operations[i][1] == '+' ? 1 : -1;
    return x;
}
