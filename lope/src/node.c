#include "include/parser.h"

node_t *createNode() {
    node_t *node = (node_t *)calloc(1, sizeof(node_t));
    if (node == NULL) {
        printf("\nFailed to allocate memory!\n");
        exit(-1);
    }
    return node;
}
// <program> => <stmts>
node_t *program(parser_t *parser) {
    // <program>
    node_t *node = createNode();
    node->type = PROGRAM_GRAMMAR;
    node->value.program = (programNode *)calloc(1, sizeof(programNode));
    // <program> => <stmts>
    node->value.program->statement = stmts(parser, node);
    return node;
}
// <stmts> => <stmt> | <stmt> <stmts>
node_t *stmts(parser_t *parser, node_t *parent) {
    //<stmts>
    node_t *node = createNode();
    node->type = STATEMENTS_GRAMMAR;
    node->value.stmts = (statementsNode *)calloc(1, sizeof(statementsNode));
    node->value.stmts->stmts = (node_t **)calloc(1, sizeof(nodeValue *));
    node->value.stmts->stmtCount = 0;
    // <stmts> => <stmt> <stmts>
    while (!nullCursor(parser)) {
        if (parent->type != PROGRAM_GRAMMAR && check(parser, RBRACE)) {
            return node;
        } else if (match_tokens(parser, 3, SEMI, COMMENT_VALUE_MULTI,
                                COMMENT_VALUE_SINGLE)) {
            if (nullCursor(parser))
                return node;
            continue;
        }

        node->value.stmts->stmts[node->value.stmts->stmtCount] = stmt(parser);
        node->value.stmts->stmtCount++;
        node->value.stmts->stmts = (node_t **)realloc(
            node->value.stmts->stmts,
            (node->value.stmts->stmtCount + 1) * sizeof(node_t *));
    }
    return node;
}
// <stmt> => <expr> | <dec_stmts> | <assign_stmts> | <con_stmts> | <iter_stmts>
// | <io_stmts>
node_t *stmt(parser_t *parser) {
    node_t *node = createNode();
    node->type = STATEMENT_GRAMMAR;
    node->value.stmt = (statementNode *)calloc(1, sizeof(statementNode));
    // <stmt> => <dec_stmts>
    if (data_type(parser)) {
        node->value.stmt->stmt = declaration_stmt(parser);
        if (check(parser, SEMI))
            parser_advance(parser);
        else if (node->value.stmt->stmt->type != ERROR) {
            node->value.stmt->stmt =
                error(parser, "[Declaration] Missing Semi-colon");
        }
    }
    // <stmt> => <assign_stmts>
    else if (check(parser, ID)) {
        node->value.stmt->stmt = assign_stmt(parser);
        if (check(parser, SEMI))
            parser_advance(parser);
        else if (node->value.stmt->stmt->type != ERROR) {
            node->value.stmt->stmt =
                error(parser, "[Assignment] Missing Semi-colon");
        }
    }
    // <stmt> => <con_stmts>
    else if (check(parser, KUNG)) {
        node->value.stmt->stmt = conditional_stmt(parser);
    }
    // <stmt> => <io_stmts>
    else if (check_tokens(parser, 2, KUHA, LAHAD)) {
        node->value.stmt->stmt = io_stmts(parser);
    }
    // <stmt> => <iter_stmts>
    else if (check_tokens(parser, 2, HABANG, PARA)) {
        node->value.stmt->stmt = iterative_stmt(parser);
    } else {
        node->value.stmt->stmt = error(parser, "[Program] Unexpected Token");
    }
    return node;
}

node_t *expr(parser_t *parser) { return logical_op(parser); }

