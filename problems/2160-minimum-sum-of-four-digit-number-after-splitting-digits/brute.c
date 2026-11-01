/* Slow and obviously right: write the four digits in every possible order,
 * cut that row of digits in every place that leaves both parts non-empty,
 * and keep the smallest sum of the two parts. */
int minimumSum(int num)
{
    int d[4], order[4], best = -1, i, cut, a, b;
    int i0, i1, i2, i3;

    for (i = 0; i < 4; i++) {
        d[i] = num % 10;
        num /= 10;
    }
    for (i0 = 0; i0 < 4; i0++)
        for (i1 = 0; i1 < 4; i1++)
            for (i2 = 0; i2 < 4; i2++)
                for (i3 = 0; i3 < 4; i3++) {
                    if (((1 << i0) | (1 << i1) | (1 << i2) | (1 << i3)) != 15)
                        continue;
                    order[0] = d[i0];
                    order[1] = d[i1];
                    order[2] = d[i2];
                    order[3] = d[i3];
                    for (cut = 1; cut < 4; cut++) {
                        a = 0;
                        b = 0;
                        for (i = 0; i < cut; i++)
                            a = a * 10 + order[i];
                        for (i = cut; i < 4; i++)
                            b = b * 10 + order[i];
                        if (best < 0 || a + b < best)
                            best = a + b;
                    }
                }
    return best;
}
