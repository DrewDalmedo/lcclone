#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif
/* 64-bit off_t and ino_t on 32-bit glibc too (see internal.h). */
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS 64
#endif

/*
 * codegen.c: the code lc writes for a signature, in C and in C++: the
 * stub a solution starts from, in LeetCode's conventions (docs/PLAN.md
 * section 5.1), and the harness main() that reads a case's inputs, calls
 * the solution and writes its result (docs/wire.md). One type table drives
 * both languages.
 */

#include <string.h>

#include "internal.h"

/* How each kind is spelled. In lc_kind order. */
static const struct kind_code {
    const char *rt;  /* runtime suffix: lc_read_<rt>, lc_write_<rt>_array */
    const char *c;   /* C type of one value */
    const char *cpp; /* C++ type of one value */
} kinds[] = {
    {"void", "void", "void"},
    {"int", "int", "int"},
    {"long", "long long", "long long"},
    {"uint32", "uint32_t", "uint32_t"},
    {"double", "double", "double"},
    {"bool", "bool", "bool"},
    {"char", "char", "char"},
    {"string", "char*", "string"},
    {"list", "struct ListNode*", "ListNode*"},
    {"tree", "struct TreeNode*", "TreeNode*"},
    {"nary", "struct Node*", "Node*"},
};

static int is_node(lc_kind k)
{
    return k == LC_T_LIST || k == LC_T_TREE || k == LC_T_NARY;
}

/* Every type the signature mentions, parameters, return and hidden. */
static int uses_kind(const lc_problem *p, lc_kind k)
{
    size_t i;

    if (!p->sig)
        return 0;
    if (p->sig->ret.kind == k)
        return 1;
    for (i = 0; i < p->sig->nparams; i++)
        if (p->sig->params[i].type.kind == k)
            return 1;
    for (i = 0; i < p->nhidden; i++)
        if (p->hidden[i].type.kind == k)
            return 1;
    return 0;
}

/* "int", "int*", "int**": a C type with `dims` pointer levels. */
static void put_c_type(lc_str *s, lc_type t, int extra)
{
    int d;

    lc_str_puts(s, kinds[t.kind].c);
    for (d = 0; d < t.dims + extra; d++)
        lc_str_putc(s, '*');
}

/* "int", "vector<int>", "vector<vector<string>>". */
static void put_cpp_type(lc_str *s, lc_type t, int std_prefix)
{
    const char *vec = std_prefix ? "std::vector<" : "vector<";
    const char *base = kinds[t.kind].cpp;
    int d;

    for (d = 0; d < t.dims; d++)
        lc_str_puts(s, vec);
    if (std_prefix && t.kind == LC_T_STRING)
        base = "std::string";
    lc_str_puts(s, base);
    for (d = 0; d < t.dims; d++)
        lc_str_putc(s, '>');
}

/* ------------------------------------------------------------------ */
/* Stubs                                                               */
/* ------------------------------------------------------------------ */

static void c_node_comments(lc_str *s, const lc_problem *p)
{
    if (uses_kind(p, LC_T_LIST))
        lc_str_puts(s,
                    "/**\n"
                    " * Definition for singly-linked list.\n"
                    " * struct ListNode {\n"
                    " *     int val;\n"
                    " *     struct ListNode *next;\n"
                    " * };\n"
                    " */\n");
    if (uses_kind(p, LC_T_TREE))
        lc_str_puts(s,
                    "/**\n"
                    " * Definition for a binary tree node.\n"
                    " * struct TreeNode {\n"
                    " *     int val;\n"
                    " *     struct TreeNode *left;\n"
                    " *     struct TreeNode *right;\n"
                    " * };\n"
                    " */\n");
    if (uses_kind(p, LC_T_NARY))
        lc_str_puts(s,
                    "/**\n"
                    " * Definition for a Node.\n"
                    " * struct Node {\n"
                    " *     int val;\n"
                    " *     int numChildren;\n"
                    " *     struct Node** children;\n"
                    " * };\n"
                    " */\n");
}

