id: 19
slug: missing
title: Missing
difficulty: easy
signature: int missing(int[] nums)
time_limit_ms: 200
---
A fixture for the judge tests: `nums` holds, in increasing order, every
number from 0 to n but one, n being its length; return the one left out.
`past.c` and `past.cpp` read one element past the end of `nums`, which
only the sanitizers notice.
