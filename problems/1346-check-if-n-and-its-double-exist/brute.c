/* Slow and obviously right: try every ordered pair of different positions. */
bool checkIfExist(int *arr, int arrSize)
{
    int i, j;

    for (i = 0; i < arrSize; i++)
        for (j = 0; j < arrSize; j++)
            if (i != j && arr[i] == 2 * arr[j])
                return true;
    return false;
}
