/* Reference solution, in closed form. Each trip to the machine turns
 * numExchange empties into one bottle that is drunk and becomes an empty
 * again, so it costs numExchange - 1 empties net. One empty must be left
 * at the end (the machine can't take the last one), so the numBottles
 * empties pay for (numBottles - 1) / (numExchange - 1) extra bottles. */
int numWaterBottles(int numBottles, int numExchange)
{
    return numBottles + (numBottles - 1) / (numExchange - 1);
}
