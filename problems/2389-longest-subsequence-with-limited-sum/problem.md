id: 2389
slug: longest-subsequence-with-limited-sum
title: Longest Subsequence With Limited Sum
difficulty: easy
tags: greedy, array, binary-search, prefix-sum, sorting
signature: int[] answerQueries(int[] nums, int[] queries)
---
Think of `nums` as a to-do list: task `i` takes `nums[i]` minutes. Each
entry of `queries` is a time budget, and is answered on its own.

For a budget `q`, you may pick any tasks you like, each at most once, as
long as their minutes add up to `q` or less; a total of exactly `q` is
fine. Two tasks that take the same time are still two tasks. Picking no
task at all is allowed, so the count can be `0`.

Return an array `answer` as long as `queries`, where `answer[i]` is the
largest number of tasks you can pick within the budget `queries[i]`. The
answers follow the order of `queries`.

In the terms of the title: the picked tasks, kept in their order, form a
subsequence of `nums`, and `answer[i]` is the length of the longest
subsequence whose sum is at most `queries[i]`.

@examples

## Constraints

- `1 <= nums.length, queries.length <= 1000`
- `1 <= nums[i], queries[i] <= 10^6`
