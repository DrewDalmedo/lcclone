/* Slow and obviously right: burn one liter at a time, counting the liters
 * burned, and let the pump act at every multiple of 5. */
int distanceTraveled(int mainTank, int additionalTank)
{
    int burned = 0;

    while (mainTank > 0) {
        mainTank--;
        burned++;
        if (burned % 5 == 0 && additionalTank > 0) {
            additionalTank--;
            mainTank++;
        }
    }
    return 10 * burned;
}
