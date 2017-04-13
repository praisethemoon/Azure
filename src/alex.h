/* See Copyright Notice in azure.h. */

#ifndef _ALEX_H_
#define _ALEX_H_

#include "azure.h"
#include "aregex.h"

#define AZ_MAX_PATH_LEN 10240

typedef enum
{
	/* Reserved words. */
	ADVICE=256, BREAK, CASE, CATCH, CONTINUE, DEF, DEFAULT, DO, ELSE, FOR,
	FOREACH, IF, _IN, INCLUDE, NIL, POINTCUT, REQUIRE, RETURN, SWITCH, THROW,
	TRY, VAR, WHILE, YIELD,
	/* Others. */
	IDENTIFIER, INTEGER, FLOATING, LITERAL, COMMENT, SPACE, DOUBLEDOT,
	TRIPLEDOT, INC, DEC, PREINC, POSTINC, PREDEC, POSTDEC, PASS, MASS, MULASS,
	DASS, MODASS, LSASS, RSASS, BAASS, BOASS, BXASS, LEQ, GEQ, EQUAL, NOTEQUAL,
	LOGAND, LOGOR, TCONSTRUCT, LSHIFT, RSHIFT, ENDOFFILE, UNBOUNDEDCOMMENT,
	UNBOUNDEDLITERAL, SCOPE, FUNC, INDEX, UNDEFINED, UKERROR
} eToken;

typedef struct
{
	int token;
	char buf[AZ_MAX_TOKEN_LEN], *dbuf;
	uint dbuflen;
	uint seencr;	/* # of CR before this token? */
} _token;

typedef struct
{
	int token;
	char* str;
	char* pattern;
} token_name;

/* Tokens, scanned by greedy matching. Change of ordering may induce bugs. */
static const token_name tokens[] =
{
	{IDENTIFIER, "", "(_|\\$|\\a)(_|\\a|\\d)*"},	/* must precede keywords */
	/* keywords */
	{ADVICE, "advice", ""},
	{BREAK, "break", ""},
	{CASE, "case", ""},
	{CATCH, "catch", ""},
	{CONTINUE, "continue", ""},
	{DEF, "def", ""},
	{DEFAULT, "default", ""},
	{DO, "do", ""},
	{ELSE, "else", ""},
	{FOR, "for", ""},
	{FOREACH, "foreach", ""},
	{IF, "if", ""},
	{_IN, "in", ""},
	{INCLUDE, "use", ""},
	{NIL, "nil", ""},
	{POINTCUT, "pointcut", ""},
	{REQUIRE, "require", ""},
	{RETURN, "return", ""},
	{SWITCH, "switch", ""},
	{THROW, "throw", ""},
	{TRY, "try", ""},
	{VAR, "var", ""},
	{WHILE, "while", ""},
	{YIELD, "yield", ""},
	/* multi-character tokens */
	{FLOATING, "", "(\\d*\\.\\d+([eE]?[-+]?\\d+)?)\
					|(\\d+\\.\\d*([eE]?[-+]?\\d+)?)|(\\d+[eE][-+]?\\d+)"},
	{INTEGER, "", "(0x[a-fA-F0-9]+)|(\\d+)"},
	{TRIPLEDOT, "...", "\\.\\.\\."},
	{DOUBLEDOT, "..", "\\.\\."},
	{INC, "++", "\\+\\+"},
	{DEC, "--", "--"},
	{PASS, "+=", "\\+="},
	{MASS, "-=", "-="},
	{MULASS, "*=", "\\*="},
	{DASS, "/=", "/="},
	{MODASS, "%=", "%="},
	{LSASS, "<<=", "<<="},
	{RSASS, ">>=", ">>="},
	{BAASS, "&=", "&="},
	{BOASS, "|=", "|="},
	{BXASS, "^=", "\\^="},
	{LEQ, "<=", "<="},
	{GEQ, ">=", ">="},
	{EQUAL, "==", "=="},
	{NOTEQUAL, "!=", "!="},
	{LOGAND, "&&", "&&"},
	{LOGOR, "||", "\\|\\|"},
	{LSHIFT, "<<", "<<"},
	{RSHIFT, ">>", ">>"},
	/* single-character tokens */
	{'(', "(", "\\("}, {')', ")", "\\)"}, {'[', "[", "\\["}, {']', "]", "\\]"},
	{'{', "{", "\\{"}, {'}', "}", "\\}"}, {'.', ".", "\\."}, {',', ",", ","},
	{':', ":", ":"}, {';', ";", ";"}, {'\\', "\\", "\\"}, {'+', "+", "\\+"},
	{'-', "-", "-"}, {'*', "*", "\\*"}, {'/', "/", "/"}, {'%', "%", "%"},
	{'=', "=", "="}, {'<', "<", "<"}, {'>', ">", ">"}, {'?', "?", "\\?"},
	{'!',"!","!"}, {'&',"&","&"}, {'|',"|","|"}, {'^',"^","\\^"},
	{'~', "~", "~"}
};

typedef struct
{
	regnode* reg;
	char first;
} REGNODE;

typedef struct
{
	char source[AZ_MAX_PATH_LEN];	/* source path */
	char* buf;						/* buffer for the source */
	long pos;						/* character position in the stream */
	_token token, next_token;		/* current and next tokens */
	int expected;					/* expected next token */
	REGNODE regtrees[sizeof(tokens)/sizeof(token_name)];
	uint line_num;
	char* lineerrors;	/* tracks the line error */
	uint line_size;		/* size of lineerrors */
} LexInfo;

int clLexOpen(LexInfo* li, const char* source, char* buf);
void clLexClose(LexInfo* li);
int clStep(LexInfo* li);
char* clGetTokenName(int token);
void freetoken(_token* tok);
int copytoken(_token* dest, _token* source);

#endif
