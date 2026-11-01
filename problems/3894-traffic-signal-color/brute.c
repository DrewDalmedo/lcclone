/* Slow and obviously right: paint a table of all 1001 readings, Invalid
 * first, then the Red stretch, then the two single readings, and look the
 * timer up. */
char *trafficSignal(int timer)
{
    static const char *words[] = {"Invalid", "Green", "Orange", "Red"};
    int table[1001], t;

    for (t = 0; t <= 1000; t++)
        table[t] = 0;
    for (t = 31; t <= 90; t++)
        table[t] = 3;
    table[30] = 2;
    table[0] = 1;
    return (char *)words[table[timer]];
}
