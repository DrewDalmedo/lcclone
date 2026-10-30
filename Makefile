# lcclone: POSIX Makefile. Builds liblc.a (the core) and lc (the CLI).
#
#   make                  build liblc.a and lc
#   make test             unit tests and end-to-end tests
#   make install          install to $(DESTDIR)$(PREFIX)
#   make clean
#
# Override CC, CFLAGS, LDFLAGS and PREFIX on the command line, e.g.
#   make CFLAGS='-O0 -g -fsanitize=address,undefined' \
#        LDFLAGS='-fsanitize=address,undefined' test

.POSIX:
.SUFFIXES:
.SUFFIXES: .c .o

CC      = cc
AR      = ar
CFLAGS  = -O2 -Wall -Wextra -Wpedantic
LDFLAGS =
LDLIBS  =
PREFIX  = /usr/local
DESTDIR =
BINDIR  = $(PREFIX)/bin
DATADIR = $(PREFIX)/share/lcclone

# Flags the build always needs, kept apart from CFLAGS so that overriding
# CFLAGS doesn't drop them.
LC_CFLAGS = -std=c11 -D_FILE_OFFSET_BITS=64 -Iinclude -Isrc/core \
	-DLC_DATADIR=\"$(DATADIR)\"

CORE_OBJ = \
	src/core/arena.o \
	src/core/cases.o \
	src/core/ctx.o \
	src/core/fs.o \
	src/core/json.o \
	src/core/md.o \
	src/core/problem.o \
	src/core/str.o

CLI_OBJ = \
	src/cli/ansi.o \
	src/cli/cmd_list.o \
	src/cli/cmd_show.o \
	src/cli/main.o \
	src/cli/pager.o

UNIT_TESTS = \
	tests/unit/test_arena \
	tests/unit/test_cases \
	tests/unit/test_ctx \
	tests/unit/test_fs \
	tests/unit/test_json \
	tests/unit/test_md \
	tests/unit/test_problem \
	tests/unit/test_str

all: liblc.a lc

.c.o:
	$(CC) $(LC_CFLAGS) $(CFLAGS) -c -o $@ $<

$(CORE_OBJ): include/lc.h src/core/internal.h
$(CLI_OBJ): include/lc.h src/cli/cli.h

# problem.c and test_problem.c have DATADIR compiled in. .datadir holds the
# value they were built with and is rewritten only when DATADIR changes, so
# "make PREFIX=..." rebuilds what it must and nothing more.
#
# A make that compares mtimes to the second (bmake, the system make of
# FreeBSD and NetBSD, or any make on a filesystem with 1-second timestamps)
# would see a .datadir rewritten in the same second as the last build as no
# newer than the objects, keep the old DATADIR and never notice again. So
# when the value changes, everything built from it is deleted (bmake checks
# a target again when it gets to it, and rebuilds it if it is missing), and
# the new .datadir is dated at least a second after the last build (for a
# make, like GNU make, that reads a target's mtime before making its
# prerequisites). A first build doesn't wait.
DATADIR_OUT = src/core/problem.o tests/unit/test_problem.o liblc.a lc \
	$(UNIT_TESTS)
src/core/problem.o tests/unit/test_problem.o: .datadir
.datadir: FORCE
	@echo '$(DATADIR)' > .datadir.tmp; \
	if cmp -s .datadir.tmp .datadir; then rm -f .datadir.tmp; else \
		rm -f $(DATADIR_OUT); \
		if [ -f .datadir ]; then sleep 1; touch .datadir.tmp; fi; \
		mv .datadir.tmp .datadir; \
	fi
FORCE:

liblc.a: $(CORE_OBJ)
	rm -f $@
	$(AR) -rc $@ $(CORE_OBJ)
	-ranlib $@

lc: $(CLI_OBJ) liblc.a
	$(CC) $(LDFLAGS) -o $@ $(CLI_OBJ) liblc.a $(LDLIBS)

