/* Slow and obviously right: try every pair of cuts and add up each of the
 * three pieces from scratch. */
static long long piece(const int *arr, int from, int to)
{
    long long sum = 0;
    int k;

    for (k = from; k <= to; k++)
        sum += arr[k];
    return sum;
}

bool canThreePartsEqualSum(int *arr, int arrSize)
{
    long long a, b, c;
    int i, j;

    for (i = 0; i < arrSize; i++)
        for (j = i + 2; j <= arrSize - 1; j++) {
            a = piece(arr, 0, i);
            b = piece(arr, i + 1, j - 1);
            c = piece(arr, j, arrSize - 1);
            if (a == b && b == c)
                return true;
        }
    return false;
}