static void cpp_node_comments(lc_str *s, const lc_problem *p)
{
    if (uses_kind(p, LC_T_LIST))
        lc_str_puts(s,
                    "/**\n"
                    " * Definition for singly-linked list.\n"
                    " * struct ListNode {\n"
                    " *     int val;\n"
                    " *     ListNode *next;\n"
                    " *     ListNode() : val(0), next(nullptr) {}\n"
                    " *     ListNode(int x) : val(x), next(nullptr) {}\n"
                    " *     ListNode(int x, ListNode *next) : val(x), "
                    "next(next) {}\n"
                    " * };\n"
                    " */\n");
    if (uses_kind(p, LC_T_TREE))
        lc_str_puts(s,
                    "/**\n"
                    " * Definition for a binary tree node.\n"
                    " * struct TreeNode {\n"
                    " *     int val;\n"
                    " *     TreeNode *left;\n"
                    " *     TreeNode *right;\n"
                    " *     TreeNode() : val(0), left(nullptr), "
                    "right(nullptr) {}\n"
                    " *     TreeNode(int x) : val(x), left(nullptr), "
                    "right(nullptr) {}\n"
                    " *     TreeNode(int x, TreeNode *left, TreeNode *right) "
                    ": val(x), left(left), right(right) {}\n"
                    " * };\n"
                    " */\n");
    if (uses_kind(p, LC_T_NARY))
        lc_str_puts(s,
                    "/*\n"
                    "// Definition for a Node.\n"
                    "class Node {\n"
                    "public:\n"
                    "    int val;\n"
                    "    vector<Node*> children;\n"
                    "\n"
                    "    Node() {}\n"
                    "\n"
                    "    Node(int _val) {\n"
                    "        val = _val;\n"
                    "    }\n"
                    "\n"
                    "    Node(int _val, vector<Node*> _children) {\n"
                    "        val = _val;\n"
                    "        children = _children;\n"
                    "    }\n"
                    "};\n"
                    "*/\n");
}

/* The C prototype of the solution, without a trailing ';' or '{'. */
static void c_prototype(lc_str *s, const lc_sig *sig)
{
    size_t i;

    put_c_type(s, sig->ret, 0);
    lc_str_printf(s, " %s(", sig->name);
    for (i = 0; i < sig->nparams; i++) {
        const lc_param *pr = &sig->params[i];

        if (i)
            lc_str_puts(s, ", ");
        put_c_type(s, pr->type, 0);
        lc_str_printf(s, " %s", pr->name);
        if (pr->type.dims >= 1)
            lc_str_printf(s, ", int %sSize", pr->name);
        if (pr->type.dims == 2)
            lc_str_printf(s, ", int* %sColSize", pr->name);
    }
    if (sig->ret.dims >= 1)
        lc_str_printf(s, "%sint* returnSize", sig->nparams ? ", " : "");
    if (sig->ret.dims == 2)
        lc_str_puts(s, ", int** returnColumnSizes");
    lc_str_putc(s, ')');
}

static void c_stub(lc_str *s, const lc_problem *p)
{
    const lc_sig *sig = p->sig;

    c_node_comments(s, p);
    if (sig->ret.dims == 1)
        lc_str_puts(s,
                    "/**\n"
                    " * Note: return an array of *returnSize elements. It "
                    "must be malloced;\n"
                    " * the caller frees it.\n"
                    " */\n");
    else if (sig->ret.dims == 2)
        lc_str_puts(s,
                    "/**\n"
                    " * Note: return an array of *returnSize arrays, and set "
                    "*returnColumnSizes\n"
                    " * to an array holding the size of each one. All of "
                    "them must be malloced;\n"
                    " * the caller frees them.\n"
                    " */\n");
    c_prototype(s, sig);
    lc_str_puts(s, " {\n\n}\n");
}

static void cpp_stub(lc_str *s, const lc_problem *p)
{
    const lc_sig *sig = p->sig;
    size_t i;

    cpp_node_comments(s, p);
    lc_str_puts(s, "class Solution {\npublic:\n    ");
    put_cpp_type(s, sig->ret, 0);
    lc_str_printf(s, " %s(", sig->name);
    for (i = 0; i < sig->nparams; i++) {
        const lc_param *pr = &sig->params[i];

        if (i)
            lc_str_puts(s, ", ");
        put_cpp_type(s, pr->type, 0);
        lc_str_printf(s, "%s %s", pr->type.dims ? "&" : "", pr->name);
    }
    lc_str_puts(s, ") {\n\n    }\n};\n");
}

