id: 671
slug: second-minimum-node-in-a-binary-tree
title: Second Minimum Node In a Binary Tree
difficulty: easy
tags: tree, depth-first-search, binary-tree
signature: int findSecondMinimumValue(TreeNode root)
---
The binary tree `root` records a knockout contest in which the **lowest**
score wins. Each leaf is an entrant with a positive score. Every other node is
a match between its two children and holds the lower of their two scores,
which goes on to the next round. So a node has either no children or exactly
two, and the root holds the winning score, the smallest in the tree.

Consider the different scores that appear anywhere in the tree, each counted
once however many nodes hold it. Return the **second smallest** of them, which
is the smallest score strictly greater than the root's. If every node holds
the same score there is no such score: return `-1`.

Examples list the tree in level order from the root, with `null` in an
entrant's empty child slots.

@examples

## Constraints

- The tree has between `1` and `25` nodes.
- `1 <= Node.val <= 2^31 - 1`
- Every node has `0` or `2` children.
- A node with children holds the smaller of its two children's values.
