/* Reference solution: keep the tape as a stack. An integer, "+" and "D"
 * push a number, "C" pops one; at the end, add up what is left. */
int calPoints(char **operations, int operationsSize)
{
    int *tape = malloc((size_t)operationsSize * sizeof *tape);
    int top = 0, sum = 0, i;

    for (i = 0; i < operationsSize; i++) {
        const char *op = operations[i];

        if (strcmp(op, "+") == 0) {
            tape[top] = tape[top - 1] + tape[top - 2];
            top++;
        } else if (strcmp(op, "D") == 0) {
            tape[top] = 2 * tape[top - 1];
            top++;
        } else if (strcmp(op, "C") == 0) {
            top--;
        } else {
            tape[top++] = (int)strtol(op, NULL, 10);
        }
    }
    for (i = 0; i < top; i++)
        sum += tape[i];
    free(tape);
    return sum;
}
