/* Slow and obviously right: run the line as a ring buffer, step by step,
 * and stop when it is empty or when as many students in a row as there are
 * in the line have turned the top sandwich down. */
int countStudents(int *students, int studentsSize, int *sandwiches,
                  int sandwichesSize)
{
    int line[100], head = 0, size = studentsSize, top = 0, refused = 0, i;

    (void)sandwichesSize;
    for (i = 0; i < studentsSize; i++)
        line[i] = students[i];
    while (size > 0 && refused < size) {
        if (line[head] == sandwiches[top]) {
            head = (head + 1) % studentsSize;
            size--;
            top++;
            refused = 0;
        } else {
            line[(head + size) % studentsSize] = line[head];
            head = (head + 1) % studentsSize;
            refused++;
        }
    }
    return size;
}
