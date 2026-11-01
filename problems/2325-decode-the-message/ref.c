/* Reference solution: one pass over key fills a 26-entry table, giving
 * each letter met for the first time the next plain letter; then each
 * letter of message is looked up in it, and spaces are copied. */
char *decodeMessage(char *key, char *message)
{
    char table[26] = {0}, next = 'a', *out;
    size_t len = strlen(message), i;

    for (; *key; key++)
        if (*key != ' ' && table[*key - 'a'] == 0)
            table[*key - 'a'] = next++;
    out = malloc(len + 1);
    for (i = 0; i < len; i++)
        out[i] = message[i] == ' ' ? ' ' : table[message[i] - 'a'];
    out[len] = '\0';
    return out;
}
