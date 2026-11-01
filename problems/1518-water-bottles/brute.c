/* Slow and obviously right: drink one bottle at a time, and recycle
 * whenever enough empties have piled up. */
int numWaterBottles(int numBottles, int numExchange)
{
    int full = numBottles, empty = 0, drunk = 0;

    while (full > 0 || empty >= numExchange) {
        if (full > 0) {
            full--;
            empty++;
            drunk++;
        } else {
            empty -= numExchange;
            full++;
        }
    }
    return drunk;
}
