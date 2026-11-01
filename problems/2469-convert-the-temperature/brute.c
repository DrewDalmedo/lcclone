/* A second route to the same numbers: count in exact whole hundredths of
 * a degree (celsius has at most two decimals), so kelvin is
 * (c100 + 27315) / 100 and fahrenheit (180 * c100 + 320000) / 10000. */
double *convertTemperature(double celsius, int *returnSize)
{
    double *out = malloc(2 * sizeof *out);
    long long c100 = (long long)(celsius * 100 + 0.5);

    out[0] = (double)(c100 + 27315) / 100;
    out[1] = (double)(180 * c100 + 320000) / 10000;
    *returnSize = 2;
    return out;
}
