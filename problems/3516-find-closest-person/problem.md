id: 3516
slug: find-closest-person
title: Find Closest Person
difficulty: easy
tags: math
signature: int findClosest(int x, int y, int z)
---
Three people stand at whole-number positions along a straight line: person
1 at `x`, person 2 at `y` and person 3 at `z`. How far apart two of them
are is the absolute difference of their positions, so person 1 is
`|x - z|` from person 3 and person 2 is `|y - z|` from person 3.

Return `1` when person 1 is strictly nearer to person 3 than person 2 is,
`2` when person 2 is strictly nearer, and `0` when both are the same
distance away.

You can also see it as a race. Person 3 waits in place while persons 1 and
2 both head straight for them, at the same speed, so whoever has less
ground to cover gets there first, and equal distances mean they arrive
together. It doesn't matter which side of person 3 each of them starts on,
and anyone already standing at `z` has arrived at once. Several people, or
all three, may share a position.

@examples

## Constraints

- `1 <= x, y, z <= 100`
