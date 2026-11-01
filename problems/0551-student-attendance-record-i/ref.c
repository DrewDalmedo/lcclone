/* Reference solution: one pass, counting the absences so far and the late
 * days in a row up to the current day (a 'P' or an 'A' sets that count back
 * to 0). It stops as soon as either rule is broken. */
bool checkRecord(char *s)
{
    int absent = 0, late = 0;

    for (; *s; s++) {
        if (*s == 'L') {
            if (++late == 3)
                return false;
        } else {
            late = 0;
            if (*s == 'A' && ++absent == 2)
                return false;
        }
    }
    return true;
}
