/* Slow and obviously right, by another method: for each kind, try every
 * character of that kind, written out in a list, and look for it in the
 * password; then compare every two positions that are next to each
 * other. */
static bool has_one_of(const char *password, const char *chars)
{
    for (; *chars; chars++)
        if (strchr(password, *chars))
            return true;
    return false;
}

bool strongPasswordCheckerII(char *password)
{
    static const char *kinds[4] = {
        "abcdefghijklmnopqrstuvwxyz",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        "0123456789",
        "!@#$%^&*()-+",
    };
    int n = (int)strlen(password), i, j, k;

    if (n < 8)
        return false;
    for (k = 0; k < 4; k++)
        if (!has_one_of(password, kinds[k]))
            return false;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (j == i + 1 && password[i] == password[j])
                return false;
    return true;
}
