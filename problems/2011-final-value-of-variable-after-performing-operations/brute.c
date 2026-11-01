/* Slow and obviously right: compare each statement with the four forms in
 * full and apply its step. */
int finalValueAfterOperations(char **operations, int operationsSize)
{
    int x = 0, i;

    for (i = 0; i < operationsSize; i++) {
        if (strcmp(operations[i], "++X") == 0)
            ++x;
        else if (strcmp(operations[i], "X++") == 0)
            x++;
        else if (strcmp(operations[i], "--X") == 0)
            --x;
        else if (strcmp(operations[i], "X--") == 0)
            x--;
    }
    return x;
}
