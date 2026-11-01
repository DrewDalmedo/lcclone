/* Reference solution: walk the title word by word, in place. Lowercase
 * each word while measuring it, then raise its first letter if it has
 * three letters or more. */
char *capitalizeTitle(char *title)
{
    char *p = title, *start;

    while (*p != '\0') {
        start = p;
        while (*p != '\0' && *p != ' ') {
            *p = (char)tolower((unsigned char)*p);
            p++;
        }
        if (p - start >= 3)
            *start = (char)toupper((unsigned char)*start);
        if (*p == ' ')
            p++;
    }
    return title;
}
