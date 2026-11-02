/* Slow and obviously right: for every entry, count the entries equal to
 * it by scanning the whole array, and call that count prime when it has
 * exactly two divisors among 1..count. */
bool checkPrimeFrequency(int *nums, int numsSize)
{
    int i, k, count, divisors, d;

    for (i = 0; i < numsSize; i++) {
        count = 0;
        for (k = 0; k < numsSize; k++)
            if (nums[k] == nums[i])
                count++;
        divisors = 0;
        for (d = 1; d <= count; d++)
            if (count % d == 0)
                divisors++;
        if (divisors == 2)
            return true;
    }
    return false;
}
