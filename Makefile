header:
	mkdir -p build
	gcc -Wall -g -O0 -pedantic -Wextra -DLACH_DEBUG=1 -c lach.h -o build/liblach.o

tests: header test_hashtable test_list test_logger test_string test_vector

test_hashtable:
	mkdir -p tests/hashtable/build
	gcc -Wall -g -O0 -pedantic -Wextra -DLACH_DEBUG=1 tests/hashtable/main.c -o tests/hashtable/build/a.out
	./tests/hashtable/build/a.out

test_list:
	mkdir -p tests/list/build
	gcc -Wall -g -O0 -pedantic -Wextra -DLACH_DEBUG=1 tests/list/main.c -o tests/list/build/a.out
	./tests/list/build/a.out

test_logger:
	mkdir -p tests/logger/build
	gcc -Wall -g -O0 -pedantic -Wextra -DLACH_DEBUG=1 tests/logger/main.c -o tests/logger/build/a.out
	./tests/logger/build/a.out

test_string:
	mkdir -p tests/string/build
	gcc -Wall -g -O0 -pedantic -Wextra -DLACH_DEBUG=1 tests/string/main.c -o tests/string/build/a.out
	./tests/string/build/a.out

test_vector:
	mkdir -p tests/vector/build
	gcc -Wall -g -O0 -pedantic -Wextra -DLACH_DEBUG=1 tests/vector/main.c -o tests/vector/build/a.out
	./tests/vector/build/a.out

clean:
	rm -rf build
	rm -rf tests/hashtable/build
	rm -rf tests/list/build
	rm -rf tests/logger/build
	rm -rf tests/string/build
	rm -rf tests/vector/build
