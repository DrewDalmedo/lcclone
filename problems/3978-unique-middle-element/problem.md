id: 3978
slug: unique-middle-element
title: Unique Middle Element
difficulty: easy
tags: array, counting
signature: bool isMiddleElementUnique(int[] nums)
---
`nums` has an odd number of entries, so exactly one position in it has as
many entries on its left as on its right. Call it the **centre**: with
`n = nums.length` and positions counted from `0`, it is position
`(n - 1) / 2`. Positions follow the order in which `nums` is given; the
array is not sorted first.

Return `true` if the value at the centre is held by no other position of
`nums`, and `false` if some other position holds it too, however many
places away from the centre that position is.

Only the centre's value matters: the other entries may repeat among
themselves as much as they like.

@examples

## Constraints

- `1 <= nums.length <= 100`
- `nums.length` is odd.
- `1 <= nums[i] <= 100`
