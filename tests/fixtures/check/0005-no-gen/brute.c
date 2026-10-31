/* One step at a time. */
int add(int a, int b)
{
    int s = a, i;

    for (i = 0; i < b; i++)
        s++;
    for (i = 0; i > b; i--)
        s--;
    return s;
}
