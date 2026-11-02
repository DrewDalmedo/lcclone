id: 3898
slug: find-the-degree-of-each-vertex
title: Find the Degree of Each Vertex
difficulty: easy
tags: graph, array, matrix
signature: int[] findDegrees(int[][] matrix)
---
A network has `n` radio stations, numbered `0` to `n - 1`, and some pairs
of stations share a direct link. The square table `matrix` records them:
`matrix[i][j]` is `1` when stations `i` and `j` are linked, and `0` when
they are not. A link works both ways, so `matrix[i][j]` and `matrix[j][i]`
always agree, and no station is linked to itself, so the diagonal is all
`0`.

Count, for every station, how many links it takes part in. In graph terms
the stations are vertices, the links are edges, and this count is the
**degree** of a vertex. Return a list of `n` numbers whose entry `i` is
the degree of station `i`. A station with no links has degree `0`.

@examples

## Constraints

- `n == matrix.length == matrix[i].length`
- `1 <= n <= 100`
- `matrix[i][j]` is `0` or `1`
- `matrix[i][i] == 0`
- `matrix[i][j] == matrix[j][i]`
