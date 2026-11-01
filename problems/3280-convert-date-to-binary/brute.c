/* Slow and obviously right: read the three numbers with sscanf, then for
 * each go through the powers of two from 4096 down to 1, writing a 1 where
 * the power fits into what is left (and taking it off), and a 0 where it
 * doesn't, once the first 1 has been written. */
static void append_binary(char *out, int v)
{
    size_t len = strlen(out);
    int p, started = 0;

    for (p = 4096; p >= 1; p /= 2) {
        if (v >= p) {
            out[len++] = '1';
            v -= p;
            started = 1;
        } else if (started) {
            out[len++] = '0';
        }
    }
    out[len] = '\0';
}

char *convertDateToBinary(char *date)
{
    char *out = calloc(64, 1);
    int year, month, day;

    sscanf(date, "%d-%d-%d", &year, &month, &day);
    append_binary(out, year);
    strcat(out, "-");
    append_binary(out, month);
    strcat(out, "-");
    append_binary(out, day);
    return out;
}
