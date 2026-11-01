/* Reference solution: the line goes round until someone wants the top
 * sandwich, so only how many students want each kind matters. Go down the
 * pile using up those counts; the first sandwich whose kind nobody left
 * wants stops everything, and every sandwich from there down is left over,
 * one per student still waiting. */
int countStudents(int *students, int studentsSize, int *sandwiches,
                  int sandwichesSize)
{
    int want[2] = {0, 0}, i;

    for (i = 0; i < studentsSize; i++)
        want[students[i]]++;
    for (i = 0; i < sandwichesSize; i++) {
        if (want[sandwiches[i]] == 0)
            return sandwichesSize - i;
        want[sandwiches[i]]--;
    }
    return 0;
}
