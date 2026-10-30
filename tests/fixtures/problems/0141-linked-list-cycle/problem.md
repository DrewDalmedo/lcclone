id: 141
slug: linked-list-cycle
title: Linked List Cycle
difficulty: easy
tags: linked-list, two-pointers, hash-table
signature: bool hasCycle(ListNode head)
hidden: int pos
---
You get `head`, the first node of a singly linked list. Follow the `next`
pointers from `head` and decide whether the walk ever returns to a node it has
already visited. Return `true` if it does, or `false` if the walk reaches the
end of the list.

Each test builds the list from an array of values. The hidden input `pos` is
the index of the node that the last node links back to, or `-1` when the last
node's `next` is `NULL`. Your function sees only `head`.

@examples

## Constraints

- The list has between `0` and `5000` nodes.
- `-1000 <= Node.val <= 1000`
- `pos` is `-1` or a valid index into the list.

Bonus: use only a constant amount of extra memory.\
Hint: two pointers that move at different speeds.
