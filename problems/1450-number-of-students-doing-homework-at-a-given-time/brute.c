/* Slow and obviously right: walk through each student's minutes one by
 * one, from the first to the last, and count the student if one of them is
 * queryTime. */
int busyStudent(int *startTime, int startTimeSize, int *endTime,
                int endTimeSize, int queryTime)
{
    int count = 0, i, t, seen;

    (void)endTimeSize;
    for (i = 0; i < startTimeSize; i++) {
        seen = 0;
        for (t = startTime[i]; t <= endTime[i]; t++)
            if (t == queryTime)
                seen = 1;
        count += seen;
    }
    return count;
}
