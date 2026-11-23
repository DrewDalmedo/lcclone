id: 234
slug: palindrome-linked-list
title: Palindrome Linked List
difficulty: easy
tags: linked-list, two-pointers, stack, recursion
signature: bool isPalindrome(ListNode head)
time_limit_ms: 500
---
Each node of the linked list `head` holds one digit. Decide whether the
sequence of digits is **symmetric**: the first digit equals the last, the
second equals the second to last, and so on toward the middle. Return `true`
if it is and `false` if it isn't. A list of one node is symmetric.

Lists in the examples are written from head to tail, so `[2,5]` is a node
holding 2 followed by a node holding 5.

@examples

## Constraints

- The list has between `1` and `10^5` nodes.
- `0 <= Node.val <= 9`

## Follow-up

Copying the digits into an array makes this easy. Can you manage in linear
time with a constant amount of extra memory?
