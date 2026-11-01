/* Small cases: a main tank of 1 to 30 liters and usually a reserve of 1 to
 * 8, so that either tank can be the one that runs out and the main tank
 * often lands on 5 exactly. Full-size cases: the seed picks both tanks
 * anywhere in 1..100, a nearly full main tank with any reserve, or a full
 * main tank with a reserve near the 24 liters it can use. */
void gen(int small)
{
    int main_tank, reserve, shape;

    if (small) {
        main_tank = (int)rnd(1, 30);
        if (rnd_chance(20))
            reserve = (int)rnd(9, 100);
        else
            reserve = (int)rnd(1, 8);
    } else {
        shape = (int)(gen_seed() % 3);
        if (shape == 0) {
            main_tank = (int)rnd(1, 100);
            reserve = (int)rnd(1, 100);
        } else if (shape == 1) {
            main_tank = (int)rnd(90, 100);
            reserve = (int)rnd(1, 100);
        } else {
            main_tank = 100;
            reserve = (int)rnd(20, 28);
        }
    }
    put_int(main_tank);
    put_int(reserve);
}
