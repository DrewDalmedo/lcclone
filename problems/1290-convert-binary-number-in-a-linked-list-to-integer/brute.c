/* From the definition: add up bit * 2^position, counting from the tail. */
int getDecimalValue(struct ListNode* head) {
    int bits[30], n = 0, i, j, sum = 0;

    for (; head; head = head->next)
        bits[n++] = head->val;
    for (i = 0; i < n; i++) {
        int power = 1;

        for (j = 0; j < n - 1 - i; j++)
            power *= 2;
        sum += bits[i] * power;
    }
    return sum;
}
