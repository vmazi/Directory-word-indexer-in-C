all: main.c libst.a
	gcc -o index main.c libst.a -Wall -Werror -g

libst.a: tokenizer.o sorted-list.o hashtable.o
	ar r libst.a tokenizer.o sorted-list.o hashtable.o

tokenizer.o: tokenizer.c tokenizer.h
	gcc -c tokenizer.c tokenizer.h -Wall -Werror -g

sorted-list.o: sorted-list.c sorted-list.h
	gcc -c sorted-list.c sorted-list.h -Wall -Werror -g

hashtable.o: hashtable.c hashtable.h
	gcc -c hashtable.c hashtable.h -Wall -Werror -g
clean: 
	rm -r -f libst.a tokenizer.h.gch sorted-list.h.gch hashtable.h.gch tokenizer.o sorted-list.o hashtable.o tokenizer.c~ hashtable.c~ sorted-list.c~ Makefile~ main.c~ index tokenizer.h~ sorted-list.h~ hashtable.h~ readme.pdf~ tesplan.txt~