node_t *iterative_stmt(parser_t *parser) {
    switch (parser->tok->type) {
        case PARA:
            return para(parser);
            break;
        case HABANG:
            return habang(parser);
            break;
        default:
            break;
    }
}
node_t *para(parser_t *parser) {
    node_t *node = createNode();  // FOR
    node->type = PARA_GRAMMAR;
    node->value._para = (paraNode *)calloc(1, sizeof(paraNode));
    if (!match(parser, PARA, node))
        return error(parser, "[Para] Missing Keyword");
    if (!match(parser, LPAREN, node))
        return error(parser, "[Para] Missing Left Parenthesis");
    if (data_type(parser)) {
        node->value._para->variable = declaration_stmt(parser);
    } else if (match(parser, ID, node)) {
        node->value._para->variable = assign_stmt(parser);
    }
    if (!match(parser, SEMI, node))
        return error(parser, "[Para] Missing Semi-colon");
    node->value._para->condition = expr(parser);
    if (!match(parser, SEMI, node))
        return error(parser, "[Para] Missing Semi-colon");
    if (check_tokens(parser, 2, INCR, DECR) ||
        (check(parser, ID) && (parser_advance_peek(parser)->type == INCR ||
                               parser_advance_peek(parser)->type == DECR)))
        node->value._para->iterator = unary_op(parser);
    else {
        node->value._para->iterator = assign_stmt(parser);
    }
    if (!match(parser, RPAREN, node))
        return error(parser, "[Para] Missing Right Paranthesis");
    if (!match(parser, LBRACE, node))
        return error(parser, "[Para] Missing Left Brace");
    node->value._para->stmts = stmts(parser, node);
    if (!match(parser, RBRACE, node))
        return error(parser, "[Para] Missing Right Brace");
    return node;
}
node_t *habang(parser_t *parser) {
    node_t *node = createNode();
    node->type = HABANG_GRAMMAR;
    node->value._habang = (habangNode *)calloc(1, sizeof(habangNode));
    if (!match(parser, HABANG, node))
        return error(parser, "[Habang] Missing Keyword");
    if (!match(parser, LPAREN, node))
        return error(parser, "[Habang] Missing Left Paranthesis");
    node->value._habang->expr = expr(parser);
    if (!match(parser, RPAREN, node))
        return error(parser, "[Habang] Missing Right Parenthesis");
    if (!match(parser, LBRACE, node))
        return error(parser, "[Habang] Missing Left Bracket");
    node->value._habang->stmts = stmts(parser, node);
    if (!match(parser, RBRACE, node))
        return error(parser, "[Habang] Missing Right Bracket");
    return node;
}

node_t *conditional_stmt(parser_t *parser) {
    if (nullCursor(parser))
        return NULL;
    else {
        switch (parser->tok->type) {
            case KUNG:
                return kung(parser);
                break;
            case SAKALI:
                return sakali(parser);
                break;
            case KUNDI:
                return kundi(parser);
                break;
            default:
                return NULL;
        }
    }
}
node_t *kung(parser_t *parser) {
    node_t *node = createNode();
    node->type = KUNG_GRAMMAR;
    if (!match(parser, KUNG, node))
        return error(parser, "[Kung] Missing Keyword");
    node->value._kung = (kungNode *)calloc(1, sizeof(kungNode));
    if (!match(parser, LPAREN, node))
        return error(parser, "[Kung] Missing Left Paranthesis");
    node->value._kung->condition = expr(parser);
    if (!match(parser, RPAREN, node))
        return error(parser, "[Kung] Missing Right Parenthesis");
    if (!match(parser, LBRACE, node))
        return error(parser, "[Kung] Missing Left Bracket");
    node->value._kung->stmts = stmts(parser, node);
    if (!match(parser, RBRACE, node))
        return error(parser, "[Kung] Missing Right Bracket");
    node->value._kung->sakali = conditional_stmt(parser);
    node->value._kung->kundi = conditional_stmt(parser);

    return node;
}
node_t *sakali(parser_t *parser) {
    node_t *node = createNode();
    node->type = SAKALI_GRAMMAR;
    node->value._sakali = (sakaliNode *)calloc(1, sizeof(sakaliNode));
    if (!match(parser, SAKALI, node))
        return error(parser, "[Sakali] Missing keyword");
    if (!match(parser, LPAREN, node))
        return error(parser, "[Sakali] Missing Left Paranthesis");
    node->value._sakali->condition = expr(parser);
    if (!match(parser, RPAREN, node))
        return error(parser, "[Sakali] Missing Right Parenthesis");
    if (!match(parser, LBRACE, node))
        return error(parser, "[Sakali] Missing Left Bracket");
    node->value._sakali->stmts = stmts(parser, node);
    if (!match(parser, RBRACE, node))
        return error(parser, "[Sakali] Missing Right Bracket");
    node->value._sakali->nextelseif = conditional_stmt(parser);
    return node;
}
node_t *kundi(parser_t *parser) {
    node_t *node = createNode();
    node->type = KUNDI_GRAMMAR;
    node->value._kundi = (kundiNode *)calloc(1, sizeof(kundiNode));
    if (!match(parser, KUNDI, node))
        return error(parser, "[Kundi] Missing keyword");
    if (!match(parser, LBRACE, node))
        return error(parser, "[Kundi] Missing Left Bracket");
    node->value._kundi->stmt = stmts(parser, node);
    if (!match(parser, RBRACE, node))
        return error(parser, "[Kundi] Missing Right Bracket");
    return node;
}

