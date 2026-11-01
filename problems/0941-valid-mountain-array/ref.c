/* Reference solution: climb from the left while each step goes up. The
 * climb must stop strictly inside the array; from there, descend while
 * each step goes down. arr is a mountain when the descent reaches the
 * last element. */
bool validMountainArray(int *arr, int arrSize)
{
    int i = 0;

    while (i + 1 < arrSize && arr[i] < arr[i + 1])
        i++;
    if (i == 0 || i == arrSize - 1)
        return false;
    while (i + 1 < arrSize && arr[i] > arr[i + 1])
        i++;
    return i == arrSize - 1;
}
