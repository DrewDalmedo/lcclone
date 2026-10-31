/* Slow and obviously right: try all 2^10 ways to switch the 10 lights on
 * or off, add up the hour and minute each one shows, and keep the real
 * times with turnedOn lights on. */
char **readBinaryWatch(int turnedOn, int *returnSize)
{
    char **out = malloc(1024 * sizeof *out);
    int pattern, light, n = 0;

    for (pattern = 0; pattern < 1024; pattern++) {
        int on = 0, hour = 0, minute = 0;

        for (light = 0; light < 10; light++)
            if (pattern >> light & 1) {
                on++;
                if (light < 4)
                    hour += 1 << light;
                else
                    minute += 1 << (light - 4);
            }
        if (on == turnedOn && hour < 12 && minute < 60) {
            out[n] = malloc(16);
            sprintf(out[n], "%d:%02d", hour, minute);
            n++;
        }
    }
    *returnSize = n;
    return out;
}
