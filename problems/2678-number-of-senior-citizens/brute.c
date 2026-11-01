/* Slow and obviously right: write out every age from 61 to 99 as two
 * digits, and count the records whose age field equals one of them. */
int countSeniors(char **details, int detailsSize)
{
    char text[3];
    int count = 0, i, age;

    for (i = 0; i < detailsSize; i++)
        for (age = 61; age <= 99; age++) {
            snprintf(text, sizeof text, "%02d", age);
            if (strncmp(details[i] + 11, text, 2) == 0)
                count++;
        }
    return count;
}
