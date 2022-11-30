// stock_main.c: Load a stock file and print it. 

#include "stock.h"

int main(int argc, char *argv[]){
  if(argc < 3){
    printf("usage: %s <stockfile> <max_width> [start] [stop]\n",argv[0]);
    return 1;
  }
  
  char *filename = argv[1];      // read filename from command line
  int max_width = atoi(argv[2]); // read width from command line

  stock_t *stock = stock_new();
  int ret = stock_load(stock, filename);
  if(ret == -1){
    printf("Failed to load stock, exiting\n");
    return 1;
  }

  int start = 0;                // default to printing whole 
  int stop = stock->count;      // range of stocks
  if(argc > 3){                 
    start = atoi(argv[3]);      // optional 3rd + 4th command line
  }                             // args allow printing a slice of the
  if(argc > 4){                 // the full stock range
    stop = atoi(argv[4]);
  }

  stock_set_hilo(stock);
  ret = stock_set_best(stock);
  if(ret == -1){
    printf("No viable buy/sell point\n");
  }

  stock_print(stock);
  stock_plot(stock, max_width, start, stop);

  stock_free(stock);

  return 0;
}
