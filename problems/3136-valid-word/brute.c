/* Slow and obviously right: test the four requirements one at a time, each
 * with its own scan and an explicit list of the characters it accepts. */
static bool every_char_in(const char *word, const char *set)
{
    size_t i;

    for (i = 0; word[i] != '\0'; i++)
        if (strchr(set, word[i]) == NULL)
            return false;
    return true;
}

static bool some_char_in(const char *word, const char *set)
{
    size_t i;

    for (i = 0; word[i] != '\0'; i++)
        if (strchr(set, word[i]) != NULL)
            return true;
    return false;
}

bool isValid(char *word)
{
    const char *vowels = "aeiouAEIOU";
    const char *consonants = "bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ";
    const char *allowed = "abcdefghijklmnopqrstuvwxyz"
                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    if (strlen(word) < 3)
        return false;
    if (!every_char_in(word, allowed))
        return false;
    if (!some_char_in(word, vowels))
        return false;
    return some_char_in(word, consonants);
}
