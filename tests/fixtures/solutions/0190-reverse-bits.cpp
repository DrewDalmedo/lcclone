class Solution {
public:
    uint32_t reverseBits(uint32_t n) {
        bitset<32> in(n), out;
        for (int i = 0; i < 32; i++)
            out[31 - i] = in[i];
        return (uint32_t)out.to_ulong();
    }
};
