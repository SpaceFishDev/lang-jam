#include "scanner.h"

char *get_input_file_path(char **argv, int argc)
{
	int i = 1;

	while (i < argc) // && argv[i][0] != '-'){
	{
		if (argv[i][0] != '-')
			return argv[i];
		++i;
	}
	return ".a";
}
char *get_output_file_path(char **argv, int argc)
{
	int i = 1;

	while (i < argc)
	{
		if (!strcmp(argv[i], "-o"))
		{
			return argv[i];
		}
		++i;
	}
	return ".o";
}
typedef enum
{
	iadd,
	imul,
	idiv,
	isub,
	imod,
	iand,
	ior,
	ixor,
	ipop,
	ipush,
	ijump,
	ibranch,
	icompare,
	istradd,
	istrmul,
	iprint,
	iputs,
	iswap,
	idup,
} instruction_type;

#define ins_to_string(type) (((char*[]){"add", "mul", "div", "sub", "mod", "and", "or", "xor", "pop", "push", "jmp", "branch", "cmp", "str_add", "str_mul", "print", "puts", "swap", "dup"})[type])

typedef struct
{
	token_t token;
	instruction_type type;
	char *arg; // optional.
} instruction_t;

#define add_ins (append_node(&instructions, initialize_node(instruction)));
#define current (llat(token_t, &tokens, i))

char* read_file(char* path)
{
	FILE *fp;
	long lSize;
	char *buffer;

	fp = fopen ( path , "r" );

	fseek( fp , 0L , SEEK_END);
	lSize = ftell( fp );
	rewind( fp );

	/* allocate memory for entire content */
	buffer = calloc( 1, lSize+1 );

	/* copy the file into the buffer */
	if( 1!=fread( buffer , lSize, 1 , fp) )
	{
		printf("oopsie.\n");
	}
	return buffer;
}

linked_list compile(linked_list tokens)
{
	linked_list instructions = {0};
	hash_map *labels = init_hash_map();
	for(int i = 0; i < tokens.length; ++i)
	{
		instruction_t *instruction = malloc(sizeof(instruction_t));
		switch (current->type)
		{
			case PUSH:
				{
					*instruction = (instruction_t){*current, ipush, current->text};
				}
			break;
			case POP:
				{
					*instruction = (instruction_t){*current, ipop, ""};
				}
			break;
			case ADD:
				{
					*instruction = (instruction_t){*current, iadd, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, iswap, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, ipop, ""};
				}
			break;
			case SUB:
				{
					*instruction = (instruction_t){*current, isub, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, iswap, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, ipop, ""};
				}
			break;
			case DIV:
				{
					*instruction = (instruction_t){*current, idiv, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, iswap, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, ipop, ""};
				}
			break;
			case MUL:
				{
					*instruction = (instruction_t){*current, imul, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, iswap, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, ipop, ""};
				}
			break;
			case OR:
				{
					*instruction = (instruction_t){*current, ior, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, iswap, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, ipop, ""};
				}
			break;
			case AND:
				{
					*instruction = (instruction_t){*current, iand, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, iswap, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, ipop, ""};
				}
			break;
			case XOR:
				{
					*instruction = (instruction_t){*current, ixor, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, iswap, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, ipop, ""};
				}
			break;
			case MOD:
				{
					*instruction = (instruction_t){*current, imod, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, iswap, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, ipop, ""};
				}
			break;
			case PRINT:
				{
					*instruction = (instruction_t){*current, iprint, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, ipop, ""};
				} 
			break;
			case PUTS:
				{
					*instruction = (instruction_t){*current, iputs, ""};
					add_ins;
					instruction = malloc(sizeof(instruction_t));
					*instruction = (instruction_t){(token_t){0}, ipop, ""};
				}
			break;
			case DUP:
			{
				*instruction = (instruction_t){*current, idup, ""};
			}
			break;
			case SWAP:
			{
				*instruction = (instruction_t){*current, iswap, ""};
			}
			break;
			case LABEL:
			{
				size_t* insptr = malloc(sizeof(size_t)); 
				*insptr = instructions.length;
				hash_map_element* element = init_element(insptr, current->text);	
				continue;
			}
			break;
			case END:
			{
				return instructions;
			}
			break;
			default:
				{
					*instruction = (instruction_t){*current, iprint, "test"};
				} 
			break;
		}
		add_ins;
	}
	return instructions;
}

int main(int argc, char **argv)
{
	char *fp = get_input_file_path(argv, argc);
	char *fp_out = get_output_file_path(argv, argc);
	char* input_file = read_file(fp);
	linked_list tokens = scan_all_tokens(input_file);
	linked_list instructions = compile(tokens);
	for(int i = 0; i < instructions.length; ++i)
	{
		printf("%s\n", ins_to_string(llat(instruction_t, &instructions, i)->type));
	}
}
