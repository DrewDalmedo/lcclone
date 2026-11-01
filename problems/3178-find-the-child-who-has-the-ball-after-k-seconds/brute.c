/* Slow and obviously right: follow the ball for k seconds, one throw at a
 * time, turning around whenever it reaches an end of the line. */
int numberOfChild(int n, int k)
{
    int pos = 0, step = 1, t;

    for (t = 0; t < k; t++) {
        pos += step;
        if (pos == 0 || pos == n - 1)
            step = -step;
    }
    return pos;
}
