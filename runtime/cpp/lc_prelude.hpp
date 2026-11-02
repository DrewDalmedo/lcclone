// lc_prelude.hpp: what a C++ solution can use without an #include, as on
// LeetCode. lc puts it at the top of every C++ build, and the .clangd file
// it writes in the workspace makes clangd read it too.
//
// It lists the standard headers one by one rather than including
// <bits/stdc++.h>, which only libstdc++ has. Headers from a newer standard
// are included only when the build uses that standard, so raising the
// standard makes them available, as on LeetCode. This file itself must stay
// valid C++11.
//
// The node structs are defined only for a problem that uses them, because
// solutions often define a `struct Node` of their own. lc defines
// LC_LISTNODE, LC_TREENODE or LC_NARYNODE before this file when the
// problem's signature needs one.
#ifndef LC_PRELUDE_HPP
#define LC_PRELUDE_HPP

#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cctype>
#include <cfloat>
#include <chrono>
#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <regex>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#if __cplusplus >= 201703L
#include <any>
#include <charconv>
#include <optional>
#include <string_view>
#include <variant>
#endif

#if __cplusplus >= 202002L
#include <bit>
#include <compare>
#include <numbers>
#include <ranges>
#include <span>
#endif

// Macros that glibc's headers don't define, even for g++, and so a
// solution may use their names, as on LeetCode: say `enum { OVERFLOW,
// UNDERFLOW };`. macOS's <math.h> has the old SVID ones (DOMAIN, OVERFLOW,
// HUGE, ...), its <limits.h> BSD ones (QUAD_MAX, SIZE_T_MAX, OPEN_MAX, ...)
// and its <stdio.h> fropen and fwopen; musl's <math.h> has HUGE. The C
// prelude #undefs the same ones. No standard library header uses them, and
// where they aren't defined this does nothing. (The C prelude also hides
// the functions those headers add, but C++ needs no such thing for helpers
// named mergesort or heapsort: with other parameter types they overload.)
#undef DOMAIN
#undef SING
#undef OVERFLOW
#undef UNDERFLOW
#undef TLOSS
#undef PLOSS
#undef X_TLOSS
#undef HUGE
#undef CHILD_MAX
#undef CLK_TCK
#undef GID_MAX
#undef OFF_MAX
#undef OFF_MIN
#undef OPEN_MAX
#undef PASS_MAX
#undef QUAD_MAX
#undef QUAD_MIN
#undef SIZE_T_MAX
#undef UID_MAX
#undef UQUAD_MAX
#undef fropen
#undef fwopen

using namespace std;

#ifdef LC_LISTNODE
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
#endif

#ifdef LC_TREENODE
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right)
        : val(x), left(left), right(right) {}
};
#endif

#ifdef LC_NARYNODE
class Node {
public:
    int val;
    vector<Node *> children;
    Node() : val(0) {}
    Node(int _val) : val(_val) {}
    Node(int _val, vector<Node *> _children) : val(_val), children(_children) {}
};
#endif

#endif // LC_PRELUDE_HPP
