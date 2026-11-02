/* Reference solution: move two indices in from both ends while their
 * characters match. At the first mismatch one of the two must be deleted,
 * so the answer is whether what lies between them is a palindrome once
 * the left one, or the right one, is left out. */
static bool between_is_palindrome(const char *s, size_t i, size_t j)
{
    while (i < j)
        if (s[i++] != s[j--])
            return false;
    return true;
}

bool validPalindrome(char *s)
{
    size_t i = 0, j = strlen(s) - 1;

    while (i < j && s[i] == s[j]) {
        i++;
        j--;
    }
    if (i >= j)
        return true;
    return between_is_palindrome(s, i + 1, j)
        || between_is_palindrome(s, i, j - 1);
}
