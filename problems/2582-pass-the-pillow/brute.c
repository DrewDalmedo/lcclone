/* Slow and obviously right: move the pillow one chair per second, turning
 * round whenever the next chair would be off either end of the row. */
int passThePillow(int n, int time)
{
    int chair = 1, step = 1, s;

    for (s = 0; s < time; s++) {
        if (chair + step < 1 || chair + step > n)
            step = -step;
        chair += step;
    }
    return chair;
}
