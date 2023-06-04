#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "speedtypes.h"

typedef enum
{
	GOTO,
	LABEL,
	BRANCH,
	DUP,
	SWAP,
	PUSH,
	POP,
	ADD,
	SUB,
	DIV,
	MUL,
	CMP,
	OR, // all bitwise operators, I have no boolean operators in this abomination
	AND,
	NOT,
	XOR,
	MOD,
	CHR, // use top of stack as index to the previous string.
	STR, // Use string, limited to ADD and CMP and also can be used in MUL with another (enumerable value)? I dont have numbers Idk what to call it
	PRINT,
	PUTS,
	END,
} token_type;

typedef struct
{
	token_type type;
	char *text;
	size_t start;
	size_t end;
} token_t;

typedef struct
{
	char *src;
	size_t position;
} scanner_t;

scanner_t *create_scanner_t(char *source)
{
	// I dont think I'd even need to free this?
	// I dont have much reason to. It exists the whole program.
	// You'll figure it out.
	// NEVER MIND HOLY----
	// you do have to free it
	// this is interpreted.. so it HAS to be freed lmao.
	scanner_t *scanner = malloc(sizeof(scanner_t));
	scanner->src = source;
	scanner->position = 0;
	return scanner;
}
// I am too lazy to type out the full names
#define current scanner->src[scanner->position]
#define pos scanner->position
#define source scanner->src

token_t scan_comment(scanner_t *scanner)
{
	++pos;
	if (current == '/')
	{
		while (current && current != '\n')
		{
			++pos;
		}
	}
}

token_t scan_push(scanner_t *scanner)
{
	++pos;
	size_t start = pos;

	while (current && current != '"')
	{
		++pos;
	}

	size_t end = pos;
	size_t length = end - start;
	pos = start;
	char *text = malloc(length + 1);
	text[length] = 0;
	while (pos < end)
	{
		text[pos - start] = source[pos];
		++pos;
	}
	++pos;
	return (token_t){PUSH, text, start, end};
}

char *collect_keyword(size_t *_start, size_t *_end, scanner_t *scanner)
{
	size_t start = pos;

	while (current >= 'a' && current <= 'z' || current >= 'A' && current <= 'Z')
	{
		++pos;
	}

	size_t end = pos;
	size_t length = end - start;
	pos = start;
	char *text = malloc(length + 1);
	text[length] = 0;
	while (pos < end)
	{
		text[pos - start] = source[pos];
		++pos;
	}
	*_start = start;
	*_end = end;
	return text;
}

token_t scan_keyword(scanner_t *scanner)
{
	size_t start = 0;
	size_t end = 0;
	char *text = collect_keyword(&start, &end, scanner);
	if (current == ':')
	{
		++pos;
		return (token_t){LABEL, text, start, end};
	}
	++pos;
	if (!strcmp(text, "pop"))
	{
		return (token_t){POP, text, start, end};
	}
	if (!strcmp(text, "cmp"))
	{
		return (token_t){CMP, text, start, end};
	}
	if (!strcmp(text, "goto"))
	{
		while (!(current >= 'a' && current <= 'z'))
		{
			++pos;
		}
		char *target = collect_keyword(&start, &end, scanner);
		return (token_t){GOTO, target, start, end};
	}
	if (!strcmp(text, "branch"))
	{
		while (!(current >= 'a' && current <= 'z'))
		{
			++pos;
		}
		char *target = collect_keyword(&start, &end, scanner);
		return (token_t){BRANCH, target, start, end};
	}
	if (!strcmp(text, "add"))
	{
		return (token_t){ADD, text, start, end};
	}
	if (!strcmp(text, "sub"))
	{
		return (token_t){SUB, text, start, end};
	}
	if (!strcmp(text, "div"))
	{
		return (token_t){DIV, text, start, end};
	}
	if (!strcmp(text, "mul"))
	{
		return (token_t){MUL, text, start, end};
	}
	if (!strcmp(text, "or"))
	{
		return (token_t){OR, text, start, end};
	}
	if (!strcmp(text, "and"))
	{
		return (token_t){AND, text, start, end};
	}
	if (!strcmp(text, "xor"))
	{
		return (token_t){XOR, text, start, end};
	}
	if (!strcmp(text, "mod"))
	{
		return (token_t){MOD, text, start, end};
	}
	if (!strcmp(text, "swap"))
	{
		return (token_t){SWAP, text, start, end};
	}
	if (!strcmp(text, "dup"))
	{
		return (token_t){DUP, text, start, end};
	}
	if(!strcmp(text, "print"))
	{
		return (token_t){PRINT, text, start ,end};
	}
	if(!strcmp(text, "puts"))
	{
		return (token_t){PUTS, text, start, end};
	}
}

token_t scan_chr(scanner_t *scanner)
{
	++pos;
	if (current >= 'a' && current <= 'z' || current >= 'A' && current <= 'Z')
	{
		token_t keyword = scan_keyword(scanner);
		keyword.type |= CHR;
		++pos;
		return keyword;
	}
}

token_t scan_str(scanner_t *scanner)
{
	++pos;
	if (current >= 'a' && current <= 'z' || current >= 'A' && current <= 'Z')
	{
		token_t keyword = scan_keyword(scanner);
		keyword.type |= STR;
		++pos;
		return keyword;
	}
}

token_t scan(scanner_t *scanner)
{
	if (current == ' ' || current == '\n' || current == '\t')
	{
		++pos;
		return scan(scanner);
	}
	if (current == '/')
	{
		return scan_comment(scanner);
	}
	if (current == '"')
	{
		return scan_push(scanner);
	}
	if (current >= 'a' && current <= 'z' || current >= 'A' && current <= 'Z')
	{
		return scan_keyword(scanner);
	}
	if (current == '?')
	{
		return scan_chr(scanner);
	}
	if (current == '~')
	{
		return scan_str(scanner);
	}
	if (current == '\0')
	{
		return (token_t){END, "", -1, 0};
	}
}

#undef current
#undef pos
#undef source

linked_list scan_all_tokens(char *text)
{
	scanner_t *scanner = create_scanner_t(text);
	linked_list list = {0};
	token_t *token = malloc(sizeof(token_t));
	while (scanner->position < strlen(scanner->src) && token->type != END)
	{
		*token = scan(scanner);
		token_t *tok = malloc(sizeof(token_t));
		*tok = *token;
		append_node(&list, initialize_node(tok));
	}
	free(token);
	return list;
}
