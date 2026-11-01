/* Reference solution: add the delay and take the sum modulo 24. */
int findDelayedArrivalTime(int arrivalTime, int delayedTime)
{
    return (arrivalTime + delayedTime) % 24;
}
