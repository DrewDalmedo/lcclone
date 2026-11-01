id: 3386
slug: button-with-longest-push-time
title: Button with Longest Push Time
difficulty: easy
tags: array
signature: int buttonWithLongestTime(int[][] events)
---
A control panel is switched on at second `0`. Its buttons carry positive
whole-number labels, and it records every push: row `events[i] = [index, time]`
of the record says that push `i` was on the button labelled `index`, at
second `time`. The rows are in the order the pushes happened.

The **push time** of a push is how long the panel sat untouched before it
came:

- for push `0`, the seconds since the panel was switched on, which is
  `events[0][1]`;
- for any later push `i`, the seconds since push `i - 1`, which is
  `events[i][1] - events[i - 1][1]`, whichever buttons the two pushes were
  on.

Return the label of the button whose push has the greatest push time. When
several pushes share the greatest push time, return the smallest label among
their buttons, wherever those pushes are in the record. The comparison is
push against push: what one button's pushes add up to over the whole record
plays no part.

@examples

## Constraints

- `1 <= events.length <= 1000`
- `events[i].length == 2`
- `1 <= events[i][0] <= 10^5`
- `1 <= events[i][1] <= 10^5`
- `events[i][1] < events[i + 1][1]`
