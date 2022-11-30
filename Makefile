# CSCI 2021 Project 1 Makefile
AN = p1
CLASS = 2021

# -Wno-comment: disable warnings for multi-line comments, present in some tests
CFLAGS = -Wall -Wno-comment -Werror -g 
CC     = gcc $(CFLAGS)
SHELL  = /bin/bash
CWD    = $(shell pwd | sed 's/.*\///g')

PROGRAMS = \
	stock_main \
	stock_demo \
	test_stock_funcs \
	hashmap_main \
	hashmap_demo_init \

all : $(PROGRAMS) 

# cleaning target to remove compiled programs/objects
clean :
	rm -f $(PROGRAMS) *.o

help :
	@echo 'Typical usage is:'
	@echo '  > make                          # build all programs'
	@echo '  > make clean                    # remove all compiled items'
	@echo '  > make zip                      # create a zip file for submission'
	@echo '  > make prob1                    # built targets associated with problem 1'
	@echo '  > make test                     # run all tests'
	@echo '  > make test-prob2               # run test for problem 2'
	@echo '  > make test-prob2 testnum=5     # run problem 2 test #5 only'


############################################################
# 'make zip' to create p1-code.zip for submission
SHELL  = /bin/bash
CWD    = $(shell pwd | sed 's/.*\///g')
zip : clean clean-tests
	rm -f $(AN)-code.zip
	cd .. && zip "$(CWD)/$(AN)-code.zip" -r "$(CWD)"
	@echo Zip created in $(AN)-code.zip
	@if (( $$(stat -c '%s' $(AN)-code.zip) > 10*(2**20) )); then echo "WARNING: $(AN)-code.zip seems REALLY big, check there are no abnormally large test files"; du -h $(AN)-code.zip; fi
	@if (( $$(unzip -t $(AN)-code.zip | wc -l) > 256 )); then echo "WARNING: $(AN)-code.zip has 256 or more files in it which may cause submission problems"; fi



# stock problem
stock_funcs.o : stock_funcs.c stock.h
	$(CC) -c $<

stock_main.o : stock_main.c stock.h
	$(CC) -c $<

stock_demo.o : stock_demo.c stock.h
	$(CC) -c $<

stock_demo : stock_demo.o stock_funcs.o
	$(CC) -o $@ $^

stock_main : stock_main.o stock_funcs.o
	$(CC) -o $@ $^

test_stock_funcs : test_stock_funcs.c stock_funcs.o 
	$(CC) -o $@ $^

# hashmap problem
hashmap_main : hashmap_main.o hashmap_funcs.o
	$(CC) -o $@ $^

hashmap_main.o : hashmap_main.c hashmap.h
	$(CC) -c $<

hashmap_funcs.o : hashmap_funcs.c hashmap.h
	$(CC) -c $<

hashmap_demo_init : hashmap_demo_init.c hashmap_funcs.o
	$(CC) -o $@ $^

# problem targets
prob1 : stock_funcs.o

prob2 : stock_main

prob3 : hashmap_main

# Testing Targets
test : test-prob1 test-prob2 test-prob3

test-setup:
	@chmod u+x testy

test-prob1 : test_stock_funcs test-setup
	./testy test_stock1.org $(testnum)

test-prob2 : test_stock_funcs stock_main test-setup
	./testy test_stock2.org $(testnum)

test-prob3 : hashmap_main test-setup
	./testy test_hashmap.org $(testnum) 

clean-tests :
	rm -rf test-results

