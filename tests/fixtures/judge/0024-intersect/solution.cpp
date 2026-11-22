class Solution {
public:
    ListNode* getIntersectionNode(ListNode* headA, ListNode* headB) {
        unordered_set<ListNode*> inA;
        for (ListNode* p = headA; p; p = p->next) inA.insert(p);
        for (ListNode* p = headB; p; p = p->next)
            if (inA.count(p)) return p;
        return nullptr;
    }
};
