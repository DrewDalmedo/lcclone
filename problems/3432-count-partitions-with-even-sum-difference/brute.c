/* Slow and obviously right: at every coupling, add up both trains from
 * scratch and test their difference. */
int countPartitions(int *nums, int numsSize)
{
    int count = 0, cut, j, front, back, diff;

    for (cut = 1; cut < numsSize; cut++) {
        front = back = 0;
        for (j = 0; j < cut; j++)
            front += nums[j];
        for (j = cut; j < numsSize; j++)
            back += nums[j];
        diff = front > back ? front - back : back - front;
        if (diff % 2 == 0)
            count++;
    }
    return count;
}
