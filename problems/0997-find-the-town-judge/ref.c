/* Reference solution: give everyone a score, +1 for each person who
 * trusts them and -1 for each person they trust. Since pairs are distinct
 * and nobody trusts themselves, a score of n - 1 means trusted by all
 * n - 1 others and trusting no one. */
int findJudge(int n, int **trust, int trustSize, int *trustColSize)
{
    int *score = calloc((size_t)n + 1, sizeof *score);
    int i, judge = -1;

    (void)trustColSize;
    for (i = 0; i < trustSize; i++) {
        score[trust[i][0]]--;
        score[trust[i][1]]++;
    }
    for (i = 1; i <= n; i++)
        if (score[i] == n - 1) {
            judge = i;
            break;
        }
    free(score);
    return judge;
}
