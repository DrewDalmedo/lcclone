/* Reference solution: hand the letters out in rounds, one to each of the
 * eight keys per round. The i-th letter (counting from 0) then stands in
 * place i / 8 + 1 of its key, and costs that many presses. */
int minimumPushes(char *word)
{
    int m = (int)strlen(word), total = 0, i;

    for (i = 0; i < m; i++)
        total += i / 8 + 1;
    return total;
}
