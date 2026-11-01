/* Slow and obviously right: sort a copy, then average everything but its
 * first and last element. */
double average(int *salary, int salarySize)
{
    int *s = malloc((size_t)salarySize * sizeof *s), i, k, t;
    long long sum = 0;

    for (i = 0; i < salarySize; i++)
        s[i] = salary[i];
    for (i = 0; i < salarySize; i++)
        for (k = i + 1; k < salarySize; k++)
            if (s[k] < s[i]) {
                t = s[i];
                s[i] = s[k];
                s[k] = t;
            }
    for (i = 1; i < salarySize - 1; i++)
        sum += s[i];
    free(s);
    return (double)sum / (salarySize - 2);
}
