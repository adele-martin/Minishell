

#include "../header/minishell.h"
typedef enum {
    TOKEN_COMMAND,
    TOKEN_ARGUMENT,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char* value;
} Token;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Token* tokenize(const char* input, int* token_count) {
    int capacity = 10;
    Token* tokens = malloc(capacity * sizeof(Token));
    *token_count = 0;

    const char* current = input;
    while (*current != '\0') {
        // Skip whitespace
        while (isspace(*current)) {
            current++;
        }

        if (*current == '\0') {
            break;
        }

        // Ensure there is enough space for new tokens
        if (*token_count >= capacity) {
            capacity *= 2;
            tokens = realloc(tokens, capacity * sizeof(Token));
        }

        // Handle brackets
        if (*current == '(') {
            tokens[*token_count].type = TOKEN_LEFT_PAREN;
            tokens[*token_count].value = strndup(current, 1);
            (*token_count)++;
            current++;
            continue;
        } else if (*current == ')') {
            tokens[*token_count].type = TOKEN_RIGHT_PAREN;
            tokens[*token_count].value = strndup(current, 1);
            (*token_count)++;
            current++;
            continue;
        }

        // Handle logical operators
        if (strncmp(current, "&&", 2) == 0) {
            tokens[*token_count].type = TOKEN_AND;
            tokens[*token_count].value = strndup(current, 2);
            (*token_count)++;
            current += 2;
            continue;
        } else if (strncmp(current, "||", 2) == 0) {
            tokens[*token_count].type = TOKEN_OR;
            tokens[*token_count].value = strndup(current, 2);
            (*token_count)++;
            current += 2;
            continue;
        }

        // Handle commands and arguments
        const char* start = current;
        while (*current != '\0' && !isspace(*current) && *current != '(' && *current != ')' &&
               strncmp(current, "&&", 2) != 0 && strncmp(current, "||", 2) != 0) {
            current++;
        }

        tokens[*token_count].type = (start == input || tokens[*token_count - 1].type == TOKEN_AND || 
                                     tokens[*token_count - 1].type == TOKEN_OR || 
                                     tokens[*token_count - 1].type == TOKEN_LEFT_PAREN) ? 
                                     TOKEN_COMMAND : TOKEN_ARGUMENT;
        tokens[*token_count].value = strndup(start, current - start);
        (*token_count)++;
    }

    return tokens;
}
int main() {
    const char* input = "echo (hello && echo world) || (echo bye)";
    int token_count;
    Token* tokens = tokenize(input, &token_count);

    for (int i = 0; i < token_count; i++) {
        printf("Token %d: Type %d, Value %s\n", i, tokens[i].type, tokens[i].value);
        free(tokens[i].value);
    }

    free(tokens);
    return 0;
}



// t_list	read_tokens(char *input)
// {
// 	t_list	*token_list;
// }
/*
CARE PRECEDENCE OF OPERATORS!

1. Redirections (<, >, >>, <>, <&. >& and >>-, as well as here-docs
	<<delimiter and here-strings <<<word) are roughly the same as 
	command-line arguments, and can appear anywhere in a simple command, 
	including before the command word. Effectively, they bind most tightly, 
	as with postfix operators in most languages.
2. Pipes (|) are the strongest binary operator. They associate to the left.
3. Finally come the short-circuiting booleans (&& and ||). 
	Unlike many languages, these have the same precedence. 
	They also associate to the left.



control operator: A token that performs a control function. 
	It is a newline or one of the following: 
	‘||’, ‘&&’, ‘&’, ‘;’, ‘;;’, ‘;&’, ‘;;&’, ‘|’, ‘|&’, ‘(’, or ‘)’.
metacharacter: A character that, when unquoted, separates words. 
	A metacharacter is a space, tab, newline, or one of the following characters: 
	‘|’, ‘&’, ‘;’, ‘(’, ‘)’, ‘<’, or ‘>’. 
token: A sequence of characters considered a single unit by the shell. 
	It is either a word or an operator.
word: A sequence of characters treated as a unit by the shell.
	Words may not include unquoted metacharacters. 


Important for lexing:
	- parenthesis for priority				()
	- single quotes (no interpretation)		''
	- double quotes (only $-interpretation)	""
	- dollar sign (environment variables)	$
	- exit status of foreground pipeline	$?
	- wildcards in curr. working directory	*

	- redirect input 						<
	- redirect output 						>
	- read input until delimiter 			<<
	- redirect output in append mode 		>>
	- output to next input -> pipes 		|
	- AND-execution							&&
	- OR-execution							||

Not interpret unclosed quotes or special characters which are not required by the
subject such as \ (backslash) or ; (semicolon).
*/

/*
GET ENVIRONMENTS: env / printenv
GET VALUE OF VAR: " printenv varname "
EXPORT GLOBAL VAR: " export varname=value "
EXPORT LOCAL VAR: " varname =value " (or " set varname =value "
CREATE NEW ENV VAR: " export varname=value "
REMOVE ENV VAR: " unset varname "
*/

/*
Fehler in der Ausgabe umleiten:
Find everything in root (/) ending in .log, where errors (2) sent to /dev/null
find / -name *.log 2> /dev/null 
*/

/*
What is the differnce between ` and ' ???

In summary, single quotes stop all interpretation -- the string is rendered 
literally. Double quotes leave $ (dollar sign), ` (backquote) as special, 
and \ (backslash) as special when followed by certain other characters. 
And ! will be treated specially
*/

