/* Slow and obviously right: a rectangle is the answer when no other one
 * has a longer diagonal, or the same diagonal and a larger area. Test
 * each rectangle against all of them, and return the area of the first
 * that passes. */
int areaOfMaxDiagonal(int **dimensions, int dimensionsSize,
                      int *dimensionsColSize)
{
    int i, k, di, dk, ai, ak, beaten;

    (void)dimensionsColSize;
    for (i = 0; i < dimensionsSize; i++) {
        di = dimensions[i][0] * dimensions[i][0] +
             dimensions[i][1] * dimensions[i][1];
        ai = dimensions[i][0] * dimensions[i][1];
        beaten = 0;
        for (k = 0; k < dimensionsSize; k++) {
            dk = dimensions[k][0] * dimensions[k][0] +
                 dimensions[k][1] * dimensions[k][1];
            ak = dimensions[k][0] * dimensions[k][1];
            if (dk > di || (dk == di && ak > ai))
                beaten = 1;
        }
        if (!beaten)
            return ai;
    }
    return -1;
}
