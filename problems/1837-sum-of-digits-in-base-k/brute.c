/* Slow and obviously right: start the abacus at zero and add one bead n
 * times, moving k beads on a rod over as one bead on the next rod, then
 * count the beads. */
int sumBase(int n, int k)
{
    int rods[16] = {0}, i, r, sum = 0;

    for (i = 0; i < n; i++) {
        r = 0;
        rods[r]++;
        while (rods[r] == k) {
            rods[r] = 0;
            r++;
            rods[r]++;
        }
    }
    for (r = 0; r < 16; r++)
        sum += rods[r];
    return sum;
}
