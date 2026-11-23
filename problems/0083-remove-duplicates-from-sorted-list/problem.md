id: 83
slug: remove-duplicates-from-sorted-list
title: Remove Duplicates from Sorted List
difficulty: easy
tags: linked-list
signature: ListNode deleteDuplicates(ListNode head)
---
The values along the linked list `head` never go down, so the nodes that
hold one value form a single unbroken run. Shorten every run to **one
node** by unlinking the others, and return the head of the shortened list.
Its values are then strictly increasing.

Lists in the examples are written from head to tail: `[2,5]` is a node
holding 2 followed by a node holding 5, and `[]` is an empty list (a null
head).

@examples

## Constraints

- The list has between `0` and `300` nodes.
- `-100 <= Node.val <= 100`
- The values are in non-decreasing order.
