/* Slow and obviously right, by another method: read the four numbers of
 * the address and print them again with "[.]" between them. (This relies
 * on the address being written without leading zeros, as it always is.) */
char *defangIPaddr(char *address)
{
    int a, b, c, d;
    char *out = malloc(32);

    sscanf(address, "%d.%d.%d.%d", &a, &b, &c, &d);
    snprintf(out, 32, "%d[.]%d[.]%d[.]%d", a, b, c, d);
    return out;
}
