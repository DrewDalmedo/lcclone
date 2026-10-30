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
LC_CFLAGS = -std=c11 -Iinclude -Isrc/core -DLC_DATADIR=\"$(DATADIR)\"

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

# problem.c has DATADIR compiled in. .datadir changes only when DATADIR
# does, so "make PREFIX=..." rebuilds what it must and nothing more.
src/core/problem.o: .datadir
.datadir: FORCE
	@echo '$(DATADIR)' > .datadir.tmp; \
	if cmp -s .datadir.tmp .datadir; then rm -f .datadir.tmp; \
	else mv .datadir.tmp .datadir; fi
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

install: all
	mkdir -p $(DESTDIR)$(BINDIR) $(DESTDIR)$(DATADIR)/problems
	cp lc $(DESTDIR)$(BINDIR)/lc
	chmod 755 $(DESTDIR)$(BINDIR)/lc
	cp -R problems/. $(DESTDIR)$(DATADIR)/problems/

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
