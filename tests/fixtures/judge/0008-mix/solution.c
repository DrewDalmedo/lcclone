char* mix(int a, long long b, uint32_t c, double d, bool e, char f, char* g) {
    char *out = malloc(256 + strlen(g));
    sprintf(out, "%d|%lld|%u|%g|%d|%c|%s", a, b, (unsigned)c, d, e ? 1 : 0, f, g);
    return out;
}
