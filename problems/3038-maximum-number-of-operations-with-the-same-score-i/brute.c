/* Slow and obviously right: for each number of operations k, from the
 * largest possible down, do k operations on a copy of the queue, taking
 * out the two front elements each time and moving the rest up, and return
 * the first k whose operations all have the same score. */
int maxOperations(int *nums, int numsSize)
{
    int queue[100], len, k, t, i, first = 0, score, same;

    for (k = numsSize / 2; k >= 1; k--) {
        len = numsSize;
        for (i = 0; i < len; i++)
            queue[i] = nums[i];
        same = 1;
        for (t = 0; t < k; t++) {
            score = queue[0] + queue[1];
            if (t == 0)
                first = score;
            else if (score != first)
                same = 0;
            for (i = 2; i < len; i++)
                queue[i - 2] = queue[i];
            len -= 2;
        }
        if (same)
            return k;
    }
    return 0;
}
