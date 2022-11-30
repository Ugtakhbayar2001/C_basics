// stock_funcs.c: support functions for the stock_main program.

#include "stock.h"

// Allocate a new stock struct and initialize its fields.

stock_t *stock_new(){
  stock_t *stock = malloc(sizeof(stock_t));
  stock->count = -1;
  stock->lo_index = -1;
  stock->hi_index = -1;
  stock->best_buy = -1;
  stock->best_sell = -1;
  stock->prices = NULL;
  stock->data_file = NULL;
  return stock;
}

// Free a stock. Check the 'data_file' and 'prices' fields:
// if they are non-NULL, then free them. Then free the pointer to
// 'stock' itself.
void stock_free(stock_t *stock){
  if(stock -> data_file != NULL){
    free(stock -> data_file);
  }
  if(stock -> prices != NULL){
    free(stock -> prices);
  }
  free(stock);
  return;
}

void stock_print(stock_t *stock){
  double profit = 0.0;
  printf("==STOCK DATA==\n");
  if(stock->data_file == NULL){
    printf("data_file: NULL\n");
  }
  else{
    printf("data_file: %s\n", stock->data_file);
  }
  printf("count: %d\n", stock->count);
  if(stock->prices == NULL){
    printf("prices: NULL\n");
  }
  else{
    if(stock->count == 0){
    printf("prices: []\n");
    } 
    else if(stock->count == 1){
      printf("prices: [%.2f]\n", stock->prices[0]);
    }
    else if(stock->count == 2){
      printf("prices: [%.2f, %.2f]\n",stock->prices[0], stock->prices[1]);
    }      
    else if(stock->count == 3){
      printf("prices: [%.2f, %.2f, %.2f]\n",stock->prices[0], stock->prices[1], stock->prices[2]);
    }      
    else if(stock->count > 3){
      printf("prices: [%.2f, %.2f, %.2f, ...]\n",stock->prices[0], stock->prices[1], stock->prices[2]);
    } 
  } 
    printf("lo_index:  %d\n", stock->lo_index);
    printf("hi_index:  %d\n", stock->hi_index);
    printf("best_buy:  %d\n", stock->best_buy);
    printf("best_sell: %d\n", stock->best_sell);
    if(stock->best_buy == -1 || stock->best_sell == -1){
      printf("profit:    0.00\n");
    }
    else{
      profit = stock->prices[stock->best_sell] - stock->prices[stock->best_buy];
      printf("profit:    %.2f\n", profit);
    }
  return;
}


// Sets the index of 'lo_index' and 'hi_index' fields of
// the stock to be the positions in 'prices' of the lowest and highest
// values present in it. Uses a simple loop over the array 'prices'
// which is 'count' elements long to examine each for high/low. If
// 'count' is zero, makes no changes to 'lo_index' and 'hi_index'.
void stock_set_hilo(stock_t *stock){
  if(stock->count > 0){
    stock->lo_index = 0;
    stock->hi_index = 0;
    for(int i = 0; i < stock->count; i++){
      if(stock-> prices[i] < stock-> prices[stock->lo_index]){
        stock->lo_index = i;
      }
    }
    for(int i = 1; i < stock->count; i++){
      if(stock-> prices[stock->hi_index] < stock-> prices[i]){
        stock->hi_index = i;
      }
    }
  }
  return;
}
  
// Sets the 'best_buy' and 'best_sell' fields of 'stock'.
// This corresponds to the pair which produces the best profit. On
// determining the best buy/sell indices which produce a positive
// profit, sets these fields in 'stock' and returns 0. If there is no
// buy/sell point which would result in a positive profit, sets the
// 'best_buy' and 'best_sell' indices to -1 and returns -1. Always
// calculates the earliest buy/sell pair of indices that would get the
// best profit: if 5,8 and 5,9 and 7,10 all give the same, maximal
// profit, the best buy/sell indices are set to 5,7.
// 
// ALGORITHM NOTES
// One intuitive algorithm to compute this is to try every possible
// buy index (outer loop) and every possible sell index after it
// (inner loop) looking for the maximum profit produced in it. This is
// a O(N^2) algorithm with N=count.  Using this algorithm is a good
// start. Some MAKEUP CREDIT will be awarded for implementing a more
// efficient, O(N) algorithm here. See the specification for more details.
int stock_set_best(stock_t *stock){
  double max_prof = 0.0;
  stock->best_buy = -1;
  stock->best_sell = -1;
  if(stock->count >= 0){
    for(int i=0; i < stock->count; i++){
      for(int j=i; j < stock->count; j++){
        if(stock->prices[j]-stock->prices[i] > max_prof){
          stock->best_buy = i;
          stock->best_sell = j;
          max_prof = stock->prices[j] - stock->prices[i];
        }
      }
    }    
  }
  if(max_prof > 0){
    return 0;
  }
  return -1;
}

// Opens file named 'filename' and counts how many times
// the '\n' newline character appears in it which corresponds to how
// many lines of text are in it.  Makes use of either fscanf() with
// the %c format to read single characters or alternative I/O
// functions like fgetc(). Closes the file before returning a count of
// how many lines are it it.  If for any reason the file cannot be
// opened, prints a message like
//
// Could not open file 'not-there.txt'
//
// and returns -1 to indicate failure.
int count_lines(char *filename){
  FILE *non_empty;
  char c;
  int lines = 0;
  non_empty = fopen(filename, "r"); 
  if(non_empty == NULL){
    printf("Could not open file '%s'\n", filename);
    return -1;
  }
  
  do{
    c = fgetc(non_empty);
    if(c == '\n'){
      lines++;
    }
  }while(c != EOF);

  fclose(non_empty);

  return lines;
}

