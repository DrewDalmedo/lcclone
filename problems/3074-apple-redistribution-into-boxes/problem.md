id: 3074
slug: apple-redistribution-into-boxes
title: Apple Redistribution into Boxes
difficulty: easy
tags: greedy, array, sorting
signature: int minimumBoxes(int[] apple, int[] capacity)
---
After the harvest, the apples sit in bags: bag `i` holds `apple[i]` apples.
They are to be shipped in boxes instead, and there are boxes of several
sizes to choose from: box `j` has room for `capacity[j]` apples.

You pick some of the boxes and move every apple out of the bags into the
boxes you picked. A bag need not be emptied into a single box: its apples
can be spread over as many of the picked boxes as you like. A picked box
may be left partly empty, but it never holds more apples than its capacity.
Return the smallest number of boxes you can pick so that all the apples
fit.

The boxes together always have room for every apple, so some choice works.

@examples

## Constraints

- `1 <= apple.length, capacity.length <= 50`
- `1 <= apple[i], capacity[j] <= 50`
- The sum of `capacity` is at least the sum of `apple`.
