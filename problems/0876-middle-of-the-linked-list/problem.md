id: 876
slug: middle-of-the-linked-list
title: Middle of the Linked List
difficulty: easy
tags: linked-list, two-pointers
signature: ListNode middleNode(ListNode head)
---
Return the node at the center of the singly linked list `head`. With an odd
number of nodes, that is the node with as many nodes before it as after it.
With an even number, two nodes share the center, and the answer is the
**later** one, the one nearer the tail.

Lists in the examples are written from head to tail: `[2,5]` is a node
holding 2 followed by a node holding 5, and `[]` is an empty list (a null
head). The output shows the list from the node you return to the tail.

@examples

## Constraints

- The list has between `1` and `100` nodes.
- `1 <= Node.val <= 100`
