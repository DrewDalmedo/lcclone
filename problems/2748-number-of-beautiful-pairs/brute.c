/* Slow and obviously right: for every i < j, write both numbers out, take
 * the first character of nums[i] and the last of nums[j], and try every
 * divisor from 2 to 9 on the two digits. */
int countBeautifulPairs(int *nums, int numsSize)
{
    char s[16];
    int pairs = 0, i, j, first, last, d, shared;

    for (i = 0; i < numsSize; i++) {
        snprintf(s, sizeof s, "%d", nums[i]);
        first = s[0] - '0';
        for (j = i + 1; j < numsSize; j++) {
            snprintf(s, sizeof s, "%d", nums[j]);
            last = s[strlen(s) - 1] - '0';
            shared = 0;
            for (d = 2; d <= 9; d++)
                if (first % d == 0 && last % d == 0)
                    shared = 1;
            if (!shared)
                pairs++;
        }
    }
    return pairs;
}