static int need_sig(lc_ctx *ctx, const lc_problem *p)
{
    if (!p->sig)
        return lc_set_error(ctx, LC_ERR_INVALID, "problem %d (%s) has no "
                            "signature", p->id, p->slug);
    return LC_OK;
}

int lc_stub(lc_ctx *ctx, const lc_problem *p, const lc_lang *lang,
            lc_str *out)
{
    int rc;

    if (!p || !lang || !out)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_stub: bad arguments");
    rc = need_sig(ctx, p);
    if (rc != LC_OK)
        return rc;
    if (lang->id == LC_LANG_C)
        c_stub(out, p);
    else
        cpp_stub(out, p);
    return out->failed ? lc_oom(ctx) : LC_OK;
}

int lc_prototype(lc_ctx *ctx, const lc_problem *p, const lc_lang *lang,
                 lc_str *out)
{
    int rc;
    size_t i;

    if (!p || !lang || !out)
        return lc_set_error(ctx, LC_ERR_INVALID, "lc_prototype: bad "
                            "arguments");
    rc = need_sig(ctx, p);
    if (rc != LC_OK)
        return rc;
    if (lang->id == LC_LANG_C) {
        c_prototype(out, p->sig);
    } else {
        put_cpp_type(out, p->sig->ret, 0);
        lc_str_printf(out, " Solution::%s(", p->sig->name);
        for (i = 0; i < p->sig->nparams; i++) {
            const lc_param *pr = &p->sig->params[i];

            if (i)
                lc_str_puts(out, ", ");
            put_cpp_type(out, pr->type, 0);
            lc_str_printf(out, "%s %s", pr->type.dims ? "&" : "", pr->name);
        }
        lc_str_putc(out, ')');
    }
    return out->failed ? lc_oom(ctx) : LC_OK;
}

/* ------------------------------------------------------------------ */
/* Harnesses                                                           */
/* ------------------------------------------------------------------ */

int lc_codegen_check(lc_ctx *ctx, const lc_problem *p)
{
    char name[LC_TYPE_NAME_MAX];
    size_t i;
    int rc = need_sig(ctx, p);

    if (rc != LC_OK)
        return rc;
    if (p->sig->ret.kind == LC_T_VOID)
        return lc_set_error(ctx, LC_ERR_INVALID, "problem %d (%s): judging "
                            "a function that returns void is not supported "
                            "yet", p->id, p->slug);
    if (p->nhidden)
        return lc_set_error(ctx, LC_ERR_INVALID, "problem %d (%s): hidden "
                            "inputs are not supported yet", p->id, p->slug);
    if (p->compare.mode == LC_CMP_CHECKER)
        return lc_set_error(ctx, LC_ERR_INVALID, "problem %d (%s): compare: "
                            "checker is not supported yet", p->id, p->slug);
    if (is_node(p->sig->ret.kind))
        return lc_set_error(ctx, LC_ERR_INVALID, "problem %d (%s): "
                            "returning %s is not supported yet", p->id,
                            p->slug, lc_type_name(p->sig->ret, name));
    for (i = 0; i < p->sig->nparams; i++)
        if (is_node(p->sig->params[i].type.kind))
            return lc_set_error(ctx, LC_ERR_INVALID, "problem %d (%s): "
                                "%s parameters are not supported yet", p->id,
                                p->slug,
                                lc_type_name(p->sig->params[i].type, name));
    return LC_OK;
}

