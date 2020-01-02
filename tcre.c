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

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tcre.h"

enum tcre_type { ORD='c', CCL='[', LANCHOR='^', RANCHOR='$', DOT='.', STAR='*', PLUS='+', QUEST='?' };
typedef enum tcre_type tcre_type_t;
struct tcre {
	tcre_type_t type;
	char c[UCHAR_MAX];
};
typedef struct tcre tcre_t;

static int amatch(char *subject, tcre_t *tcre);
static int cmatch(tcre_type_t type, char c, tcre_t *tcre);
static int smatch(char *subject, tcre_t *ch, tcre_t *tcre);

static int cmatch(tcre_type_t type, char c, tcre_t *tcre) {
	return (type == tcre[0].type && tcre[0].c[(unsigned char)c]);
}

static int smatch(char *subject, tcre_t *ch, tcre_t *tcre) {
	size_t i;
	for (i = 0; i < strlen(subject); i++) {
		int rc;
		rc = amatch(subject, &tcre[i]);
		if (rc == 1) {
			return 1;
		}

		if (cmatch(ORD, subject[i], ch) && cmatch(DOT, '.', ch)) {
			break;
		}
	}
	return 0;
}

static int amatch(char *subject, tcre_t *tcre) {

	if (cmatch(ORD, '\0', &tcre[0])) {
		return 1; /* end of regex without bailing? success! */
	} else if (cmatch(QUEST, '?', &tcre[1])) {
		if (cmatch(ORD, subject[0], &tcre[0])) {
			return amatch(&subject[1], &tcre[2]);
		} else {
			return amatch(&subject[0], &tcre[2]);
		}
	} else if (cmatch(PLUS, '+', &tcre[1])) {
		if (!cmatch(ORD, subject[0], &tcre[0])) {
			return 0;
		}
		return smatch(subject, &tcre[0],  &tcre[2]);
	} else if (cmatch(STAR, '*', &tcre[1])) {
		return smatch(subject, &tcre[0],  &tcre[2]);
	} else if (cmatch(ORD, '\0', &tcre[1]) && cmatch(RANCHOR, '$', &tcre[0])) {
		return strlen(subject) == 0;
	} else if (strlen(subject) != 0 && (cmatch(DOT, '.', &tcre[0]) || cmatch(ORD, subject[0], &tcre[0]))) {
		return amatch(&subject[1], &tcre[1]);
	} else {
		return 0;
	}
}

static tcre_t *compile(char *pattern) {
	tcre_t *tcre;
	size_t len;
	size_t i, j, k;
	int invert;

	if (pattern == NULL) {
		return NULL;
	}

	len = strlen(pattern) + 1;
	tcre = (tcre_t *) realloc(NULL, sizeof(tcre_t) * len);
	if (tcre == NULL) {
		return NULL;
	}
	memset(tcre, '\0', sizeof(tcre_t) * len);

	for (invert = j = i = 0; i < len; i++, j++) {
		switch (pattern[i]) {
			case '[':
				tcre[j].type = ORD;
				if (pattern[++i] == '^') {
					invert = 1;
				}
				while (pattern[i] != ']') {
					if (pattern[i+1] == '-' && pattern[i+2] != ']') {
						size_t x, y;
						x = pattern[i];
						y = pattern[i+2];

						for (x = pattern[i], y = pattern[i+2]; x <= y; x++) {
							tcre[j].c[(unsigned char)x] = 1;
						}

						i+=2;
					} else {
						tcre[j].c[(unsigned char)pattern[i]] = 1;
						i++;
					}
				}
				if (invert) {
					for (k = 0; k < UCHAR_MAX; k++) {
						tcre[j].c[k] = !tcre[j].c[k];
					}
				}
				break;
			case '?':
				tcre[j].type = QUEST;
				tcre[j].c[(unsigned char)pattern[i]] = 1;
				break;
			case '+':
				tcre[j].type = PLUS;
				tcre[j].c[(unsigned char)pattern[i]] = 1;
				break;
			case '*':
				tcre[j].type = STAR;
				tcre[j].c[(unsigned char)pattern[i]] = 1;
				break;
			case '.':
				tcre[j].type = DOT;
				tcre[j].c[(unsigned char)pattern[i]] = 1;
				break;
			case '^':
				tcre[j].type = LANCHOR;
				tcre[j].c[(unsigned char)pattern[i]] = 1;
				break;
			case '$':
				tcre[j].type = RANCHOR;
				tcre[j].c[(unsigned char)pattern[i]] = 1;
				break;
			case '\\':
				tcre[j].type = ORD;
				i++;
				tcre[j].c[(unsigned char)pattern[i]] = 1;
				break;
			default:
				tcre[j].type = ORD;
				tcre[j].c[(unsigned char)pattern[i]] = 1;
				break;
		}
	}

	return tcre;
}

int match(char *subject, char *pattern) {
	int rc, result;
	size_t i;
	tcre_t *tcre;

	tcre = compile(pattern);

	if (subject == NULL || tcre == NULL) {
		result = 0;
	} else if (strlen(subject) == 0 && cmatch(ORD, '\0', &tcre[0])) {
		result = 1;
	} else if (cmatch(ORD, '\0', &tcre[1]) && (cmatch(LANCHOR, '^', &tcre[0]) || cmatch(RANCHOR, '$', &tcre[0]))) {
		result = 1;
	} else if (cmatch(LANCHOR, '^', &tcre[0])) {
		rc = amatch(subject, &tcre[1]);
		if (rc == 1) {
			result = 1;
		} else {
			result = 0;
		}
	} else {
		result = 0;
		for (i = 0; i < strlen(subject); i++) {
			rc = amatch(&subject[i], tcre);
			if (rc == 1) {
				result = 1;
				break;
			}
		}
	}

	free(tcre);

	return result;
}