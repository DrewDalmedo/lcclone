id: 160
slug: intersection-of-two-linked-lists
title: Intersection of Two Linked Lists
difficulty: easy
tags: hash-table, linked-list, two-pointers
signature: ListNode getIntersectionNode(ListNode headA, ListNode headB)
hidden: int skipA, int skipB
build: intersect headA headB skipA skipB
time_limit_ms: 200
---
Two singly linked lists, `headA` and `headB`, may **merge**: from some node
on, they continue through the very same nodes to the end. Return the first
node that belongs to both, or `NULL` if they have none in common.

Nodes are the same only if they are one object in memory: two separate nodes
holding equal values are not shared. Neither list has a cycle, and both must
be as you found them when your function returns.

An example gives each list's values from head to tail, then `skipA` and
`skipB`: how many nodes of `headA` and of `headB` come before the first
shared node. When nothing is shared, they are the lengths of the lists.
These two numbers only describe the test data. Your function receives the
two heads alone. The output shows the list that starts at the node you
return, and `[]` for `NULL`.

@examples

## Constraints

- `headA` has `m` nodes and `headB` has `n` nodes.
- `1 <= m, n <= 3 * 10^4`
- `1 <= Node.val <= 10^5`
- `0 <= skipA <= m` and `0 <= skipB <= n`

## Follow-up

Comparing every pair of nodes is too slow here, and a table of visited
nodes costs memory. Is there a way that needs only two pointers?
