/* Slow and obviously right: play it out. Walk over the switches from the
 * lowest, flip each one that differs from goal and count the flip, then
 * make sure the panel shows goal. */
int minBitFlips(int start, int goal)
{
    long long shown = start, value;
    int moves = 0, k;

    for (k = 0; k < 31; k++) {
        value = 1LL << k;
        if ((shown & value) != (goal & value)) {
            shown ^= value;
            moves++;
        }
    }
    return shown == goal ? moves : -1;
}
