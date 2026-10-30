long long* lens(int* a, int aSize, long long* b, int bSize, uint32_t* c, int cSize, double* d, int dSize, bool* e, int eSize, char* f, int fSize, char** g, int gSize, int* returnSize) {
    long long *out = malloc(10 * sizeof *out), sa = 0, sb = 0, te = 0, tg = 0;
    int i;
    for (i = 0; i < aSize; i++) sa += a[i];
    for (i = 0; i < bSize; i++) sb += b[i];
    for (i = 0; i < eSize; i++) te += e[i];
    for (i = 0; i < gSize; i++) tg += (long long)strlen(g[i]);
    out[0] = aSize; out[1] = sa; out[2] = bSize; out[3] = sb; out[4] = cSize;
    out[5] = dSize; out[6] = eSize; out[7] = fSize; out[8] = gSize; out[9] = tg + te;
    (void)c; (void)d; (void)f;
    *returnSize = 10;
    return out;
}
