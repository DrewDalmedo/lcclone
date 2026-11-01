/* Reference solution: vote cancelling (Boyer-Moore). Keep a candidate and
 * a count; an equal element adds one, a different one takes one away, and
 * a count of zero lets the next element become the candidate. Every
 * element that takes a vote away cancels one copy of the candidate, and
 * the majority has more copies than all the other elements together, so
 * it is the candidate at the end. */
int majorityElement(int *nums, int numsSize)
{
    int cand = nums[0], count = 0, i;

    for (i = 0; i < numsSize; i++) {
        if (count == 0)
            cand = nums[i];
        count += nums[i] == cand ? 1 : -1;
    }
    return cand;
}
