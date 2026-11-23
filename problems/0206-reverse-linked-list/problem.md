id: 206
slug: reverse-linked-list
title: Reverse Linked List
difficulty: easy
tags: linked-list, recursion
signature: ListNode reverseList(ListNode head)
---
`head` is the first node of a singly linked list. Flip the direction of
every link, so that the old last node comes first and the old first node
ends up at the tail with a null `next`, and return the new first node.

Lists in the examples are written from head to tail: `[2,5]` is a node
holding 2 followed by a node holding 5, and `[]` is an empty list (a null
head).

@examples

## Constraints

- The list has between `0` and `5000` nodes.
- `-5000 <= Node.val <= 5000`

## Follow-up

If your solution uses a loop, write one that recurses, and the other way
round.
