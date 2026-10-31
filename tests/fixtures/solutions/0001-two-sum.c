/* Two Sum in C with uthash, which lc's C prelude provides as LeetCode's
 * C environment does: no #include needed. */
struct entry {
    int key;
    int index;
    UT_hash_handle hh;
};

int* twoSum(int* nums, int numsSize, int target, int* returnSize) {
    struct entry *seen = NULL, *e, *tmp;
    int *out = malloc(2 * sizeof *out);

    *returnSize = 0;
    for (int i = 0; i < numsSize && *returnSize == 0; i++) {
        int want = target - nums[i];

        HASH_FIND_INT(seen, &want, e);
        if (e) {
            out[0] = e->index;
            out[1] = i;
            *returnSize = 2;
            break;
        }
        HASH_FIND_INT(seen, &nums[i], e);
        if (!e) {
            e = malloc(sizeof *e);
            e->key = nums[i];
            e->index = i;
            HASH_ADD_INT(seen, key, e);
        }
    }
    HASH_ITER(hh, seen, e, tmp) {
        HASH_DEL(seen, e);
        free(e);
    }
    return out;
}
