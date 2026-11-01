/* Reference solution: two indices, i in name and j in typed. Each
 * character of typed either matches name[i] (then both move on) or repeats
 * the character before it in typed (an extra copy, skipped). Anything else
 * fails. At the end all of name must have been matched. */
bool isLongPressedName(char *name, char *typed)
{
    size_t i = 0, j;

    for (j = 0; typed[j] != '\0'; j++) {
        if (name[i] != '\0' && name[i] == typed[j])
            i++;
        else if (j == 0 || typed[j] != typed[j - 1])
            return false;
    }
    return name[i] == '\0';
}
