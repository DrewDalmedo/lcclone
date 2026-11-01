/* Reference solution: a link "xy" of the backwards string is a link "yx"
 * of s itself. Mark every link of s in a 26 x 26 table, then look for a
 * link whose letters swapped are marked too (a doubled letter is its own
 * swap). */
bool isSubstringPresent(char *s)
{
    bool seen[26][26] = {{false}};
    size_t i;

    if (!s[0])
        return false;
    for (i = 0; s[i + 1]; i++)
        seen[s[i] - 'a'][s[i + 1] - 'a'] = true;
    for (i = 0; s[i + 1]; i++)
        if (seen[s[i + 1] - 'a'][s[i] - 'a'])
            return true;
    return false;
}
