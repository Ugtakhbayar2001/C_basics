#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"
 
int main(int argc, char *argv[]){
  int echo = 0;                                // controls echoing, 0: echo off, 1: echo on
  if(argc > 1 && strcmp("-echo",argv[1])==0) { // turn echoing on via -echo command line option
    echo=1;
  }
 
  printf("Hashmap Main\n");
  printf("Commands:\n");
  printf("  hashcode <key>   : prints out the numeric hash code for the given key (does not change the hash map)\n");
  printf("  put <key> <val>  : inserts the given key/val into the hash map, overwrites existing values if present\n");
  printf("  get <key>        : prints the value associated with the given key or NOT FOUND\n");
  printf("  print            : shows contents of the hashmap ordered by how they appear in the table\n");
  printf("  structure        : prints detailed structure of the hash map\n");
  printf("  clear            : reinitializes hash map to be empty with default size\n");
  printf("  save <file>      : writes the contents of the hash map the given file\n");
  printf("  load <file>      : clears the current hash map and loads the one in the given file\n");
  printf("  next_prime <int> : if <int> is prime, prints it, otherwise finds the next prime and prints it\n");
  printf("  expand           : expands memory size of hashmap to reduce its load factor\n");
  printf("  quit             : exit the program\n");
   
  char cmd[128];
  hashmap_t hm;
  int success;
  hashmap_init(&hm, HASHMAP_DEFAULT_TABLE_SIZE);
 
  while(1){
    printf("HM> ");                 
    success = fscanf(stdin,"%s",cmd); 
    if(success==EOF){                 
      printf("\n");                   
      break;                          
    }

    // end
    if( strcmp("quit", cmd)==0 ){     
      if(echo){
        printf("quit\n");
      }
      break;                          
    } 
    // gets the hashcode
    else if( strcmp("hashcode", cmd)==0 ){ 
      fscanf(stdin,"%s",cmd);           
      if(echo){
        printf("hashcode %s\n",cmd);
      }
      printf("%ld\n", hashcode(cmd));
    }
    
    // adds given key/val to the hashmap
    else if(strcmp("put", cmd)== 0){  
      char key[128];
      char val[128];
      fscanf(stdin,"%s %s",key,val); 
      if(echo){
        printf("put %s %s\n",key, val);
      }
      success = hashmap_put(&hm, key, val);
      if(success == 0){
        printf("Overwriting previous key/val\n");
      }
      
    }

    // looks up value associated with given key in the hashmap.
    else if( strcmp("get", cmd)==0 ){     
      fscanf(stdin,"%s",cmd);            
      if(echo){
        printf("get %s\n",cmd);
      }
      char *value = hashmap_get(&hm, cmd); 
      if(value == NULL){                        
        printf("NOT FOUND\n");
      }
      else {
        printf("FOUND: %s\n",value);
      }
    }

    // clears and initializes hashmap
    else if( strcmp("clear", cmd)==0 ){ 
      if(echo){
        printf("clear\n");
      }
      hashmap_free_table(&hm);
      hashmap_init(&hm, HASHMAP_DEFAULT_TABLE_SIZE);
 
    }

    // shows structure of the hashmap
    else if(strcmp("structure", cmd)== 0){  
      if(echo){
        printf("structure\n");
      }
      hashmap_show_structure(&hm);
    }
    
    // outputs all elements of the hash table
    else if( strcmp("print", cmd)==0 ){ 
      if(echo){
        printf("print\n");
      }
      hashmap_write_items(&hm, stdout);
    }
     
    // saves hashmap to the given file
    else if( strcmp("save", cmd)==0 ){   
    fscanf(stdin,"%s",cmd);           
      if(echo){
        printf("save %s\n",cmd);
      }
      hashmap_save(&hm, cmd);
    }

    // loads a hashmap file created with hahsmap_save()
    else if( strcmp("load", cmd)==0 ){   
    fscanf(stdin,"%s",cmd);           
      if(echo){
        printf("load %s\n",cmd);
      }
      hashmap_load(&hm, cmd);
      
    }

    // makes sure that hash table_size stays prime
    else if(strcmp("next_prime", cmd)== 0){  
      fscanf(stdin,"%s",cmd); 
      if(echo){
        printf("next_prime %s\n",cmd);
      }
      printf("%d\n", next_prime(atoi(cmd)));
    }

    // allocates a new, larger area of memory for the "table" field and
    // re-adds all items currently in the hash table to it.
     else if( strcmp("expand", cmd)==0 ){ 
      if(echo){
        printf("expand\n");
      }
      hashmap_expand(&hm);
    }

    
    
    // unknown command
    else{                                 
      if(echo){
        printf("%s\n",cmd);
      }
      printf("unknown command %s\n",cmd);
    }
  }

  hashmap_free_table(&hm);
  return 0;
}