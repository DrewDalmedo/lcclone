/* Slow and obviously right: run the race. Each tick, look whether person
 * 1 or person 2 has reached z; if neither has, move each of them one step
 * toward z. */
int findClosest(int x, int y, int z)
{
    int a = x, b = y;

    for (;;) {
        if (a == z && b == z)
            return 0;
        if (a == z)
            return 1;
        if (b == z)
            return 2;
        a += a < z ? 1 : -1;
        b += b < z ? 1 : -1;
    }
}
