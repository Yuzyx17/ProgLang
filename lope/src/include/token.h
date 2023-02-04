#ifndef H
#define H

typedef enum {
    /* id is identifiers */
    ID,        // [a-zA-Z]*
    EQUALS,    // =
    SEMI,      // ;
    LPAREN,    // (
    RPAREN,    // )
    LBRACKET,  // [
    RBRACKET,  // ]
    LBRACE,    // {
    RBRACE,    // }
    COMMENT,   // #

    /* ADD: operators */
    ADD,         // +
    SUB,         // -
    DIV,         // /
    INTDIV,      // ~
    MULT,        // **
    MOD,         // %
    EXP,         // ^
    LESS,        // <
    GREATER,     // >
    NEGATE,      // !
    COLON,       // :
    UNDERSCORE,  // _

    // Unary
    INCR,  // ++
    DECR,  // --

    // Relational
    EQ_TO,       // ==
    LS_THAN_EQ,  // <=
    GR_THAN_EQ,  // >=
    NOT_EQ,      // !=

    // Operator + Assignment
    ADD_ASGN,     // +=
    SUB_ASGN,     // -=
    MULT_ASGN,    // **=
    DIV_ASGN,     // /=
    MOD_ASGN,     // %=
    INTDIV_ASGN,  // ~=

    /* ADD: keywords */
    HABANG,   // habang
    KUNG,     // kung
    SAKALI,   // sakali
    KUNDI,    // kundi
    PARA,     // para
    DEP,      // depinisyon/dep
    ANGKAT,   // angkat
    SUBUKAN,  // subukan
    MALIBAN,  // maliban
    WAKAS,    // wakas
    TIGIL,    // tigil
    TULOY,    // tuloy
    BALIK,    // balik
    AT,       // at
    O,        // o
    NASA,     // nasa
    LAHAT,    // lahat

    /* ADD: data types*/
    BOLYAN,  // bolyan
    TOTOO,   // totoo
    MALI,    // mali
    NUMERO,  // numero / num
    NUM,     // num literals [0-9]*
    LINYA,   // linya
    STRING,  // "[a-!]"
    WALA,    // wala
    PUNTO,   // punto / puntonumero
    DOBLE,   // doble / doblenumero
    KAR,     // karakter / kar

    /* unknown token */
    UNKNOWN,  // invalid
    // for comments
    COMMENT_VALUE_SINGLE,  // #
    COMMENT_VALUE_MULTI,   // #*
    // for constant
    CAPITAL,    // [A-Z]*
    COMMA,      // ,
    CHAR_LIT,   // "[a-z]"
    PUNTO_LIT,  // num.num | num
    LAHAD,      // lahad
    KUHA,       // kuha
    IBURA,      // ibura
    FROM,       // angkat
    ADDRESS
} type;
typedef struct STRUCT {
    type type;
    char *value;
    unsigned int lpos;
    unsigned int cpos;

} token_t;

token_t *init_token(int type, char *value, int l, int lc);

#endif