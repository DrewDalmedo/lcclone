/* Reference solution: count the letters of magazine, then spend one count
 * for each letter of the note; a count going below zero means that letter
 * ran out. */
bool canConstruct(char *ransomNote, char *magazine)
{
    int count[26] = {0};
    const char *p;

    for (p = magazine; *p; p++)
        count[*p - 'a']++;
    for (p = ransomNote; *p; p++)
        if (--count[*p - 'a'] < 0)
            return false;
    return true;
}
