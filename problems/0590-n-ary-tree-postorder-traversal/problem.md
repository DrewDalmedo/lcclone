id: 590
slug: n-ary-tree-postorder-traversal
title: N-ary Tree Postorder Traversal
difficulty: easy
tags: stack, tree, depth-first-search
signature: int[] postorder(NaryNode root)
---
List the values of the tree `root`, whose nodes can have any number of
children, in **postorder**: a node is reported only once everything beneath
it has been. For each node, the subtrees of its children come first, from
its first child to its last, and the node's own value follows them. The
root is therefore always last. An empty tree gives an empty array.

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

A node can't be written when it is first met, which makes this order harder
than preorder without recursion. Try it with an explicit stack.
