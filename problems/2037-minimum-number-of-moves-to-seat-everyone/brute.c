/* Slow and obviously right: try every way to give each student a seat of
 * their own, and keep the cheapest. Only for a few students. */
static int best;

static void place(const int *seats, const int *students, int n, int j,
                  int *taken, int cost)
{
    int i, d;

    if (cost >= best)
        return;
    if (j == n) {
        best = cost;
        return;
    }
    for (i = 0; i < n; i++)
        if (!taken[i]) {
            d = seats[i] - students[j];
            taken[i] = 1;
            place(seats, students, n, j + 1, taken, cost + (d < 0 ? -d : d));
            taken[i] = 0;
        }
}

int minMovesToSeat(int *seats, int seatsSize, int *students,
                   int studentsSize)
{
    int taken[100] = {0};

    (void)studentsSize;
    best = 1 << 30;
    place(seats, students, seatsSize, 0, taken, 0);
    return best;
}
