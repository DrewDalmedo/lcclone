/* Slow and obviously right: try 1, 2, 3, ... and return the first number
 * one of whose digits is in nums1 and one of whose digits is in nums2. */
static int has(const int *a, int n, int d)
{
    int i;

    for (i = 0; i < n; i++)
        if (a[i] == d)
            return 1;
    return 0;
}

int minNumber(int *nums1, int nums1Size, int *nums2, int nums2Size)
{
    int x, v, in1, in2;

    for (x = 1;; x++) {
        in1 = 0;
        in2 = 0;
        for (v = x; v > 0; v /= 10) {
            if (has(nums1, nums1Size, v % 10))
                in1 = 1;
            if (has(nums2, nums2Size, v % 10))
                in2 = 1;
        }
        if (in1 && in2)
            return x;
    }
}
