/* Reference solution: greedy by units per box. Since units_i is at most
 * 1000, add up how many boxes there are of each units value (a counting
 * sort), then fill the truck from the largest value down, taking a whole
 * value's boxes at once, or as many of them as still fit. */
int maximumUnits(int **boxTypes, int boxTypesSize, int *boxTypesColSize,
                 int truckSize)
{
    int boxes[1001] = {0};
    int left = truckSize, total = 0, units, take, i;

    (void)boxTypesColSize;
    for (i = 0; i < boxTypesSize; i++)
        boxes[boxTypes[i][1]] += boxTypes[i][0];
    for (units = 1000; units >= 1 && left > 0; units--) {
        take = boxes[units] < left ? boxes[units] : left;
        total += take * units;
        left -= take;
    }
    return total;
}
