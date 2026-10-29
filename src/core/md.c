/* STUB: replaced by the real markdown parser. */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdlib.h>

#include "internal.h"

int lc_md_parse(const char *text, size_t len, lc_md_doc **out)
{
    (void)text;
    (void)len;
    *out = calloc(1, sizeof **out);
    return *out ? LC_OK : LC_ERR_NOMEM;
}

void lc_md_free(lc_md_doc *doc)
{
    free(doc);
}