node_t *io_stmts(parser_t *parser) {
    switch (parser->tok->type) {
        case KUHA:
            return kuha(parser);
            break;
        case LAHAD:
            return lahad(parser);
            break;
        default:
            break;
    }
}
node_t *kuha(parser_t *parser) {
    node_t *node = createNode();
    node->type = KUHA_GRAMMAR;
    node->value.input = (kuhaNode *)calloc(1, sizeof(kuhaNode));
    if (!match(parser, KUHA, node))
        return error(parser, "[Kuha] Missing Keyword");
    if (!match(parser, LPAREN, node))
        return error(parser, "[Kuha] Missing Left Parenthesis");
    node->value.input->stringFormat = literalTerm(parser);
    if (!match(parser, COMMA, node))
        return error(parser, "[Kuha] Missing Comma");
    if (!match(parser, ADDRESS, node))
        return error(parser, "[Kuha] Missing Variabe Address");
    node->value.input->varAddress = parser_match(parser, ID);
    if (!match(parser, RPAREN, node))
        return error(parser, "[Kuha] Missing Variable");
    if (!match(parser, SEMI, node))
        return error(parser, "[Kuha] Missing Semi-colon");
    return node;
}
// optimize node
node_t *lahad(parser_t *parser) {
    node_t *node = createNode();
    node->type = LAHAD_GRAMMAR;
    node->value.printString =
        (printStringNode *)calloc(1, sizeof(printStringNode));
    if (!match(parser, LAHAD, node))
        return error(parser, "[Kuha] Missing Keyword");
    if (!match(parser, LPAREN, node))
        return error(parser, "[Lahad] Missing Left Parenthesis");
    node->value.printString->stringValue = logical_op(parser);
    if (check(parser, RPAREN)) {
        match(parser, RPAREN, node);
        if (match(parser, SEMI, node))
            return node;
        else
            return error(parser, "[Lahad] Missing Semi-colon");

    } else if (check(parser, SEMI))
        return error(parser, "[Lahad] Missing Right Parenthesis");
    // if COMMA, ask if it's print value or print expr
    else if (check(parser, COMMA)) {
        match(parser, COMMA, node);

        // ask if the nxt token is NUM, PUNTO, then it's print the
        // expr
        if (check(parser, NUM) || check(parser, PUNTO)) {
            node_t *printExpNode = createNode();
            node->type = LAHAD_GRAMMAR;
            printExpNode->value.printExpression =
                (printExp *)calloc(1, sizeof(printExp));

            // get the string format
            printExpNode->value.printExpression->stringFormat =
                node->value.printString->stringValue;  // UwU
            free(node);

            // get the expr
            printExpNode->value.printExpression->expr = logical_op(parser);
            if (!match(parser, RPAREN, node))
                return error(parser, "[Lahad] Missing Right Parenthesis");
            if (!match(parser, SEMI, node))
                return error(parser,
                             "[Lahad] Missing Semi-colon on Lahad Statement");
            return printExpNode;
        }

        node_t *printNode = createNode();
        node->type = LAHAD_GRAMMAR;
        printNode->value.printValue =
            (printValueNode *)calloc(1, sizeof(printValueNode));

        // get the string format
        printNode->value.printValue->stringFormat =
            node->value.printString->stringValue;
        free(node);

        // get the identifier
        printNode->value.printValue->identifier = parser_match(parser, ID);

        if (!match(parser, RPAREN, node))
            return error(parser, "[Lahad] Missing Right Parenthesis");
        if (!match(parser, SEMI, node))
            return error(parser,
                         "[Lahad] Missing Semi-colon on Lahad Statement");
        return printNode;
    } else
        return error(parser, "[Lahad] Missing Token Here");
}

