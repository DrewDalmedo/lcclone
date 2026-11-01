/* Slow and obviously right: walk along a copy of the charges and do what
 * the technician does, draining every gadget to the right of each tested
 * one (never below 0). */
int countTestedDevices(int *batteryPercentages, int batteryPercentagesSize)
{
    int n = batteryPercentagesSize, tested = 0, i, j;
    int *charge = malloc((size_t)n * sizeof *charge);

    for (i = 0; i < n; i++)
        charge[i] = batteryPercentages[i];
    for (i = 0; i < n; i++) {
        if (charge[i] <= 0)
            continue;
        tested++;
        for (j = i + 1; j < n; j++)
            if (charge[j] > 0)
                charge[j]--;
    }
    free(charge);
    return tested;
}
