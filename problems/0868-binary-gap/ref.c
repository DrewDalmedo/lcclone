/* Reference solution: walk the bits from place 0 up, remembering the place
 * of the last 1 seen; at each 1 after the first, the distance back to it is
 * a candidate. */
int binaryGap(int n)
{
    int last = -1, best = 0, place;

    for (place = 0; place < 31; place++) {
        if ((n >> place) & 1) {
            if (last >= 0 && place - last > best)
                best = place - last;
            last = place;
        }
    }
    return best;
}