// Loads a stock from file 'filename' into 'stock' filling
// its 'prices' and 'count' fields in. Makes use of the count_lines()
// function to determine how many lines are in the file which will
// dictate 'count' and the length of 'prices'. Allocates space in the
// heap for the stock's 'prices' array, opens the 'filename' and
// iterates through reading prices into the array. The data format for
// prices files is
//
// time_03 133.00
// time_04 143.00
// time_05 168.00
// time_06 91.00
// 
// where each line has a time as as single string and a price which is
// floating point number. The times can be ignored which can be
// accomplished with a fscanf() call with format "%*s" to read a
// string but ignore/discard it.
// 
// Assigns the 'datafile' field to be a duplicated string of
// 'filename' for which 'strdup()' is extremely useful. This string
// must be free()'d later likely in 'stock_free()'
// 
// On successfully loading the stock, returns 0.
//
// If 'filename' cannot be opened, prints the message 
// 
// Unable to open stock file 'some-stock.txt', bailing out
//
// with 'filename' substituted in for the name of the stock and
// returns -1.
int stock_load(stock_t *stock, char *filename){
  stock->count = count_lines(filename);
  if(stock->count == -1){
    printf("Unable to open stock file '%s', bailing out\n", filename);
    return -1;
  }
  stock->prices = malloc(sizeof(double) * stock->count);
  FILE *non_empty = fopen(filename, "r");
  if(non_empty == NULL){
    printf("Unable to open stock file '%s', bailing out\n", filename);
    fclose(non_empty);
    return -1;
  }
  for(int i=0; i < stock->count; i++){
    fscanf(non_empty, "%*s %lf", &stock->prices[i]);
  }
  fclose(non_empty);
  stock->data_file = strdup(filename);
  return 0;
}

// Plots a graphical representation of stock
// information. First calculates and prints plot which is in the
// following format:
//
// ==PLOT DATA==
// start/stop:  0 15
// max_height:  14
// price range: 309.00
// plot step:   22.07
//            +--B=S----------+
//     300.93 |    H   *      |
//     278.86 | *  H   *      |
//     256.79 | *  H   *      |
//     234.71 | *  H   *      |
//     212.64 |**  H   *      |
//     190.57 |**  H * *      |
//     168.50 |**  H** *  *  *|
//     146.43 |**  H** *  ****|
//     124.36 |**  H****  ****|
//     102.29 |**  H****  ****|
//      80.21 |** *H***** ****|
//      58.14 |** *H***** ****|
//      36.07 |** *H***** ****|
//      14.00 |****H*****L****|
//            +^----^----^----+
//             0    5    10   
// 
// Here brief notes on the format with more detail in the project
// specification.
// - Parameters start, stop, and max_height are printed first along with
//   calculated information like the price_range (uses hi_index and
//   lo_index)
// - The main body of the plot is exactly max_height characters high. The
//   lowest line is the price at lo_index; the highest is hi_index minus
//   plot_step
// - The vertical axis prices can be printed with the format specifier
//   %10.2f to give the correct leading whitespace with 2 digits of
//   accuracy
// - If the hi and lo prices appear in the range, their bar is printed
//   with an H or an L while all other stocks are printed with a *
// - The top axis may include a B and an S at the positions of the
//   best_buy and best_sell index with a = between these to show the
//   period of ownership.
// - The bottom axis shows tic marks as ^ and below them index labels at
//   values divisible by 5. For the numeric index labels, it is easiest
//   to print spaces to a value divisible by 5 then use the format
//   specifier %-5d to print integers: this aligns left and prints
//   whitespace padding on the right to width 5. In a loop, one can
//   advance by +5 each time a label is printed.
void stock_plot(stock_t *stock, int max_height, int start, int stop){
  double range = stock->prices[stock->hi_index] - stock->prices[stock->lo_index];
  double plot_step = range/max_height;
  double bottom = stock->prices[stock->lo_index];

  // plot data section 
  printf("==PLOT DATA==\n");
  printf("start/stop:  %d %d\n", start, stop);
  printf("max_height:  %d\n", max_height);
  printf("price range: %.2f\n", range);
  printf("plot step:   %.2f\n", plot_step);

  // Top portion of the plot
  printf("           +");
  for(int i=start; i < stop; i++){
    if(i==stock->best_buy){
      printf("B");
    }
    if(i==stock->best_sell){
      printf("S");
    }
    if(i > stock->best_buy && i < stock->best_sell){
      printf("=");
    }
    else if(i < stock->best_buy || i > stock->best_sell) {
      printf("-");
    }
  }
  printf("+\n");

  //  main part of plot
  for(int i = max_height; i > 0; i--){
    printf("%10.2f ", (bottom + (i-1)*plot_step));
    printf("|");
    for(int j = start; j < stop; j++){
      if(j == stock->hi_index){
        printf("H");
      }
      else if((bottom + (i-1)*plot_step) <= stock->prices[j]){
        if(j == stock->lo_index){
        printf("L");
        }
        else{
          printf("*");
        } 
      }
      else{
        printf(" ");
      }
    }
    printf("|\n");
  }

  // bottom portion of the plot
  printf("           +");
  for(int i = start; i < stop; i++){
    if(i == 0 ||  i%5 == 0){
      printf("^");
    }
    else{
      printf("-");
    }
  }
  printf("+\n");

  // printing values divisible by 5
  printf("            ");
  for(int i = start; i < stop; i++){
    if(i == 0 ||  i%5 == 0){
      printf("%-5d", i);
    }
  }
  return;
}
  
