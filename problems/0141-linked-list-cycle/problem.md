id: 141
slug: linked-list-cycle
title: Linked List Cycle
difficulty: easy
tags: hash-table, linked-list, two-pointers
signature: bool hasCycle(ListNode head)
hidden: int pos
build: cycle head pos
---
Start at `head` and keep following `next` pointers. Either the walk stops at
a null pointer, or the last node leads back to a node already passed and the
walk goes round forever. Return `true` if it goes round and `false` if it
stops.

What matters is which **node** you are on, not what it holds: different
nodes may store the same value.

An example shows the values from head to tail, then `pos`: the index (from
0) of the node that the last node links back to, or `-1` when the last
node's `next` is null. `pos` only describes the test data. Your function
receives `head` alone.

@examples

## Constraints

- The list has between `0` and `10^4` nodes.
- `-10^5 <= Node.val <= 10^5`
- `pos` is `-1` or the index of a node of the list.

## Follow-up

Remembering every node you have visited takes `O(n)` memory. Can you tell
the two cases apart with a constant amount?
