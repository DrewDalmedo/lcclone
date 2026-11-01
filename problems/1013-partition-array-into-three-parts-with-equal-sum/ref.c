/* Reference solution: each piece must sum to a third of the total, so the
 * total must divide by 3. Walk the prefix sums: take the earliest point
 * where they reach a third, then the earliest later point where they reach
 * two thirds, and check that at least one element is left for the back. */
bool canThreePartsEqualSum(int *arr, int arrSize)
{
    long long total = 0, third, sum = 0;
    int i, found = 0;

    for (i = 0; i < arrSize; i++)
        total += arr[i];
    if (total % 3 != 0)
        return false;
    third = total / 3;
    for (i = 0; i < arrSize - 1; i++) {
        sum += arr[i];
        if (!found && sum == third)
            found = 1;
        else if (found && sum == 2 * third)
            return true;
    }
    return false;
}
