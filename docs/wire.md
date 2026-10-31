# Wire format

`lc` parses every test value itself (see [problem-format.md](problem-format.md),
"Literals"). What it hands a solution's harness, and what the harness hands
back, is a simpler format that takes a few lines of C to read and write: the
wire format. `lc_wire_encode` and `lc_wire_decode` in `include/lc.h`
implement it on the `lc` side; the harness runtimes in `runtime/`
(`common/lc_rt_core.h` with `c/lc_rt.c` or `cpp/lc_rt.cpp`) implement the
other side.

## Shape

- A value is a sequence of **tokens** separated by whitespace (spaces, tabs,
  newlines). `lc` writes single spaces inside a value and a newline after
  each input.
- A test case's inputs are written to the program's stdin one after another,
  in the order of the signature's parameters, then the `hidden:` inputs. A
  case with no inputs writes nothing.
- The harness writes the result in the same format on file descriptor 3, so
  whatever the solution prints on stdout stays separate. The result is what
  the function returns or, for a problem solved in place
  ([`check:`](problem-format.md#check)), the parameter after the call, or
  its first k elements: an array like any other.
- The type of every value is known to both sides from the signature, so
  nothing in the format says what type a token is.

## Tokens by type

| Type       | Literal       | Wire                 | Notes |
|------------|---------------|----------------------|-------|
| `int`      | `-5`          | `-5`                 | decimal, as `long` and `uint32` |
| `long`     | `-5`          | `-5`                 | |
| `uint32`   | `4294967295`  | `4294967295`         | |
| `double`   | `2.5`         | `2.5`                | see below |
| `bool`     | `true`        | `1`                  | `0` or `1` |
| `char`     | `"a"`         | `97`                 | the byte's value, 0 to 255 |
| `string`   | `"a b"`       | `3:a b`              | the length in bytes, `:`, then exactly that many bytes |
| `T[]`      | `[2,7,11]`    | `3 2 7 11`           | the count, then each item |
| `T[][]`    | `[[1,2],[3]]` | `2 2 1 2 1 3`        | arrays of arrays nest the same way |
| `string[]` | `["ab","c"]`  | `2 2:ab 1:c`         | |
| `ListNode` | `[1,2,3]`     | `3 1 2 3`            | like `int[]` |
| `TreeNode` | `[1,null,2]`  | `3 1 n 2`            | level order, `n` for null, no trailing nulls |
| `NaryNode` | `[1,null,2]`  | `3 1 n 2`            | LeetCode's N-ary level order, `n` for null |
| `void`     |               | nothing              | |

**Strings** may hold any bytes, spaces and newlines included, because the
length says where they end. The last byte must be followed by whitespace or
the end of the text.

**Doubles** are written by `lc` as the shortest of `%.15g`, `%.16g` and
`%.17g` that reads back as the same number, always with `.` as the decimal
point, so they round-trip exactly. When decoding, `lc` accepts any decimal
number in that form (for example `-0`, `1e+20`, `0.30000000000000004`) and
also `inf`, `-inf` and `nan`, which a solution may return. A harness should
write non-finite doubles in exactly those three spellings.

**Trees** a harness sends back must be well formed: a null root is `0`, not
`1 n`, and every item must have a parent (see problem-format.md, "Types").
Trailing `n` tokens are accepted and dropped.

## Decoding results safely

A result comes from a program that may be wrong, so `lc_wire_decode` never
trusts it:

- every token is checked against its type, including integer ranges;
- a count or string length larger than what is left of the text is an error,
  so a corrupt count can't make `lc` allocate much more than a fixed multiple
  of the text's own size;
- nesting is bounded by the type, so nothing in the text can make decoding
  recurse deeply.

Errors name the byte offset (counting from 1) and the bad token:
`byte 5: expected an int, got 'x'`,
`byte 1: array length 99999999999 is longer than the rest of the result`.
