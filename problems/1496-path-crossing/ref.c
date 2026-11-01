/* Reference solution: walk the route and keep every corner reached in a
 * hash set (open addressing). Coordinates stay within [-10^4, 10^4], so
 * (x + 10^4) * 20001 + (y + 10^4) is a distinct key for each corner, and
 * it fits in an int. */
#define PC_SLOTS 32768 /* a power of two, more than twice 10^4 + 1 */

static int pc_insert(int *table, int key)
{
    unsigned h = ((unsigned)key * 2654435761u) & (PC_SLOTS - 1);

    while (table[h] != -1) {
        if (table[h] == key)
            return 0;
        h = (h + 1) & (PC_SLOTS - 1);
    }
    table[h] = key;
    return 1;
}

bool isPathCrossing(char *path)
{
    int *table = malloc(PC_SLOTS * sizeof *table);
    int x = 0, y = 0, i;
    bool crossed = false;

    for (i = 0; i < PC_SLOTS; i++)
        table[i] = -1;
    pc_insert(table, 10000 * 20001 + 10000);
    for (i = 0; path[i] && !crossed; i++) {
        switch (path[i]) {
        case 'N': y++; break;
        case 'S': y--; break;
        case 'E': x++; break;
        default:  x--; break;
        }
        if (!pc_insert(table, (x + 10000) * 20001 + (y + 10000)))
            crossed = true;
    }
    free(table);
    return crossed;
}
