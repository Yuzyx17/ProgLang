#include "include/parser.h"

void exec_parser(parser_t *parser) {
    node_t *node = program(parser);
    grammarOf(node, 0);
    free(parser);
    free(node);
    printf("\nSuccess");
}
parser_t *parser_init(token_t **tokens) {
    parser_t *parser = (parser_t *)malloc(sizeof(parser_t));
    parser->toks = tokens;
    parser->i = 0;
    parser->tok = parser->toks[parser->i];
    return parser;
}

void parser_advance(parser_t *parser) {
    parser->i++;
    parser->tok = parser->toks[parser->i];
}
void parser_recede(parser_t *parser) {
    parser->i--;
    parser->tok = parser->toks[parser->i];
}
int nullCursor(parser_t *parser) {
    if (parser->tok == (void *)0) {
        return true;
    }
    return false;
}
int endOfTokenList(parser_t *parser) {
    if (sizeof(parser->toks) / sizeof(parser->tok) >= parser->i) {
        return true;
    }
    return false;
}
token_t *parser_peek(parser_t *parser) { return parser->tok; }
token_t *parser_advance_peek(parser_t *parser) {
    token_t *token = parser->toks[parser->i + 1];
    return token;
}
token_t *parser_previous_peek(parser_t *parser) {
    token_t *token = parser->toks[parser->i - 1];
    return token;
}

node_t *parser_match(parser_t *parser, type token_to_match) {
    if (nullCursor(parser)) {
        return error(parser, "Token Mismatch");
    } else if (parser->tok->type == token_to_match) {
        return _identifier(parser);
    } else {
        return error(parser, "Token Mismatch");
    }
}
node_t *parser_match_tokens(parser_t *parser, int count, ...) {
    va_list args;
    va_start(args, count);
    type tok;
    for (int i = 0; i < count; i++) {
        tok = va_arg(args, type);
        if (nullCursor(parser)) {
            return error(parser, "End of Line");
        } else if (parser->tok->type == tok) {
            return _identifier(parser);
        }
    }
    return error(parser, "Token Mismatch or Missing");
}

node_t *match_node(node_t *node, parser_t *parser, nodeType node_type) {
    if (node->type == node_type) {
        return node;
    }
    return error(parser, "Inspection error");
}
node_t *match_nodes(node_t *node, parser_t *parser, int count, ...) {
    va_list args;
    va_start(args, count);
    int tok;
    for (int i = 0; i < count; i++) {
        tok = va_arg(args, int);
        if (node == NULL) {
            return error(parser, "Missing expression");
        } else if (node->type == tok) {
            return node;
        }
    }
    return false;
}

int match(parser_t *parser, type token_to_match) {
    if (nullCursor(parser)) {
        return false;
    } else if ((parser_peek(parser)->type == token_to_match)) {
        // printf("DELT: %s\n", parser_peek(parser)->value);
        parser_advance(parser);
        return true;
    } else {
        return false;
    }
}
int check(parser_t *parser, type token_to_match) {
    if (nullCursor(parser)) {
        return false;
    } else if ((parser_peek(parser)->type == token_to_match)) {
        return true;
    } else {
        return false;
    }
}

int match_tokens(parser_t *parser, int count, ...) {
    va_list args;
    va_start(args, count);
    int tok;
    for (int i = 0; i < count; i++) {
        tok = va_arg(args, int);
        if (nullCursor(parser)) {
            return false;
        } else if ((parser->tok->type == tok)) {
            // printf("DELT: %s\n", parser_peek(parser)->value);
            parser_advance(parser);
            return true;
        }
    }
    return false;
}
int check_tokens(parser_t *parser, int count, ...) {
    va_list args;
    va_start(args, count);
    int tok;
    for (int i = 0; i < count; i++) {
        tok = va_arg(args, int);
        if (nullCursor(parser)) {
            return false;
        } else if ((parser_peek(parser)->type == tok)) {
            return true;
        }
    }
    return false;
}

