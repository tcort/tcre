/******************************************************************************

Copyright (c) 2020 Thomas Cort

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "check.h"
#include "tcre.h"

int main(int argc, char *argv[]) {

	size_t i;
	int debug;
	char *pass[][2] = {
		{ "13.37", "[123456789]+[0123456789]*\\.[0123456789][0123456789]" },
		{ "a03.37a", "[123456789]+[0123456789]*\\.[0123456789][0123456789]" },
		{ "Hello", "[Hh]ello" },
		{ "abababc", "a[ab]+c" },
		{ "Hello", "[^h]ello" },
		{ "hello", "[Hh]ello" },
		{ "Hello", "^H.l*o$" },
		{ "Hello", "Hel+o"   },
		{ "Hello", "^"       },
		{ "Hello", "$"       },
		{ "Hello", "Hell?o"  },
		{ "Hello", "Hell?o"  },
		{ "Helo",  "Hell?o"  },
		{ "",      "^$"      },
		{ "",      ""        },
		{ "Hello", "Hello"   },
		{ "\\^\\.\\*\\$", "^.*$" },
		{ "This is text", "This is text"},
		{ "a4", "[a-z0-9]+"},
		{ "z", "[a-z0-9]"},
		{ "a", "[^b-z0-9]"},
		{ "9", "[^b-z0-8]"},
		{ "-", "[-]"},
		{ "4", "[:digit:]" },
		{ "C42C", "C[:digit:]+C" }
	};
#define NPASS (sizeof(pass)/sizeof(pass[0]))

	char *fail[][2] = {
		{ "a", "[b-z0-9]"},
		{ "9", "[b-z0-8]"},
		{ "yello", "[Hh]ello" },
		{ "Heo",  "Hel+o"   },
		{ "abc",  NULL      },
		{ NULL,   "abc"     },
		{ NULL,   NULL      },
		{ "Hola", "^H.l*o$" },
		{ "CxxC", ".[:digit:]."    }
	};
#define NFAIL (sizeof(fail)/sizeof(fail[0]))

	if (argc == 2 && strcmp(argv[1], "-d") == 0) {
		debug = 1;
	} else {
		debug = 0;
	}

	for (i = 0; i < NPASS; i++) {
		if (debug == 1) {
			printf("subject='%s' pattern='%s'\n", pass[i][0], pass[i][1]);
		}
		check(match(pass[i][0], pass[i][1]) == 1, "should match");
	}

	for (i = 0; i < NFAIL; i++) {
		if (debug == 1) {
			printf("subject='%s' pattern='%s'\n", fail[i][0], fail[i][1]);
		}
		check(match(fail[i][0], fail[i][1]) == 0, "should not match");
	}

	if (debug == 1) {
		printf("OK\n");
	}

	exit(EXIT_SUCCESS);
}
