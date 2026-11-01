// Free functions named like POSIX functions from <unistd.h>. nice, sleep
// and pause have the very types <unistd.h> gives them, which with C++
// linkage would clash with its declarations.
int nice(int a) { return a; }
unsigned sleep(unsigned s) { return s; }
int pause() { return 0; }
long write(long a, long b) { return a + b; }
int read() { return 0; }

class Solution {
public:
    int add(int a, int b) {
        return nice((int)write(a, b)) + (int)sleep(0) + pause() + read();
    }
};
