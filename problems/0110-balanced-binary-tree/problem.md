id: 110
slug: balanced-binary-tree
title: Balanced Binary Tree
difficulty: easy
tags: tree, depth-first-search, binary-tree
signature: bool isBalanced(TreeNode root)
---
Inspect the binary tree `root` one node at a time. At each node, measure its
two branches: the **height** of a branch is the number of nodes on the
longest way down through it, and a missing branch has height 0. The node
**passes** when the heights of its two branches differ by at most 1.

Return `true` if every node passes, and `false` if even one fails, however
deep it sits. A tree with no nodes has nothing to fail, so it gets `true`.

In the examples a tree is a level-order list in which `null` fills an empty
child position.

@examples

## Constraints

- The tree has between `0` and `5000` nodes.
- `-10^4 <= Node.val <= 10^4`

## Follow-up

Measuring both branches afresh at every node repeats a lot of work. Can you
settle the answer while working out each height only once?
