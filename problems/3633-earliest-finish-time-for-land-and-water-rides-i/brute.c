/* Slow and obviously right: every land ride with every water ride, in
 * both orders, also trying every start time of the first ride from its
 * opening up to the latest one that could matter (3000). */
int earliestFinishTime(int *landStartTime, int landStartTimeSize,
                       int *landDuration, int landDurationSize,
                       int *waterStartTime, int waterStartTimeSize,
                       int *waterDuration, int waterDurationSize)
{
    int best = INT_MAX, i, j, t, second, finish;

    (void)landDurationSize;
    (void)waterDurationSize;
    for (i = 0; i < landStartTimeSize; i++)
        for (j = 0; j < waterStartTimeSize; j++) {
            /* Land ride i first, boarded at time t. */
            for (t = landStartTime[i]; t <= 3000; t++) {
                second = t + landDuration[i];
                if (second < waterStartTime[j])
                    second = waterStartTime[j];
                finish = second + waterDuration[j];
                if (finish < best)
                    best = finish;
            }
            /* Water ride j first, boarded at time t. */
            for (t = waterStartTime[j]; t <= 3000; t++) {
                second = t + waterDuration[j];
                if (second < landStartTime[i])
                    second = landStartTime[i];
                finish = second + landDuration[i];
                if (finish < best)
                    best = finish;
            }
        }
    return best;
}
