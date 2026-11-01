/* Reference solution: work out the volume in 64 bits, where it can't
 * overflow, decide bulky and heavy, and copy the matching label into
 * malloc'ed memory. */
char *categorizeBox(int length, int width, int height, int mass)
{
    long long volume = (long long)length * width * height;
    int bulky = length >= 10000 || width >= 10000 || height >= 10000
                || volume >= 1000000000LL;
    int heavy = mass >= 100;
    const char *label;
    char *out;

    if (bulky && heavy)
        label = "Both";
    else if (bulky)
        label = "Bulky";
    else if (heavy)
        label = "Heavy";
    else
        label = "Neither";
    out = malloc(strlen(label) + 1);
    strcpy(out, label);
    return out;
}
