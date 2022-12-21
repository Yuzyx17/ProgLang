#ifndef TOKEN_H
#define TOKEN_H

typedef struct TOKEN_STRUCT
{
    enum
    {
        /* id is identifiers */
        TOKEN_ID,       // [a-zA-Z]
        TOKEN_EQUALS,   // =
        TOKEN_SEMI,     // ;
        TOKEN_LPAREN,   // (
        TOKEN_RPAREN,   // )
        TOKEN_LBRACKET, // [
        TOKEN_RBRACKET, // ]
        TOKEN_LBRACE,   // {
        TOKEN_RBRACE,   // }
        TOKEN_COMMENT,  // #

        /* ADD: operators */
        TOKEN_ADD,        // +
        TOKEN_SUB,        // -
        TOKEN_DIV,        // /
        TOKEN_INTDIV,     // ~
        TOKEN_MULT,       // *
        TOKEN_MOD,        // %
        TOKEN_EXP,        // ^
        TOKEN_LESS,       // <
        TOKEN_GREATER,    // >
        TOKEN_NEGATE,     // !
        TOKEN_COLON,      //:
        TOKEN_UNDERSCORE, // _

        // Unary
        TOKEN_INCR,    // ++
        TOKEN_DECR,    // --
        
        // Relational
        TOKEN_EQ_TO,        // ==
        TOKEN_LS_THAN_EQ,   // <=
        TOKEN_GR_THAN_EQ,   // >=
        TOKEN_NOT_EQ,       // !=

        // Operator + Assignment
        TOKEN_ADD_ASGN,     // +=
        TOKEN_SUB_ASGN,     // -=
        TOKEN_MULT_ASGN,    // *=
        TOKEN_DIV_ASGN,     // /=
        TOKEN_MOD_ASGN,     // %=
        TOKEN_INTDIV_ASGN,  // ~=

        /* ADD: keywords */
        TOKEN_WHLE,     // habang
        TOKEN_IF,       // kung
        TOKEN_ELSE,     // sakali
        TOKEN_ELIF,     // kundi
        TOKEN_FOR,      // para
        TOKEN_FUNCTION, // depinisyon/dep
        TOKEN_IMPORT,   // angkat
        TOKEN_TRY,      // subukan
        TOKEN_EXCEPT,   // maliban
        TOKEN_FINALLY,  // wakas
        TOKEN_BREAK,    // tigil
        TOKEN_CONTINUE, // tuloy
        TOKEN_RETURN,   // balik
        TOKEN_AND,      // at
        TOKEN_OR,       // o
        TOKEN_IN,       // nasa
        TOKEN_GLOBAL,   // lahat

        /* ADD: data types*/
        TOKEN_BOOL,   // bolyan
        TOKEN_BOOLT,  // totoo
        TOKEN_BOOLM,  // mali
        TOKEN_INT,    // numero / num
        TOKEN_NUM,    // num literals [0-9]*
        TOKEN_STR,    // linya
        TOKEN_STRING, // "[a-!]"
        TOKEN_NULL,   // wala
        TOKEN_FLOAT,  // punto / puntonumero
        TOKEN_DBL,    // doble / doblenumero
        TOKEN_CHAR,   // karakter / kar

        /* unknown token */
        TOKEN_UNKNOWN, // invalid
        //for comments
        TOKEN_COMMENT_VALUE_SINGLE,
        TOKEN_COMMENT_VALUE_MULTI,
        // for constant
        TOKEN_CAPITAL,
        TOKEN_COMMA,
        TOKEN_CHAR_LIT,
        TOKEN_FLOAT_LIT,
        TOKEN_PRINT,
        TOKEN_SCAN,
    } type;

    char *value;

} token_t;

token_t *init_token(int type, char *value);

#endif