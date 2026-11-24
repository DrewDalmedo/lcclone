id: 783
slug: minimum-distance-between-bst-nodes
title: Minimum Distance Between BST Nodes
difficulty: easy
tags: tree, depth-first-search, breadth-first-search, binary-search-tree, binary-tree
signature: int minDiffInBST(TreeNode root)
---
A straight railway line keeps a record of its stations in a binary search
tree, `root`. A node's value is its station's position, in metres from the
start of the line. No two stations share a position: everything in a node's
left subtree lies before it on the line, and everything in its right subtree
lies after it.

Stations at positions `a` and `b` are `|a - b|` metres apart. Return the
distance between the **two stations that stand closest together**. It is
measured along the railway, so how many edges separate the two nodes in the
tree plays no part.

Each example writes the tree out one depth at a time, starting at the root,
and puts `null` where a branch has no station.

@examples

## Constraints

- The line has between `2` and `100` stations (nodes).
- `0 <= Node.val <= 10^5`
- The tree is a valid binary search tree, so all positions are distinct.
