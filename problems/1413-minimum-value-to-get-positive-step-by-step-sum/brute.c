/* Slow and obviously right: try v = 1, 2, 3, ... and replay every
 * transaction, stopping at the first v whose balance never drops below 1. */
int minStartValue(int *nums, int numsSize)
{
    int v, balance, i, ok;

    for (v = 1;; v++) {
        balance = v;
        ok = 1;
        for (i = 0; i < numsSize && ok; i++) {
            balance += nums[i];
            if (balance < 1)
                ok = 0;
        }
        if (ok)
            return v;
    }
}
