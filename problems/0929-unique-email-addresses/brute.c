/* Slow and obviously right: follow the three steps one at a time on copies
 * (split at the '@', cut the user part at its first '+', then delete its
 * dots), and count each mailbox when no earlier entry gave the same one. */
static void mailbox(const char *addr, char *out)
{
    char user[101];
    const char *at = strchr(addr, '@');
    size_t ulen = (size_t)(at - addr), i, k = 0;
    char *plus;

    memcpy(user, addr, ulen);
    user[ulen] = '\0';
    plus = strchr(user, '+');
    if (plus)
        *plus = '\0';
    for (i = 0; user[i] != '\0'; i++)
        if (user[i] != '.')
            out[k++] = user[i];
    out[k] = '\0';
    strcat(out, at);
}

int numUniqueEmails(char **emails, int emailsSize)
{
    char boxes[100][101];
    int count = 0, i, j, seen;

    for (i = 0; i < emailsSize; i++) {
        mailbox(emails[i], boxes[i]);
        seen = 0;
        for (j = 0; j < i; j++)
            if (strcmp(boxes[i], boxes[j]) == 0)
                seen = 1;
        if (!seen)
            count++;
    }
    return count;
}
