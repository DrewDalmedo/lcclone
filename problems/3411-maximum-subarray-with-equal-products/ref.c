/* Reference solution. Two elements are always fair (gcd(x, y) * lcm(x, y)
 * is x * y), and a run of three or more is fair exactly when no prime
 * divides two of its elements. Below 10 the primes are 2, 3, 5 and 7, so
 * each element gets a mask of the primes dividing it, and a sliding window
 * keeps the union of its masks with no prime used twice. The answer is the
 * longest such window, but never less than 2. */
int maxLength(int *nums, int numsSize)
{
    static const int mask[11] = {0, 0, 1, 2, 1, 4, 3, 8, 1, 2, 5};
    int used = 0, best = 2, left = 0, right;

    for (right = 0; right < numsSize; right++) {
        while (used & mask[nums[right]])
            used &= ~mask[nums[left++]];
        used |= mask[nums[right]];
        if (right - left + 1 > best)
            best = right - left + 1;
    }
    return best;
}
