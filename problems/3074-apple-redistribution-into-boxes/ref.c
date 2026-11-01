/* Reference solution: apples can be split freely, so only the totals
 * matter. Count the boxes of each capacity (1 to 50), then take boxes from
 * the largest capacity down until their room adds up to at least the number
 * of apples. */
int minimumBoxes(int *apple, int appleSize, int *capacity, int capacitySize)
{
    int count[51] = {0};
    int need = 0, boxes = 0, c, i;

    for (i = 0; i < appleSize; i++)
        need += apple[i];
    for (i = 0; i < capacitySize; i++)
        count[capacity[i]]++;
    for (c = 50; c >= 1; c--)
        while (count[c] > 0) {
            if (need <= 0)
                return boxes;
            need -= c;
            boxes++;
            count[c]--;
        }
    return boxes;
}
