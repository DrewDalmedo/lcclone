/* Reference solution: the k-th seat from the left goes to the k-th
 * student from the left (a crossing pair can always be uncrossed without
 * costing more). Counting sort both lists, positions 1 to 100, and walk
 * them together, adding up the distances. */
int minMovesToSeat(int *seats, int seatsSize, int *students,
                   int studentsSize)
{
    int seat_at[101] = {0}, student_at[101] = {0};
    int total = 0, p = 0, q = 0, k;

    for (k = 0; k < seatsSize; k++)
        seat_at[seats[k]]++;
    for (k = 0; k < studentsSize; k++)
        student_at[students[k]]++;
    for (k = 0; k < seatsSize; k++) {
        while (seat_at[p] == 0)
            p++;
        while (student_at[q] == 0)
            q++;
        seat_at[p]--;
        student_at[q]--;
        total += p > q ? p - q : q - p;
    }
    return total;
}
