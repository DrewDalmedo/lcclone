/* Reference solution: only the run of 1s right before the final 0
 * matters. The bit before that run, if there is one, is a 0, and a 0
 * always ends an instruction (alone or as the end of a 10), so the run
 * starts a new instruction and is read as 11, 11, ... An even run leaves
 * the final 0 alone; an odd one pairs its last 1 with it. */
bool isOneBitCharacter(int *bits, int bitsSize)
{
    int run = 0, i;

    for (i = bitsSize - 2; i >= 0 && bits[i] == 1; i--)
        run++;
    return run % 2 == 0;
}
