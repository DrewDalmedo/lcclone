/* Distinct salaries: draw each one again until it differs from those
 * already drawn. Small cases take a narrow range, at the bottom or the
 * top of the allowed one, so the extremes often sit side by side. */
void gen(int small)
{
    int n = small ? (int)rnd(3, 8) : 100, i, k, again;
    int lo = 1000, hi = 1000000;
    int *salary = malloc((size_t)n * sizeof *salary);

    if (small) {
        if (rnd_chance(30))
            lo = hi - 15;
        else
            hi = lo + 15;
    }
    for (i = 0; i < n; i++)
        do {
            salary[i] = (int)rnd(lo, hi);
            again = 0;
            for (k = 0; k < i; k++)
                if (salary[k] == salary[i])
                    again = 1;
        } while (again);
    put_ints(salary, n);
    free(salary);
}
