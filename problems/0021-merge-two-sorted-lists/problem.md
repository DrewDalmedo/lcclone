id: 21
slug: merge-two-sorted-lists
title: Merge Two Sorted Lists
difficulty: easy
tags: linked-list, recursion
signature: ListNode mergeTwoLists(ListNode list1, ListNode list2)
---
`list1` and `list2` are singly linked lists whose values never decrease
from one node to the next. Weave them into **one** list with that same
property, holding every node of both, and return its first node. Do it by
changing `next` pointers: the result is made of the nodes you were given.

Either list may be empty. If both are, the result is empty too.

Lists in the examples are written from head to tail: `[2,5]` is a node
holding 2 followed by a node holding 5, and `[]` is an empty list (a null
head).

@examples

## Constraints

- Each list has between `0` and `50` nodes.
- `-100 <= Node.val <= 100`
- The values of each list are in non-decreasing order.
