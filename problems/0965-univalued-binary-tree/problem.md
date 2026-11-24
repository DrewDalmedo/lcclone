id: 965
slug: univalued-binary-tree
title: Univalued Binary Tree
difficulty: easy
tags: tree, depth-first-search, breadth-first-search, binary-tree
signature: bool isUnivalTree(TreeNode root)
---
Every node of the binary tree `root` has been painted, and its value is the
number of its colour. Did the painter stick to a single colour? Return `true`
if all the nodes hold one and the same value, and `false` if any two of them
differ. Colour `0` is a colour like the others, and a tree of one node is
always a single colour.

Example trees read from the top row down, left to right in each row; a `null`
says the node above has nothing on that side.

@examples

## Constraints

- The tree has between `1` and `100` nodes.
- `0 <= Node.val < 100`
