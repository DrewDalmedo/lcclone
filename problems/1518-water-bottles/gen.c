/* Small cases use small numbers, with numExchange mostly at most
 * numBottles + 1, so that trips to the machine, leftover empties and the
 * no-trip edge all come up often. Full-size cases range over the whole of
 * the constraints; the seed picks numExchange = 2 (the most trips),
 * numBottles = 100, a numExchange that allows at least one trip, or two
 * random numbers. */
void gen(int small)
{
    int bottles, exchange, top;

    if (small) {
        bottles = (int)rnd(1, 20);
        top = rnd_chance(20) ? 22 : bottles + 1;
        exchange = (int)rnd(2, top < 2 ? 2 : top);
    } else {
        switch ((int)(gen_seed() % 4)) {
        case 0:
            bottles = (int)rnd(1, 100);
            exchange = 2;
            break;
        case 1:
            bottles = 100;
            exchange = (int)rnd(2, 100);
            break;
        case 2:
            bottles = (int)rnd(2, 100);
            exchange = (int)rnd(2, bottles);
            break;
        default:
            bottles = (int)rnd(1, 100);
            exchange = (int)rnd(2, 100);
            break;
        }
    }
    put_int(bottles);
    put_int(exchange);
}
