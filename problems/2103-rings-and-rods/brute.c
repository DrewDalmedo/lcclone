/* Slow and obviously right: for each rod and each colour, read the whole
 * log looking for a ring of that colour on that rod, and count the rods
 * where all three searches succeed. */
int countPoints(char *rings)
{
    const char *colours = "RGB";
    size_t len = strlen(rings), i;
    int rod, c, found, all, complete = 0;

    for (rod = 0; rod < 10; rod++) {
        all = 1;
        for (c = 0; c < 3; c++) {
            found = 0;
            for (i = 0; i + 1 < len; i += 2)
                if (rings[i] == colours[c] && rings[i + 1] == '0' + rod)
                    found = 1;
            if (!found)
                all = 0;
        }
        complete += all;
    }
    return complete;
}
