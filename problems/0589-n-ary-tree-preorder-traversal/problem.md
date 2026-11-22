id: 589
slug: n-ary-tree-preorder-traversal
title: N-ary Tree Preorder Traversal
difficulty: easy
tags: stack, tree, depth-first-search
signature: int[] preorder(NaryNode root)
---
Visit the tree `root`, whose nodes can have any number of children, in
**preorder**, and return the values in the order you reach them. A node is
visited before anything below it: first the node, then the whole subtree of
its first child, then the whole subtree of its second child, and so on. An
empty tree gives an empty array.

Trees in the examples are written level by level. The root comes first,
then a `null`. After that come the children of each node in turn, in the
order the nodes were listed, with a `null` closing each group; a node
without children has an empty group, and the `null`s at the very end are
left out. So `[5,null,8,2,null,4]` is a root 5 with the children 8 and 2,
where 8 has one child, 4.

@examples

## Constraints

- The tree has between `0` and `10^4` nodes.
- Its height is at most `1000` levels.
- `0 <= Node.val <= 10^4`

## Follow-up

Write it without recursion, keeping the nodes still to visit on a stack of
your own.
