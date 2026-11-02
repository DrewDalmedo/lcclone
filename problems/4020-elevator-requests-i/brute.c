/* Slow and obviously right: move the elevator one floor at a time and
 * count a second for each step. */
int elevatorRequests(int n, int *requests, int requestsSize)
{
    int at = 0, seconds = 0, i;

    for (i = 0; i < requestsSize; i++) {
        while (at != requests[i]) {
            at += at < requests[i] ? 1 : -1;
            if (at < 0 || at >= n)
                return -1;
            seconds++;
        }
    }
    return seconds;
}
