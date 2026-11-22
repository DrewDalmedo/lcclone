id: 1290
slug: convert-binary-number-in-a-linked-list-to-integer
title: Convert Binary Number in a Linked List to Integer
difficulty: easy
tags: linked-list, math
signature: int getDecimalValue(ListNode head)
---
Every node of the linked list `head` stores a single bit. Taken from head to
tail, the bits are a number written in base 2: the head is its highest bit
and the tail its lowest. Work out the number and return it. The list may
begin with zeros, which add nothing.

Lists in the examples are written from head to tail, so `[1,0]` is a node
holding 1 followed by a node holding 0, which is the number 2.

@examples

## Constraints

- The list has between `1` and `30` nodes.
- `Node.val` is `0` or `1`.
