/* Slow and obviously right: go through the days one by one, raising the
 * deposit by 1 each day and starting each Monday at one more than the
 * Monday before. */
int totalMoney(int n)
{
    int total = 0, monday = 1, deposit = 1, day;

    for (day = 0; day < n; day++) {
        if (day > 0 && day % 7 == 0) {
            monday++;
            deposit = monday;
        }
        total += deposit;
        deposit++;
    }
    return total;
}
