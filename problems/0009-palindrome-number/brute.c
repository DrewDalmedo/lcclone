/* Slow and obviously right: write x out in decimal, as text, and compare
 * the text with itself read backwards. */
bool isPalindrome(int x)
{
    char s[16];
    int n = snprintf(s, sizeof s, "%d", x), i;

    for (i = 0; i < n / 2; i++)
        if (s[i] != s[n - 1 - i])
            return false;
    return true;
}
