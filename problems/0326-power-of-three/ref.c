/* Reference solution: 3^19 = 1162261467 is the largest power of three
 * that fits in an int. Since 3 is prime, the positive divisors of 3^19 are
 * exactly 3^0, ..., 3^19, so a positive n is a power of three exactly when
 * it divides 3^19. Testing n > 0 first rules out 0 (no division by zero)
 * and the negated powers -3^k, which divide 3^19 too. */
bool isPowerOfThree(int n)
{
    return n > 0 && 1162261467 % n == 0;
}
