/* Reference solution: a jump changes x + y by 1 or 3, an odd amount, so
 * the parity of x + y flips with every jump, and a knight can reach every
 * square of an 8 by 8 board. So a route of even length exists exactly
 * when x + y has the same parity at start and at target. */
bool canReach(int *start, int startSize, int *target, int targetSize)
{
    (void)startSize;
    (void)targetSize;
    return (start[0] + start[1] + target[0] + target[1]) % 2 == 0;
}
