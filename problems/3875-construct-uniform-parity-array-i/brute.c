/* Slow and obviously right: the choices at different positions don't
 * affect each other, so the list can be made all of parity p exactly
 * when every position has some option (the value itself, or its
 * difference with another value) of parity p. Try both parities. */
bool uniformArray(int *nums1, int nums1Size)
{
    int p, i, j, found, all;

    for (p = 0; p <= 1; p++) {
        all = 1;
        for (i = 0; i < nums1Size && all; i++) {
            found = (nums1[i] % 2 != 0) == p;
            for (j = 0; j < nums1Size && !found; j++)
                if (j != i && ((nums1[i] - nums1[j]) % 2 != 0) == p)
                    found = 1;
            if (!found)
                all = 0;
        }
        if (all)
            return true;
    }
    return false;
}
