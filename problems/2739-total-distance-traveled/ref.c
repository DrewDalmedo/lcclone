/* Reference solution: the first transfer comes after 5 liters of the
 * original fuel, and each later one after 4 more, since 5 liters are burned
 * and 1 comes back. So min(additionalTank, (mainTank - 1) / 4) transfers
 * happen, and every liter that ever reaches the main tank gives 10 km. */
int distanceTraveled(int mainTank, int additionalTank)
{
    int transfers = (mainTank - 1) / 4;

    if (transfers > additionalTank)
        transfers = additionalTank;
    return 10 * (mainTank + transfers);
}
