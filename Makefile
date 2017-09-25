CFLAGS=-O1 -Wall -Werror

default all: memcmpe

memcmpe.o: memcmpe.c
	$(CC) $(CFLAGS) -c $? -o $@

memcmpe: memcmpe.o
	$(CC) $(CFLAGS) -o $@ $?


clean: ## Cleanup
	rm -fv *.o

help: ## Show help
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##/\t/'