void grammarOf(node_t *node, int depth) {
    if (node == NULL) {
        return;
    }
    switch (node->type) {
        case PROGRAM_GRAMMAR:
            printf("<program> => <stmts>");
            grammarOf(node->value.program->statement, depth);
            break;
        case STATEMENTS_GRAMMAR:
            printf("\n<stmts> => <stmt> | <stmt> <stmts>");
            for (int x = 0; x < node->value.stmts->stmtCount || x > 1000; x++) {
                grammarOf(node->value.stmts->stmts[x], depth);
            }
            break;
        case STATEMENT_GRAMMAR:
            printf("\n<stmt> => ");
            grammarOf(node->value.stmt->stmt, depth);
            break;
        case ASSIGN_STMTS_GRAMMAR:
            printf("<assgn_stmt>\n<assgn_stmt> => ");
            grammarOf(node->value.assgn->identifier, depth);
            grammarOf(node->value.assgn->assignType, depth);
            grammarOf(node->value.assgn->expr, depth);
            break;
        case DEC_STMTS_GRAMMAR:
            printf("<dec_stmt>\n<dec_stmt> => ");
            grammarOf(node->value.declaration->dataType, depth);
            grammarOf(node->value.declaration->identifier, depth);
            grammarOf(node->value.declaration->assignType, depth);
            grammarOf(node->value.declaration->expr, depth);
            break;
        case TERMINAL:
        case OPERATORS:
        case CONST:
        case IDENTIFIER:
            printf("<%s> ", token_type[node->value.atom->nodeToken->type]);
            break;
        case HABANG_GRAMMAR:
            printf("<habang_stmt>\n<habang_stmt> => ");
            grammarOf(node->value._habang->expr, depth);
            grammarOf(node->value._habang->stmts, depth);
            break;
        case KUNG_GRAMMAR:
            printf("<kung_stmt> => ");
            grammarOf(node->value._kung->condition, depth);
            grammarOf(node->value._kung->stmts, depth);
            grammarOf(node->value._kung->sakali, depth);
            grammarOf(node->value._kung->kundi, depth);
            break;
        case SAKALI_GRAMMAR:
            printf("\n<sakali> => ");
            grammarOf(node->value._sakali->condition, depth);
            grammarOf(node->value._sakali->stmts, depth);
            grammarOf(node->value._sakali->nextelseif, depth);
            if (node->value._sakali != NULL) {
            }
            break;
        case KUNDI_GRAMMAR:
            printf("\n<kundi> => ");
            grammarOf(node->value._kundi->stmt, depth);
            if (node->value._kundi != NULL) {
            }
            break;
        case PARA_GRAMMAR:
            printf("<para>\n<para> => ");
            grammarOf(node->value._para->variable, depth);
            grammarOf(node->value._para->condition, depth);
            grammarOf(node->value._para->iterator, depth);
            grammarOf(node->value._para->stmts, depth);
            break;
        case LOGICAL_GRAMMAR:
        case RELATIONAL_GRAMMAR:
        case ARITHMETIC_GRAMMAR:
            printf("<expr>\n<expr> => ");
            grammarOf(node->value._expression->left, depth);
            grammarOf(node->value._expression->operation, depth);
            grammarOf(node->value._expression->right, depth);
            break;
        case EXPRESSION:
            if (node->value.__expression->operation != NULL) {
                printf("<expr>\n<expr> => ");
            }
            grammarOf(node->value.__expression->left, depth);
            grammarOf(node->value.__expression->operation, depth);
            grammarOf(node->value.__expression->right, depth);
            grammarOf(node->value.__expression->operation2, depth);
            grammarOf(node->value.__expression->next_expr, depth);
            break;
        case UNARY_GRAMMAR:
            printf("<expr>\n <expr> => ");
            grammarOf(node->value.unary->token, depth);
            grammarOf(node->value.unary->operation, depth);
            break;
        case ERROR:
            printf("\n%s in Line:%d Pos:%d At (%s)\n", node->value.error->error,
                   node->value.error->token->lpos,
                   node->value.error->token->cpos,
                   node->value.error->token->value);
            break;
        case KUHA_GRAMMAR:
            printf("<kuha_stmt>\n<kuha_stmt> => ");
            grammarOf(node->value.kuha->stringFormat, depth);
            grammarOf(node->value.kuha->varAddress, depth);
            printf("\n");
            break;
        case LAHAD_GRAMMAR_A:
            printf("<lahad_stmt>\n<lahad_stmt> => ");
            grammarOf(node->value.printString->stringValue, depth);
            printf("\n");
            break;
        case LAHAD_GRAMMAR_B:
            printf("<lahad_stmt>\n<lahad_stmt> => ");
            grammarOf(node->value.printExpression->stringFormat, depth);
            grammarOf(node->value.printExpression->expr, depth);
            printf("\n");
            break;
        case LAHAD_GRAMMAR_C:
            printf("<lahad_stmt>\n<lahad_stmt> => ");
            grammarOf(node->value.printValue->stringFormat, depth);
            grammarOf(node->value.printValue->identifier, depth);
            printf("\n");
            break;
        default:
            break;
    }
}

void grammarIndent(int indent_count) {
    for (int x = 0; x < indent_count - 1; x++) {
        printf("\t");
    }
}
void indent(int indent_count) {
    for (int x = 0; x < indent_count - 1; x++) {
        printf("\t");
        if (x == indent_count - 2)
            printf("|");
    }
    if (indent_count > 0)
        printf("-->");
}
