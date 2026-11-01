/* Reference solution: the answer is always true. If all the values have
 * the same parity, copy them all. Otherwise copy the odd ones, and from
 * each even one subtract one odd value (at another position, since the
 * value there is odd): even minus odd is odd, so every entry is odd. */
bool uniformArray(int *nums1, int nums1Size)
{
    (void)nums1;
    (void)nums1Size;
    return true;
}