# Unit tests: one program per suite, each linked against liblc.a.
tests/unit/test_arena.o tests/unit/test_cases.o tests/unit/test_ctx.o \
tests/unit/test_fs.o tests/unit/test_json.o tests/unit/test_md.o \
tests/unit/test_problem.o tests/unit/test_str.o: \
	include/lc.h src/core/internal.h tests/unit/test.h
# These two #include core sources (to wrap malloc), so they carry their own
# copies of those modules and never use the ones in liblc.a.
tests/unit/test_fs.o: src/core/arena.c src/core/str.c src/core/ctx.c \
	src/core/fs.c
tests/unit/test_md.o: src/core/arena.c src/core/str.c src/core/md.c

tests/unit/test_arena: tests/unit/test_arena.o liblc.a
	$(CC) $(LDFLAGS) -o $@ tests/unit/test_arena.o liblc.a $(LDLIBS)
tests/unit/test_cases: tests/unit/test_cases.o liblc.a
	$(CC) $(LDFLAGS) -o $@ tests/unit/test_cases.o liblc.a $(LDLIBS)
tests/unit/test_ctx: tests/unit/test_ctx.o liblc.a
	$(CC) $(LDFLAGS) -o $@ tests/unit/test_ctx.o liblc.a $(LDLIBS)
tests/unit/test_fs: tests/unit/test_fs.o liblc.a
	$(CC) $(LDFLAGS) -o $@ tests/unit/test_fs.o liblc.a $(LDLIBS)
tests/unit/test_json: tests/unit/test_json.o liblc.a
	$(CC) $(LDFLAGS) -o $@ tests/unit/test_json.o liblc.a $(LDLIBS)
tests/unit/test_md: tests/unit/test_md.o liblc.a
	$(CC) $(LDFLAGS) -o $@ tests/unit/test_md.o liblc.a $(LDLIBS)
tests/unit/test_problem: tests/unit/test_problem.o liblc.a
	$(CC) $(LDFLAGS) -o $@ tests/unit/test_problem.o liblc.a $(LDLIBS)
tests/unit/test_str: tests/unit/test_str.o liblc.a
	$(CC) $(LDFLAGS) -o $@ tests/unit/test_str.o liblc.a $(LDLIBS)

check: test
test: all $(UNIT_TESTS)
	@fail=0; \
	for t in $(UNIT_TESTS); do ./$$t || fail=1; done; \
	sh tests/e2e.sh ./lc || fail=1; \
	if [ $$fail -ne 0 ]; then echo 'make test: FAILED'; exit 1; fi; \
	echo 'make test: all passed'

# The installed files must be usable by everyone whatever the umask of the
# checkout or of the installer: directories install creates get mode 755,
# and the shipped packs are made world-readable (only those: the data
# directory can also hold your own packs).
install: all
	umask 022; mkdir -p $(DESTDIR)$(BINDIR) $(DESTDIR)$(DATADIR)/problems
	cp lc $(DESTDIR)$(BINDIR)/lc
	chmod 755 $(DESTDIR)$(BINDIR)/lc
	cp -R problems/. $(DESTDIR)$(DATADIR)/problems/
	for d in problems/*/; do \
		chmod -R a+rX "$(DESTDIR)$(DATADIR)/problems/`basename "$$d"`"; \
	done

# Removes only what install added: the binary and the packs shipped in
# problems/. With PREFIX=~/.local the data directory is also where your own
# packs and progress live, so it is never removed wholesale.
uninstall:
	rm -f $(DESTDIR)$(BINDIR)/lc
	for d in problems/*/; do \
		rm -rf "$(DESTDIR)$(DATADIR)/problems/`basename "$$d"`"; \
	done
	-rmdir "$(DESTDIR)$(DATADIR)/problems" "$(DESTDIR)$(DATADIR)" 2>/dev/null

clean:
	rm -f liblc.a lc $(CORE_OBJ) $(CLI_OBJ) $(UNIT_TESTS) tests/unit/*.o
	rm -f .datadir .datadir.tmp
	rm -rf tests/tmp

.PHONY: all check test install uninstall clean FORCE