// optimize node
node_t *declaration_stmt(parser_t *parser) {
    node_t *assgn_node = createNode();
    assgn_node->value.declaration =
        (declarationNode *)calloc(1, sizeof(declarationNode));

    node_t *dec_node = createNode();
    dec_node->value._declaration =
        (noValueDeclarationNode *)calloc(1, sizeof(noValueDeclarationNode));
    assgn_node->type = DEC_STMTS_GRAMMAR;
    dec_node->type = TEMP_DEC;

    assgn_node->value.declaration->dataType = _token(parser);
    dec_node->value._declaration->dataType =
        assgn_node->value.declaration->dataType;

    if (!check(parser, ID)) {
        free(assgn_node);
        free(dec_node);
        return error(parser, "[Declaration] Missing identifier");
    }
    assgn_node->value._declaration->identifier = _token(parser);
    dec_node->value._declaration->identifier =
        assgn_node->value.declaration->identifier;
    if (check(parser, SEMI)) {
        free(assgn_node);
        return dec_node;
    } else {
        free(dec_node);
        if (!assign_op(parser)) {
            return error(parser, "[Declaration] Missing assignment");
        }
        assgn_node->value.declaration->assignType = _operators(parser);

        assgn_node->value.declaration->expr = expr(parser);
        return assgn_node;
    }
}
node_t *assign_stmt(parser_t *parser) {
    node_t *node = createNode();
    node->value.assgn = (assgnNode *)calloc(1, sizeof(assgnNode));
    node->type = ASSIGN_STMTS_GRAMMAR;
    node->value.assgn->identifier = _token(parser);
    if (check_tokens(parser, 2, INCR, DECR)) {
        parser_recede(parser);
        node->value.unary = (unaryNode *)calloc(1, sizeof(unaryNode));
        node->type = UNARY_GRAMMAR;
        node->value.unary->token = _token(parser);
        node->value.unary->operation = _operators(parser);
        return node;
    }
    if (!assign_op(parser)) {
        free(node);
        return error(parser, "[Assignment] Missing assignment operator");
    }
    node->value.assgn->assignType = _operators(parser);
    node->value.assgn->expr = expr(parser);
    return node;
}

node_t *logical_op(parser_t *parser) {
    node_t *node = relational_op(parser);
    while (check_tokens(parser, 2, O, AT)) {
        node_t *logicalNode = createNode();
        logicalNode->type = LOGICAL_GRAMMAR;
        logicalNode->value._expression =
            (expressionNode *)calloc(1, sizeof(expressionNode));
        logicalNode->value._expression->left = node;
        logicalNode->value._expression->operation = _operators(parser);
        logicalNode->value._expression->right = relational_op(parser);
        node = logicalNode;
    }
    return node;
}
node_t *relational_op(parser_t *parser) {
    node_t *node = arithmetic_op(parser);
    while (check_tokens(parser, 6, EQ_TO, NOT_EQ, GREATER, LESS, GR_THAN_EQ,
                        LS_THAN_EQ)) {
        node_t *relationalNode = createNode();
        relationalNode->type = RELATIONAL_GRAMMAR;
        relationalNode->value._expression =
            (expressionNode *)calloc(1, sizeof(expressionNode));
        relationalNode->value._expression->left = node;
        relationalNode->value._expression->operation = _operators(parser);
        relationalNode->value._expression->right = arithmetic_op(parser);
        node = relationalNode;
    }
    return node;
}
node_t *arithmetic_op(parser_t *parser) {
    node_t *node = negateN(parser);
    while (check_tokens(parser, 7, EXP, MULT, DIV, INTDIV, MOD, ADD, SUB)) {
        node_t *arithmeticNode = createNode();
        arithmeticNode->type = ARITHMETIC_GRAMMAR;
        arithmeticNode->value._expression =
            (expressionNode *)calloc(1, sizeof(expressionNode));
        arithmeticNode->value._expression->left = node;
        arithmeticNode->value._expression->operation = _operators(parser);
        arithmeticNode->value._expression->right = negateN(parser);
        node = arithmeticNode;
    }
    return node;
}
node_t *negateN(parser_t *parser) {
    while (check(parser, NEGATE)) {
        node_t *node = createNode();
        node->type = UNARY_GRAMMAR;
        node->value.unary = (unaryNode *)calloc(1, sizeof(unaryNode));
        node->value.unary->operation = _operators(parser);
        node->value.unary->token = literalTerm(parser);
        return node;
    }
    return literalTerm(parser);
}
node_t *unary_op(parser_t *parser) {
    node_t *node = createNode();
    node->type = UNARY_GRAMMAR;
    node->value.unary = (unaryNode *)calloc(1, sizeof(unaryNode));
    if (check(parser, ID)) {
        node->value.unary->token = _token(parser);
        node->value.unary->operation = _token(parser);
    } else {
        node->value.unary->operation = _operators(parser);
        node->value.unary->token = _identifier(parser);
    }
    return node;
}

