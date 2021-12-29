SOURCE_MAIN=main

fma_tetris: $(SOURCE_MAIN).o
	gcc -Wall -Werror -Wpedantic -Wextra $(SOURCE_MAIN).o -o $(SOURCE_MAIN) -lraylib

main: $(SOURCE_MAIN).c
	gcc -c -Wall -Werror -Wpedantic -Wextra $(SOURCE_MAIN).c