static void c_harness(lc_str *s, const lc_problem *p)
{
    const lc_sig *sig = p->sig;
    const char *rt;
    size_t i;

    lc_str_printf(s, "/* lc's harness for problem %d (%s). */\n"
                  "int main(void)\n{\n    lc_rt_read_input();\n",
                  p->id, p->slug);
    for (i = 0; i < sig->nparams; i++) {
        const lc_param *pr = &sig->params[i];

        rt = kinds[pr->type.kind].rt;
        if (pr->type.dims == 0) {
            lc_str_puts(s, "    ");
            put_c_type(s, pr->type, 0);
            lc_str_printf(s, " %s = lc_read_%s();\n", pr->name, rt);
        } else if (pr->type.dims == 1) {
            lc_str_printf(s, "    int %sSize;\n    ", pr->name);
            put_c_type(s, pr->type, 0);
            lc_str_printf(s, " %s = lc_read_%s_array(&%sSize);\n", pr->name,
                          rt, pr->name);
        } else {
            lc_str_printf(s, "    int %sSize;\n    int* %sColSize;\n    ",
                          pr->name, pr->name);
            put_c_type(s, pr->type, 0);
            lc_str_printf(s, " %s = lc_read_%s_array2(&%sSize, "
                          "&%sColSize);\n", pr->name, rt, pr->name, pr->name);
        }
    }
    lc_str_puts(s, "    lc_rt_end_input();\n");

    rt = kinds[sig->ret.kind].rt;
    if (sig->ret.dims >= 1)
        lc_str_puts(s, "    int lc_ret_size = -1;\n");
    if (sig->ret.dims == 2)
        lc_str_puts(s, "    int* lc_ret_cols = NULL;\n");
    lc_str_puts(s, "    ");
    put_c_type(s, sig->ret, 0);
    lc_str_printf(s, " lc_ret = %s(", sig->name);
    for (i = 0; i < sig->nparams; i++) {
        const lc_param *pr = &sig->params[i];

        lc_str_printf(s, "%s%s", i ? ", " : "", pr->name);
        if (pr->type.dims >= 1)
            lc_str_printf(s, ", %sSize", pr->name);
        if (pr->type.dims == 2)
            lc_str_printf(s, ", %sColSize", pr->name);
    }
    if (sig->ret.dims >= 1)
        lc_str_printf(s, "%s&lc_ret_size", sig->nparams ? ", " : "");
    if (sig->ret.dims == 2)
        lc_str_puts(s, ", &lc_ret_cols");
    lc_str_puts(s, ");\n");
    if (sig->ret.dims == 0)
        lc_str_printf(s, "    lc_write_%s(\"%s\", lc_ret);\n", rt, sig->name);
    else if (sig->ret.dims == 1)
        lc_str_printf(s, "    lc_write_%s_array(\"%s\", lc_ret, "
                      "lc_ret_size);\n", rt, sig->name);
    else
        lc_str_printf(s, "    lc_write_%s_array2(\"%s\", lc_ret, "
                      "lc_ret_size, lc_ret_cols);\n", rt, sig->name);
    lc_str_puts(s, "    lc_rt_flush();\n    return 0;\n}\n");
}

static void cpp_harness(lc_str *s, const lc_problem *p)
{
    const lc_sig *sig = p->sig;
    size_t i;

    lc_str_printf(s, "// lc's harness for problem %d (%s).\n"
                  "int main()\n{\n    lc_rt_read_input();\n",
                  p->id, p->slug);
    for (i = 0; i < sig->nparams; i++) {
        const lc_param *pr = &sig->params[i];

        lc_str_puts(s, "    ");
        put_cpp_type(s, pr->type, 1);
        lc_str_printf(s, " %s%s;\n    lc_get(%s);\n", pr->name,
                      pr->type.dims || pr->type.kind == LC_T_STRING ? ""
                                                                    : "{}",
                      pr->name);
    }
    lc_str_puts(s, "    lc_rt_end_input();\n"
                "    Solution *lc_sol = new Solution();\n    ");
    put_cpp_type(s, sig->ret, 1);
    lc_str_printf(s, " lc_ret = lc_sol->%s(", sig->name);
    for (i = 0; i < sig->nparams; i++)
        lc_str_printf(s, "%s%s", i ? ", " : "", sig->params[i].name);
    lc_str_puts(s, ");\n    lc_put(lc_ret);\n    lc_rt_flush();\n"
                "    return 0;\n}\n");
}

int lc_codegen_harness(lc_ctx *ctx, const lc_problem *p, const lc_lang *lang,
                       lc_str *out)
{
    int rc = lc_codegen_check(ctx, p);

    if (rc != LC_OK)
        return rc;
    if (lang->id == LC_LANG_C)
        c_harness(out, p);
    else
        cpp_harness(out, p);
    return out->failed ? lc_oom(ctx) : LC_OK;
}

void lc_codegen_defines(const lc_problem *p, lc_str *out)
{
    if (uses_kind(p, LC_T_LIST))
        lc_str_puts(out, "#define LC_LISTNODE 1\n");
    if (uses_kind(p, LC_T_TREE))
        lc_str_puts(out, "#define LC_TREENODE 1\n");
    if (uses_kind(p, LC_T_NARY))
        lc_str_puts(out, "#define LC_NARYNODE 1\n");
}
