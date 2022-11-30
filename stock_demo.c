// stock_demo.c: provides a quick demo of stock functions and the
// output of stock_plot() which should look like the following.
// 
// ==PLOT DATA==
// start/stop:  0 15
// max_height:  15
// price range: 309.00
// plot step:   20.60
//            +--B=S----------+
//     302.40 |    H   *      |
//     281.80 | *  H   *      |
//     261.20 | *  H   *      |
//     240.60 | *  H   *      |
//     220.00 |**  H   *      |
//     199.40 |**  H * *      |
//     178.80 |**  H** *  *   |
//     158.20 |**  H** *  ** *|
//     137.60 |**  H****  ****|
//     117.00 |**  H****  ****|
//      96.40 |**  H****  ****|
//      75.80 |** *H***** ****|
//      55.20 |** *H***** ****|
//      34.60 |** *H***** ****|
//      14.00 |****H*****L****|
//            +^----^----^----+
//             0    5    10   

#include "stock.h"

int main(int argc, char *argv[]){
  stock_t *stock = stock_new();
  stock_load(stock, "data/stock-min-after-max.txt");
  stock_set_hilo(stock);
  stock_set_best(stock);
  stock_plot(stock, 15, 0, stock->count);
  stock_free(stock);
  return 0;
}
