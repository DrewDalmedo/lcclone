id: 203
slug: remove-linked-list-elements
title: Remove Linked List Elements
difficulty: easy
tags: linked-list, recursion
signature: ListNode removeElements(ListNode head, int val)
---
Cut every node holding the value `val` out of the linked list `head`, leave
the others in their order, and return the first node that survives. That
may not be the node you were given, since nodes at the front can be cut
too. If none survives, the result is the empty list.

Lists in the examples are written from head to tail: `[2,5]` is a node
holding 2 followed by a node holding 5, and `[]` is an empty list (a null
head).

@examples

## Constraints

- The list has between `0` and `10^4` nodes.
- `1 <= Node.val <= 50`
- `0 <= val <= 50`
