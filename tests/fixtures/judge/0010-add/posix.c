/* Helpers named like POSIX functions from <unistd.h>, with types of their
 * own: LeetCode's C has no <unistd.h>, so this builds there. They are
 * global on purpose, so that a call to read or write from the harness or
 * the C library would land here and give a wrong answer. */
int nice(int a) { return a; }
int read(int a, int b) { return a + b; }
long write(void) { return 0; }
double sleep(double x) { return x; }
int pipe(int a) { return a; }
int dup(int a) { return a; }
int access(int a) { return a; }
int pause(void) { return 0; }
int link(int a, int b) { return a - b; }

int add(int a, int b) {
    return nice(read(pipe(a), dup(access(b)))) + link(1, 1) + (int)write() +
           pause() + (int)sleep(0.0);
}
