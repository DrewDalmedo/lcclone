id: 24
slug: intersect
title: Intersect
difficulty: easy
signature: ListNode getIntersectionNode(ListNode headA, ListNode headB)
hidden: int skipA, int skipB
build: intersect headA headB skipA skipB
---
A fixture for `build: intersect`: the node where two lists join, or null.
From node `skipA` of `headA` and node `skipB` of `headB` on, the lists
share their nodes; when `skipA` is the length of `headA` (and `skipB` that
of `headB`), they don't join. The result shows the list from that node.
