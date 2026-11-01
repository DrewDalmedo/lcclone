/* Reference solution: each test so far has drained 1 from every gadget
 * further right, so when the technician reaches gadget i after t tests,
 * its charge is max(0, b[i] - t). It is tested exactly when b[i] > t. */
int countTestedDevices(int *batteryPercentages, int batteryPercentagesSize)
{
    int tested = 0, i;

    for (i = 0; i < batteryPercentagesSize; i++)
        if (batteryPercentages[i] > tested)
            tested++;
    return tested;
}
