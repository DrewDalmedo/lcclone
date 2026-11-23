id: 559
slug: maximum-depth-of-n-ary-tree
title: Maximum Depth of N-ary Tree
difficulty: easy
tags: tree, depth-first-search, breadth-first-search
signature: int maxDepth(NaryNode root)
---
`root` is the root of a tree whose nodes can have any number of children.
Of all the paths that lead down from the root to a leaf, take one with the
most nodes, and return how many nodes it has, counting both of its ends.
That count is the tree's **depth**: a lone root has depth 1, and the empty
tree has depth 0.

Trees in the examples are written level by level. The root comes first,
then a `null`. After that come the children of each node in turn, in the
order the nodes were listed, with a `null` closing each group; a node
without children has an empty group, and the `null`s at the very end are
left out. So `[5,null,8,2,null,4]` is a root 5 with the children 8 and 2,
where 8 has one child, 4.

@examples

## Constraints

- The tree has between `0` and `10^4` nodes.
- Its depth is at most `1000`.
- `0 <= Node.val <= 10^4`
