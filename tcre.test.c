/******************************************************************************

Copyright (c) 2020,  Thomas Cort


Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
		{ "-", "[-]"}
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

	exit(EXIT_SUCCESS);
}
