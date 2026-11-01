id: 682
slug: baseball-game
title: Baseball Game
difficulty: easy
tags: stack, array, simulation
signature: int calPoints(string[] operations)
---
An adding machine prints numbers on a paper tape, one per line, each new
line below the ones before it. The tape starts out blank. Carry out the
strings of `operations` one at a time, from first to last; each is one of
four commands:

- an integer, such as `"42"`, `"0"` or `"-7"`: print that number;
- `"+"`: print the sum of the two numbers on the bottom two lines;
- `"D"`: print twice the number on the bottom line;
- `"C"`: cut off the bottom line, so that its number is gone.

The bottom line always means the bottom of the tape as it is at that
moment. `"+"` and `"D"` only read the numbers they use, which stay on the
tape. A `"C"` cuts off whatever line is at the bottom, even one that `"+"`
or `"D"` printed, and the line above it then becomes the bottom line, for
the next command to read or cut off in turn.

Return the sum of the numbers left on the tape after the last command, or
`0` if the tape is blank by then.

@examples

## Constraints

- `1 <= operations.length <= 1000`
- `operations[i]` is `"C"`, `"D"`, `"+"`, or an integer from `-3 * 10^4`
  to `3 * 10^4`, written in decimal with a `-` in front when it is
  negative and no leading zeros.
- Every `"+"` finds at least two lines on the tape, and every `"C"` and
  `"D"` at least one.
- At every moment, the absolute values of the numbers on the tape add up
  to at most `2^31 - 1`. So every number printed, any sum of numbers on
  the tape and the answer fit in a 32-bit signed integer.
