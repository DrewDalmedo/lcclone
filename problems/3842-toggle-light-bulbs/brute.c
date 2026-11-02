/* Slow and obviously right: keep a list of the bulbs that are on. Each
 * entry is looked for in the list and removed if it is there, appended if
 * not. At the end, sort the list with a simple exchange sort. */
int *toggleLightBulbs(int *bulbs, int bulbsSize, int *returnSize)
{
    int *list = malloc(100 * sizeof *list);
    int n = 0, i, j, found, t;

    for (i = 0; i < bulbsSize; i++) {
        found = -1;
        for (j = 0; j < n; j++)
            if (list[j] == bulbs[i])
                found = j;
        if (found >= 0) {
            for (j = found; j + 1 < n; j++)
                list[j] = list[j + 1];
            n--;
        } else {
            list[n++] = bulbs[i];
        }
    }
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++)
            if (list[j] < list[i]) {
                t = list[i];
                list[i] = list[j];
                list[j] = t;
            }
    *returnSize = n;
    return list;
}
