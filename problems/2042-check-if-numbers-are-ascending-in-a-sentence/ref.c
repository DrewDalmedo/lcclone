/* Reference solution: one pass over s, building the value of each run of
 * digits; when a run ends (at a space or at the end of s), compare its
 * value with the previous number's. Numbers are at least 1, so 0 is a safe
 * starting value for "the previous number". */
bool areNumbersAscending(char *s)
{
    int prev = 0, value = 0, in_number = 0, i;

    for (i = 0;; i++) {
        if (s[i] >= '0' && s[i] <= '9') {
            value = value * 10 + (s[i] - '0');
            in_number = 1;
        } else {
            if (in_number) {
                if (value <= prev)
                    return false;
                prev = value;
                value = 0;
                in_number = 0;
            }
            if (s[i] == '\0')
                return true;
        }
    }
}
