#ifndef NODES
#define NODES

#include "stdarg.h"
#include "stdio.h"
#include "stdlib.h"
#include "token.h"

typedef struct node_t node_t;
typedef struct parser_t parser_t;
// Nodetype enums
typedef enum {
    ARITHMETIC_GRAMMAR,
    ASSIGN_STMTS_GRAMMAR,
    BOOLEAN_GRAMMAR,
    CON_STMTS,
    CONST,
    DATA_TYPE,
    OPERATORS,
    TERMINAL,
    DEC_STMTS_GRAMMAR,
    ERROR,
    HABANG_GRAMMAR,
    IDENTIFIER,
    ITER_STMTS_GRAMMAR,
    KEYWORD,
    KUNDI_GRAMMAR,
    KUNG_GRAMMAR,
    LAHAD_GRAMMAR_A,
    LAHAD_GRAMMAR_B,
    LAHAD_GRAMMAR_C,
    LOGICAL_GRAMMAR,
    PARA_GRAMMAR,
    PROGRAM_GRAMMAR,
    RELATIONAL_GRAMMAR,
    SAKALI_GRAMMAR,
    KUHA_GRAMMAR,
    STATEMENT_GRAMMAR,
    STATEMENTS_GRAMMAR,
    TEMP_DEC,
    UNARY_GRAMMAR
} nodeType;
typedef struct {
    node_t **stmts;
    int stmtCount;
} statementsNode;
typedef struct {
    node_t *stmt;
} statementNode, kundiNode;
typedef struct {
    node_t *statement;
} programNode;
typedef struct {
    char *error;
    token_t *token;
} errorNode;
typedef struct {
    token_t *nodeToken;
} tokenNode;
typedef struct {
    node_t *variable;
    node_t *condition;
    node_t *iterator;
    node_t *stmts;
} paraNode;
typedef struct {
    node_t *expr;
    node_t *stmts;
} habangNode;  // *WHILE
typedef struct {
    node_t *operation;
    node_t *token;
} unaryNode;  // *UNARY
typedef struct {
    node_t *left;       // expr
    node_t *operation;  // tokenNode
    node_t *right;      // expr
} expressionNode;
typedef struct {
    node_t *dataType;
    node_t *identifier;
    node_t *assignType;
    node_t *expr;
} declarationNode;
typedef struct {
    node_t *identifier;
    node_t *assignType;
    node_t *expr;
} assgnNode;
typedef struct {
    node_t *condition;
    node_t *stmts;
    node_t *sakali;
    node_t *kundi;
} kungNode;
typedef struct {
    node_t *condition;
    node_t *stmts;
    node_t *nextelseif;
} sakaliNode;
typedef struct {
    node_t *dataType;
    node_t *identifier;
} noValueDeclarationNode;
typedef struct {
    node_t *stringFormat;
    node_t *varAddress;
} kuhaNode;
typedef struct {
    node_t *stringValue;
} printStringNode;
typedef struct {
    node_t *stringFormat;
    node_t *identifier;
} printValueNode;
typedef struct {
    node_t *stringFormat;
    node_t *expr;
} printExp;

typedef union {
    programNode *program;
    statementsNode *stmts;
    statementNode *stmt;
    habangNode *_habang;
    paraNode *_para;
    assgnNode *assgn;
    expressionNode *_expression;
    unaryNode *unary;
    tokenNode *atom;
    errorNode *error;
    kungNode *_kung;
    sakaliNode *_sakali;
    kundiNode *_kundi;
    noValueDeclarationNode *_declaration;
    declarationNode *declaration;
    kuhaNode *kuha;
    printStringNode *printString;
    printValueNode *printValue;
    printExp *printExpression;
} nodeValue;

struct node_t {
    nodeType type;
    nodeValue value;
};

// Generate the nodes

// allocates a memory for the node
node_t *createNode();

/**
 * @brief generates an errorNode, replaces the expected Node
 * @warning avoid usage for delimeters < , [] {} () ; >
 * @param parser
 * @param errorMsg message
 * @return errorNode
 */
node_t *error(parser_t *parser, char *errorMsg);

// program = stmts
node_t *program(parser_t *parser);
node_t *stmts(parser_t *parser, node_t *parent);
node_t *stmt(parser_t *parser);

node_t *iterative_stmt(parser_t *parser);
node_t *habang(parser_t *parser);
node_t *para(parser_t *parser);

node_t *conditional_stmt(parser_t *parser);
node_t *kung(parser_t *parser);
node_t *sakali(parser_t *parser);
node_t *kundi(parser_t *parser);

node_t *declaration_stmt(parser_t *parser);
node_t *assign_stmt(parser_t *parser);

node_t *io_stmts(parser_t *parser);
node_t *kuha(parser_t *parser);
node_t *lahad(parser_t *parser);

node_t *expr(parser_t *parser);

node_t *logical(parser_t *parser);
node_t *relational(parser_t *parser);

node_t *arithmetic(parser_t *parser);
node_t *negate(parser_t *parser);
node_t *unary(parser_t *parser);

node_t *literal(parser_t *parser);

node_t *_const(parser_t *parser);
node_t *_identifier(parser_t *parser);
node_t *_data_type(parser_t *parser);
node_t *_operators(parser_t *parser);

int data_type(parser_t *parser);
int assign_op(parser_t *parser);
int arithemetic_op(parser_t *parser);
int logical_op(parser_t *parser);
int relational_op(parser_t *parser);
int bool_op(parser_t *parser);
// Generates the token nodes
int operators(parser_t *parser);

/**
 * @brief generates the token node and advances
 * @param parser
 * @return atomNode
 */
node_t *_token(parser_t *parser);

#endif