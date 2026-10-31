/* Reference solution: one pass for the sum, the smallest and the largest
 * salary; take the two extremes out of the sum and divide by n - 2. */
double average(int *salary, int salarySize)
{
    long long sum = 0;
    int lo = salary[0], hi = salary[0], i;

    for (i = 0; i < salarySize; i++) {
        sum += salary[i];
        if (salary[i] < lo)
            lo = salary[i];
        if (salary[i] > hi)
            hi = salary[i];
    }
    return (double)(sum - lo - hi) / (salarySize - 2);
}
