/* Slow and obviously right: try every place for the cut, and check that
 * nothing to its left is a b and nothing to its right is an a. */
bool checkString(char *s)
{
    int n = (int)strlen(s), cut, i, good;

    for (cut = 0; cut <= n; cut++) {
        good = 1;
        for (i = 0; i < n; i++)
            if ((i < cut && s[i] == 'b') || (i >= cut && s[i] == 'a'))
                good = 0;
        if (good)
            return true;
    }
    return false;
}
