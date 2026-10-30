# Fixture for tests/e2e.sh: every construct of the statement markdown subset.
id: 1
slug: kitchen-sink
title: Kitchen Sink With A Title Long Enough To Wrap When The Terminal Is Narrow
difficulty: medium
tags: string, simulation, two-pointers, sliding-window, prefix-sum, hash-table
signature: int[][] solve(string s, int[] nums, int k)
time_limit_ms: 1500
memory_limit_mb: 128
---
# Kitchen Sink

This statement uses **every** construct of the markdown subset, so that the
renderer can be checked line by line. It has *emphasis*, ***strong emphasis
inside emphasis***, `code spans`, a [plain link](https://example.com/docs), a
[link with **bold** and `code`](https://example.com/mixed) and a link whose
text is its own address: [https://example.com](https://example.com). Escaped
markers stay literal: \*not emphasis\*, and snake_case_name stays plain.

A word longer than any terminal overflows instead of being split:
abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz.

Width counts characters, not bytes: café crème brûlée, naïve façade, 数组 and
Ω. Ünïcödé wörds wrap at the same column as ASCII ones would, ünïcödé wörds
wrap at the same column as ASCII ones would, ünïcödé wörds wrap here too.

This line ends in two spaces  
so this one starts after a hard break, and this one ends in a backslash\
which makes another break. A tab	becomes a space; runs of   spaces stay.

@examples

## Lists

- A bulleted item that is long enough to wrap onto a second line, which then
  lines up under the item's own text.
- A second item.
  - A nested item, also long enough to wrap, so that its hanging indent can
    be seen clearly.
    - Deeper still.
- A third item.

3. A numbered list that starts at three.
4. The next number.
   1) A nested numbered item that is long enough to wrap onto the next line.
   2) Another one.

> A block quote that is long enough to wrap onto more than one line when the
> terminal is eighty columns wide.
>
> A second quote paragraph.

---

```cpp
int main() {
	return 0; // never wrapped, even when this comment makes the line much longer than the terminal
}
```

~~~
tilde fence, no language
~~~

### Level Three Heading ###

#### Level four with `code` and *emphasis*

The end.