node_t *literalTerm(parser_t *parser) {
    if (check(parser, LPAREN)) {
        if (!match(parser, LPAREN, NULL))
            return error(parser, "[Expr] Missing Left Paranthesis here");
        node_t *node = expr(parser);
        if (!match(parser, RPAREN, node))
            return error(parser, "[Expr] Missing Left Paranthesis here");
        return node;

    } else if (check(parser, ID)) {
        return _identifier(parser);
    } else if (check_tokens(parser, 6, NUM, CHAR_LIT, STRING, PUNTO_LIT, TOTOO,
                            MALI)) {
        return _const(parser);
    }
    return error(parser, "[Expr] Missing variable or expression");
}

node_t *_const(parser_t *parser) {
    if (nullCursor(parser)) {
        return error(parser, "End of line, expected");
    }
    node_t *node = createNode();
    node->value.atom = (tokenNode *)calloc(1, sizeof(tokenNode));
    node->type = CONST;
    node->value.atom->nodeToken = parser->tok;
    parser_advance(parser);
    return node;
}
node_t *_identifier(parser_t *parser) {
    if (nullCursor(parser)) {
        return error(parser, "End of line, expected");
    }
    node_t *node = createNode();
    node->value.atom = (tokenNode *)calloc(1, sizeof(tokenNode));
    node->type = IDENTIFIER;
    node->value.atom->nodeToken = parser->tok;
    parser_advance(parser);
    return node;
}
node_t *_data_type(parser_t *parser) {
    if (nullCursor(parser)) {
        return error(parser, "End of line, expected");
    }
    node_t *node = createNode();
    node->value.atom = (tokenNode *)calloc(1, sizeof(tokenNode));
    node->type = DATA_TYPE;
    node->value.atom->nodeToken = parser->tok;
    parser_advance(parser);
    return node;
}
node_t *_operators(parser_t *parser) {
    if (nullCursor(parser)) {
        return error(parser, "End of line, expected");
    }
    node_t *node = createNode();
    node->value.atom = (tokenNode *)calloc(1, sizeof(tokenNode));
    node->type = OPERATORS;
    node->value.atom->nodeToken = parser->tok;
    parser_advance(parser);
    return node;
}
node_t *_token(parser_t *parser) {
    if (nullCursor(parser)) {
        return error(parser, "End of line, expected");
    }
    node_t *node = createNode();
    node->value.atom = (tokenNode *)calloc(1, sizeof(tokenNode));
    node->type = TERMINAL;
    node->value.atom->nodeToken = parser->tok;
    parser_advance(parser);
    return node;
}

node_t *error(parser_t *parser, char *errorMsg) {
    node_t *node = createNode();
    node->type = ERROR;
    node->value.error = (errorNode *)calloc(1, sizeof(errorNode));
    node->value.error->error = errorMsg;
    if (!nullCursor(parser)) {
        node->value.error->token = parser_previous_peek(parser);
        parser_advance(parser);
    } else
        node->value.error->token = parser_previous_peek(parser);
    return node;
}

int data_type(parser_t *parser) {
    switch (parser->tok->type) {
        case BOLYAN:
            return 1;
        case NUMERO:
            return 1;
        case DOBLE:
            return 1;
        case LINYA:
            return 1;
        case PUNTO:
            return 1;
        case KAR:
            return 1;
        default:
            return 0;
    }
}
int assign_op(parser_t *parser) {
    switch (parser->tok->type) {
        case EQUALS:
            return 1;
        case ADD_ASGN:
            return 1;
        case SUB_ASGN:
            return 1;
        case MULT_ASGN:
            return 1;
        case INTDIV_ASGN:
            return 1;
        case DIV_ASGN:
            return 1;
        case MOD_ASGN:
            return 1;
        default:
            return 0;
    }
}
