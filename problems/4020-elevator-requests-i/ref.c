/* Reference solution: add up the length of every leg, starting from
 * floor 0, in the order the requests come. */
int elevatorRequests(int n, int *requests, int requestsSize)
{
    int at = 0, total = 0, i, d;

    (void)n;
    for (i = 0; i < requestsSize; i++) {
        d = requests[i] - at;
        total += d < 0 ? -d : d;
        at = requests[i];
    }
    return total;
}
