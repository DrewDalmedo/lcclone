/* Reference solution: count the students whose closed interval
 * [startTime[i], endTime[i]] holds queryTime. */
int busyStudent(int *startTime, int startTimeSize, int *endTime,
                int endTimeSize, int queryTime)
{
    int count = 0, i;

    (void)endTimeSize;
    for (i = 0; i < startTimeSize; i++)
        if (startTime[i] <= queryTime && queryTime <= endTime[i])
            count++;
    return count;
}
