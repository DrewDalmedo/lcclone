/* Reference solution: the numbers are distinct, so each number starts at
 * most one piece. Record, for every number, the piece it starts (index + 1,
 * 0 for none), then walk arr: at each position the piece starting with
 * arr's number must come next, and must match arr number by number. */
bool canFormArray(int *arr, int arrSize, int **pieces, int piecesSize,
                  int *piecesColSize)
{
    int start[101] = {0};
    int i, j, p;

    for (i = 0; i < piecesSize; i++)
        start[pieces[i][0]] = i + 1;
    i = 0;
    while (i < arrSize) {
        p = start[arr[i]] - 1;
        if (p < 0 || piecesColSize[p] > arrSize - i)
            return false;
        for (j = 0; j < piecesColSize[p]; j++, i++)
            if (arr[i] != pieces[p][j])
                return false;
    }
    return true;
}
