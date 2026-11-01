/* Slow and obviously right: try every x from -1000 to 1000, and for each
 * check that every value of nums1 + x occurs in it exactly as often as in
 * nums2. */
int addedInteger(int *nums1, int nums1Size, int *nums2, int nums2Size)
{
    int x, i, j, in1, in2, ok;

    (void)nums2Size;
    for (x = -1000; x <= 1000; x++) {
        ok = 1;
        for (i = 0; i < nums1Size && ok; i++) {
            in1 = in2 = 0;
            for (j = 0; j < nums1Size; j++) {
                if (nums1[j] == nums1[i])
                    in1++;
                if (nums2[j] == nums1[i] + x)
                    in2++;
            }
            if (in1 != in2)
                ok = 0;
        }
        if (ok)
            return x;
    }
    return 0;
}
