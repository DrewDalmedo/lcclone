/* Reference solution: read the two age digits at characters 11 and 12 of
 * each record, and count the ages above 60. */
int countSeniors(char **details, int detailsSize)
{
    int count = 0, i, age;

    for (i = 0; i < detailsSize; i++) {
        age = (details[i][11] - '0') * 10 + (details[i][12] - '0');
        if (age > 60)
            count++;
    }
    return count;
}
