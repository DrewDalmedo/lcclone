id: 2525
slug: categorize-box-according-to-criteria
title: Categorize Box According to Criteria
difficulty: easy
tags: math
signature: string categorizeBox(int length, int width, int height, int mass)
---
A parcel depot sends every box down one of four chutes, labelled
`"Both"`, `"Bulky"`, `"Heavy"` and `"Neither"`. Each box arrives with the
lengths of its three sides, `length`, `width` and `height`, and its
`mass`.

A box is **bulky** when at least one of its sides is `10^4` or longer, or
when its volume, the product `length * width * height`, is `10^9` or more.
Either reason is enough on its own. A box is **heavy** when its `mass` is
`100` or more.

Return the label of the chute the box goes down:

- `"Both"` if it is bulky and heavy;
- `"Bulky"` if it is bulky but not heavy;
- `"Heavy"` if it is heavy but not bulky;
- `"Neither"` if it is neither bulky nor heavy.

Each label must match the spelling above letter for letter, starting with
a capital. The volume is the exact product of the three sides: it can reach `10^15`, far
more than a 32-bit `int` holds.

@examples

## Constraints

- `1 <= length, width, height <= 10^5`
- `1 <= mass <= 10^3`
