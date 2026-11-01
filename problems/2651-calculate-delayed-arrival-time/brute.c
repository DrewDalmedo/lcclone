/* Slow and obviously right: move the clock forward one hour at a time,
 * going from 23 back to 0. */
int findDelayedArrivalTime(int arrivalTime, int delayedTime)
{
    int hour = arrivalTime, i;

    for (i = 0; i < delayedTime; i++)
        hour = hour == 23 ? 0 : hour + 1;
    return hour;
}
