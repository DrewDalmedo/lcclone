/* Copy the digits into an array and compare from both ends. */
bool isPalindrome(struct ListNode* head) {
    int *d = malloc(100001 * sizeof *d), n = 0, i;
    bool same = true;

    for (; head; head = head->next)
        d[n++] = head->val;
    for (i = 0; i < n / 2; i++)
        if (d[i] != d[n - 1 - i])
            same = false;
    free(d);
    return same;
}
