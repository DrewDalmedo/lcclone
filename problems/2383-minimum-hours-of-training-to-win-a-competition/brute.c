/* Slow and obviously right: for h = 0, 1, 2, ... hours, try every way of
 * splitting them between energy and experience, and play the whole
 * tournament with each split; return the first h for which one split
 * beats every challenger. */
static int wins(int en, int ex, int *energy, int *experience, int n)
{
    int i;

    for (i = 0; i < n; i++) {
        if (en <= energy[i] || ex <= experience[i])
            return 0;
        en -= energy[i];
        ex += experience[i];
    }
    return 1;
}

int minNumberOfHours(int initialEnergy, int initialExperience, int *energy,
                     int energySize, int *experience, int experienceSize)
{
    int h, a;

    (void)experienceSize;
    for (h = 0;; h++)
        for (a = 0; a <= h; a++)
            if (wins(initialEnergy + a, initialExperience + h - a, energy,
                     experience, energySize))
                return h;
}
