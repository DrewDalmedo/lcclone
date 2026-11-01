/* Reference solution: the two stats are trained separately. Energy only
 * falls, so it must start above the total of energy[]. For experience,
 * walk the challengers keeping the current experience, and whenever it is
 * not above experience[i], train just enough to get one above it; the
 * extra hours carry over to every later challenger. */
int minNumberOfHours(int initialEnergy, int initialExperience, int *energy,
                     int energySize, int *experience, int experienceSize)
{
    int total = 0, hours = 0, cur = initialExperience, i;

    (void)experienceSize;
    for (i = 0; i < energySize; i++)
        total += energy[i];
    if (initialEnergy <= total)
        hours += total + 1 - initialEnergy;
    for (i = 0; i < energySize; i++) {
        if (cur <= experience[i]) {
            hours += experience[i] + 1 - cur;
            cur = experience[i] + 1;
        }
        cur += experience[i];
    }
    return hours;
}
