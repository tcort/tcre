tcre.test: tcre.c tcre.test.c check.h tcre.h
	cc -o tcre.test -Wall -Werror -std=c99 -pedantic tcre.c tcre.test.c
