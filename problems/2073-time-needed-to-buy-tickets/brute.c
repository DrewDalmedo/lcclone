/* Slow and obviously right: run the window second by second with a
 * circular queue of the fans still waiting, until fan k is done. */
int timeRequiredToBuy(int *tickets, int ticketsSize, int k)
{
    int queue[100], left[100], head = 0, count = ticketsSize, seconds = 0;
    int i, fan;

    for (i = 0; i < ticketsSize; i++) {
        queue[i] = i;
        left[i] = tickets[i];
    }
    for (;;) {
        fan = queue[head];
        head = (head + 1) % ticketsSize;
        count--;
        seconds++;
        left[fan]--;
        if (left[fan] == 0 && fan == k)
            return seconds;
        if (left[fan] > 0) {
            queue[(head + count) % ticketsSize] = fan;
            count++;
        }
    }
}
