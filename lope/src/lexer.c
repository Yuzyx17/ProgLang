#include "include/lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/token.h"

lexer_t *init_lexer(char *contents) {
    // allocating memory for lexer struct into lexer memory
    lexer_t *lexer = calloc(1, sizeof(struct LEXER_STRUCT));
    // -> to access contents in structs; in this case, lexer can access
    // contents, i , c in struct inputting of file contents into lexer struct
    lexer->contents = contents;  // contents is file contents
    // current index
    // assignments of index into lexer struct i
    lexer->i = 0;
    // current line
    lexer->l = 1;
    // current char in-line
    lexer->lc = 1;
    // current character
    lexer->c = contents[lexer->i];
    return lexer;
}

// lexer-advance -> iurong natin yung current index ng character sa loob ng
// contents
void lexer_advance(lexer_t *lexer) {
    // if the current character is equal to null space and less than the len of
    // contents, add 1 to index and character should be equal to lexer's content
    // indexed by the current lexer
    if ((lexer->c != '\0' || lexer->c != ' ' || lexer->c != '\t') &&
        lexer->i < strlen(lexer->contents)) {
        lexer->lc += 1;
        lexer->i += 1;
        lexer->c = lexer->contents[lexer->i];
    }
};

// for skipping whitespace
void lexer_skip_whitespace(lexer_t *lexer) {
    // 10 is code for new line; meaning while lexer is a space or a new line:
    while (lexer->c == ' ' || lexer->c == '\n' || lexer->c == '\t' ||
           lexer->c == '\r') {
        if (lexer->c == '\n') {
            lexer->lc = 0;
            lexer->l += 1;
        }
        // advance the lexer
        lexer_advance(lexer);
    }
};

// Special Characters
// check if current character is a special character and if the next struct
// index is also a special character if so, return the token, and return the
// special character as a string
token_t *lexer_get_next_token(lexer_t *lexer) {
    // while lexer is not a null and i is less than the lexer contents; meaning
    // there are tokens that are needed to be parsed.
    while (lexer->c != '\0' && lexer->i < strlen(lexer->contents)) {
        if (lexer->c == ' ' || lexer->c == '\t' || lexer->c == '\n' ||
            lexer->c == '\r' || lexer->c == '\f') {
            if (lexer->c == '\n')
                lexer->lc = 0;
            lexer_skip_whitespace(lexer);
        }
        int startPos = lexer->lc;
        // -> for calling value or contents of struct
        switch (lexer->c) {
            // if the case is =, call lexer advance while carrying the token,
            // add its parameter the current lexer instance, and the result
            // of init_token if we inputted EQUALS, and the current lexer
            // character as a string.
            case '#':
                if (lexer->contents[lexer->i + 1] == '*') {
                    return lexer_collect_comment_multi(lexer);
                    /* return lexer_advance_with_token(lexer,
                     * init_token(COMMENT_VALUE_MULTI,
                     * lexer_get_operator_ext_as_string(lexer))); */
                } else {
                    return lexer_collect_comment_single(lexer);
                    /* return lexer_advance_with_token(lexer,
                     * init_token(COMMENT_VALUE_SINGLE,
                     * lexer_get_current_char_as_string(lexer))); */
                }
            case '=':  // 2times
                if (lexer->contents[lexer->i + 1] == '=') {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(EQ_TO,
                                   lexer_get_operator_ext_as_string(lexer),
                                   lexer->l, startPos));
                } else {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(EQUALS,
                                   lexer_get_current_char_as_string(lexer),
                                   lexer->l, startPos));
                }
                break;
            case ';':
                return lexer_advance_with_token(
                    lexer,
                    init_token(SEMI, lexer_get_current_char_as_string(lexer),
                               lexer->l, startPos));
                break;
            case '(':
                return lexer_advance_with_token(
                    lexer,
                    init_token(LPAREN, lexer_get_current_char_as_string(lexer),
                               lexer->l, startPos));
                break;
            case ')':
                return lexer_advance_with_token(
                    lexer,
                    init_token(RPAREN, lexer_get_current_char_as_string(lexer),
                               lexer->l, startPos));
                break;
            case '}':
                return lexer_advance_with_token(
                    lexer,
                    init_token(RBRACE, lexer_get_current_char_as_string(lexer),
                               lexer->l, startPos));
                break;
            case '{':
                return lexer_advance_with_token(
                    lexer,
                    init_token(LBRACE, lexer_get_current_char_as_string(lexer),
                               lexer->l, startPos));
                break;
            case ']':
                return lexer_advance_with_token(
                    lexer, init_token(RBRACKET,
                                      lexer_get_current_char_as_string(lexer),
                                      lexer->l, startPos));
                break;
            case '[':
                return lexer_advance_with_token(
                    lexer, init_token(LBRACKET,
                                      lexer_get_current_char_as_string(lexer),
                                      lexer->l, startPos));
                break;
            case '<':
                if (lexer->contents[lexer->i + 1] == '=') {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(LS_THAN_EQ,
                                   lexer_get_operator_ext_as_string(lexer),
                                   lexer->l, startPos));
                } else {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(LESS,
                                   lexer_get_current_char_as_string(lexer),
                                   lexer->l, startPos));
                }
                break;
            case '>':
                if (lexer->contents[lexer->i + 1] == '=') {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(GR_THAN_EQ,
                                   lexer_get_operator_ext_as_string(lexer),
                                   lexer->l, startPos));
                } else {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(GREATER,
                                   lexer_get_current_char_as_string(lexer),
                                   lexer->l, startPos));
                }
                break;
            case '+':
                if (lexer->contents[lexer->i + 1] == '=') {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(ADD_ASGN,
                                   lexer_get_operator_ext_as_string(lexer),
                                   lexer->l, startPos));
                } else if (lexer->contents[lexer->i + 1] == '+') {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(INCR,
                                   lexer_get_operator_ext_as_string(lexer),
                                   lexer->l, startPos));
                } else {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(ADD, lexer_get_current_char_as_string(lexer),
                                   lexer->l, startPos));
                }
                break;
            case '-':
                if (lexer->contents[lexer->i + 1] == '=') {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(SUB_ASGN,
                                   lexer_get_operator_ext_as_string(lexer),
                                   lexer->l, startPos));
                } else if (lexer->contents[lexer->i + 1] == '-') {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(DECR,
                                   lexer_get_operator_ext_as_string(lexer),
                                   lexer->l, startPos));
                } else {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(SUB, lexer_get_current_char_as_string(lexer),
                                   lexer->l, startPos));
                }
                break;
            case '/':
                if (lexer->contents[lexer->i + 1] == '=') {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(DIV_ASGN,
                                   lexer_get_operator_ext_as_string(lexer),
                                   lexer->l, startPos));
                } else {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(DIV, lexer_get_current_char_as_string(lexer),
                                   lexer->l, startPos));
                }
                break;
            case '~':
                if (lexer->contents[lexer->i + 1] == '=') {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(INTDIV_ASGN,
                                   lexer_get_operator_ext_as_string(lexer),
                                   lexer->l, startPos));
                } else {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(INTDIV,
                                   lexer_get_current_char_as_string(lexer),
                                   lexer->l, startPos));
                }
                break;
            case '*':
                if (lexer->contents[lexer->i + 1] == '=') {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(MULT_ASGN,
                                   lexer_get_operator_ext_as_string(lexer),
                                   lexer->l, startPos));
                } else if (lexer->contents[lexer->i + 1] == '#') {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(COMMENT_VALUE_MULTI,
                                   lexer_get_operator_ext_as_string(lexer),
                                   lexer->l, startPos));
                } else {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(MULT,
                                   lexer_get_current_char_as_string(lexer),
                                   lexer->l, startPos));
                }
                break;
            case '%':
                if (lexer->contents[lexer->i + 1] == '=') {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(MOD_ASGN,
                                   lexer_get_operator_ext_as_string(lexer),
                                   lexer->l, startPos));
                } else {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(LESS,
                                   lexer_get_current_char_as_string(lexer),
                                   lexer->l, startPos));
                }
                break;
            case '^':
                return lexer_advance_with_token(
                    lexer,
                    init_token(EXP, lexer_get_current_char_as_string(lexer),
                               lexer->l, startPos));
                break;
            case '|':
                if (lexer->contents[lexer->i + 1] == '|') {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(O, lexer_get_operator_ext_as_string(lexer),
                                   lexer->l, startPos));
                } else {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(UNKNOWN,
                                   lexer_get_current_char_as_string(lexer),
                                   lexer->l, startPos));
                }
                break;
            case '&':
                if (lexer->contents[lexer->i + 1] == '&') {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(AT, lexer_get_operator_ext_as_string(lexer),
                                   lexer->l, startPos));
                } else {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(ADDRESS,
                                   lexer_get_current_char_as_string(lexer),
                                   lexer->l, startPos));
                }
                break;
            case '!':
                if (lexer->contents[lexer->i + 1] == '=') {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(NOT_EQ,
                                   lexer_get_operator_ext_as_string(lexer),
                                   lexer->l, startPos));
                } else {
                    return lexer_advance_with_token(
                        lexer,
                        init_token(NEGATE,
                                   lexer_get_current_char_as_string(lexer),
                                   lexer->l, startPos));
                }
                break;
            case ':':
                return lexer_advance_with_token(
                    lexer,
                    init_token(COLON, lexer_get_current_char_as_string(lexer),
                               lexer->l, startPos));
                break;
            case '_':
                return lexer_advance_with_token(
                    lexer, init_token(UNDERSCORE,
                                      lexer_get_current_char_as_string(lexer),
                                      lexer->l, startPos));
                break;
            case ',':
                return lexer_advance_with_token(
                    lexer,
                    init_token(COMMA, lexer_get_current_char_as_string(lexer),
                               lexer->l, startPos));
            default:
                // for KEYWORDS
                // linya
                // c = i;
                if (isalpha(lexer->c)) {
                    // is to append all characters that are not special
                    // characters and check if it is a keyword, noise, or
                    // reserved word
                    return lexer_collect_keyword(lexer);
                };

                // for numbers
                if (isdigit(lexer->c)) {
                    return lexer_collect_number(lexer);
                };

                // FOR THE STRINGS
                if (lexer->c == '"') {
                    return lexer_collect_string(lexer);
                }

                if (lexer->c == '\'') {
                    return lexer_collect_char_lit(lexer);
                }
                if (lexer->c == '\0') {
                    break;
                }
                // Unknown
                return lexer_advance_with_token(
                    lexer,
                    init_token(UNKNOWN, lexer_get_current_char_as_string(lexer),
                               lexer->l, startPos));
                break;
        }
    }
    return (void *)0;
};

// for string literals
// if the current character is " call this function and advance the lexer
token_t *lexer_collect_string(lexer_t *lexer) {
    int startPos = lexer->lc;
    // if the lexer collected a LINYA_LIT, advance it to skip the "
    lexer_advance(lexer);
    // value is the allocation of the memory for a string
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    // if the current character is not a ". then, everything between " ", make
    // into a string.
    while (lexer->c != '"') {
        char *s = lexer_get_current_char_as_string(lexer);
        // reallocate the string length of the value by adding the length of s
        // to update it and fit the string.
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);  // append the current character to value string.
        lexer_advance(lexer);
    }
    lexer_advance(lexer);
    // return the value by calling the init_token function wherein it will be a
    // STRING as type and added into the struct.

    return init_token(LINYA_LIT, value, lexer->l, startPos - 1);
};

char *lexer_get_operator_ext_as_string(lexer_t *lexer) {
    char *value = calloc(3, sizeof(char));
    value[0] = lexer->c;
    lexer_advance(lexer);
    value[1] = lexer->c;
    value[2] = '\0';

    return value;
}

token_t *lexer_collect_id(lexer_t *lexer) {
    // value is the allocation of the memory for a string
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    // while the character is an alphanumeric
    int startPos = lexer->lc;
    while (isalpha(lexer->c)) {
        char *s = lexer_get_current_char_as_string(lexer);
        // reallocate the string length of the value by adding the length of s
        // to update it and fit the string.
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);  // append the current character to value string.
        lexer_advance(lexer);
    }

    return init_token(ID, value, lexer->l, startPos);
}

token_t *lexer_advance_with_token(lexer_t *lexer, token_t *token) {
    lexer_advance(lexer);
    return token;
}

token_t *lexer_collect_keyword(lexer_t *lexer) {
    // value is the allocation of the memory for a string
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    int startPos = lexer->lc;
    // while the character is an alphanumeric
    while ((lexer->c != ' ' && lexer->c != '\n' && lexer->c != '\t' &&
            lexer->c != '\0') &&
               isalnum(lexer->c) ||
           lexer->c == '_')  // isalnum(lexer->c) || lexer->c == '_'
    {
        char *s = lexer_get_current_char_as_string(lexer);
        // reallocate the string length of the value by adding the length of s
        // to update it and fit the string.
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);  // append the current character to value string.
        lexer_advance(lexer);
    }
    // advance the token.
    // return the value by calling the init_token function wherein it will be a
    // STRING as type and added into the struct. WHILE - HABANG
    if (value[0] == 'h') {
        if (value[1] == 'a') {
            if (value[2] == 'b') {
                if (value[3] == 'a') {
                    if (value[4] == 'n') {
                        if (value[5] == 'g') {
                            if (value[6] == '\0')
                                return init_token(HABANG, value, lexer->l,
                                                  startPos);
                            else
                                goto SCAN_INDENTIFIER;
                        } else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else
            goto SCAN_INDENTIFIER;
    }
    // SAKALI - SAKALI | //SUBUKAN - SUBUKAN
    else if (value[0] == 's') {
        if (value[1] == 'a') {
            if (value[2] == 'k') {
                if (value[3] == 'a') {
                    if (value[4] == 'l') {
                        if (value[5] == 'i') {
                            if (value[6] == '\0')
                                return init_token(SAKALI, value, lexer->l,
                                                  startPos);
                            else
                                goto SCAN_INDENTIFIER;
                        } else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else if (value[1] == 'u') {
            if (value[2] == 'b') {
                if (value[3] == 'u') {
                    if (value[4] == 'k') {
                        if (value[5] == 'a') {
                            if (value[6] == 'n') {
                                if (value[7] == '\0')
                                    return init_token(SUBUKAN, value, lexer->l,
                                                      startPos);
                                else
                                    goto SCAN_INDENTIFIER;
                            } else
                                goto SCAN_INDENTIFIER;
                        } else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else
            goto SCAN_INDENTIFIER;
    }
    // PARA - PARA | //PUNTO - PUNTONUMERO/PUNTO
    else if (value[0] == 'p') {
        if (value[1] == 'a') {
            if (value[2] == 'r') {
                if (value[3] == 'a') {
                    if (value[4] == '\0')
                        return init_token(PARA, value, lexer->l, startPos);
                    else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else if (value[1] == 'u') {
            if (value[2] == 'n') {
                if (value[3] == 't') {
                    if (value[4] == 'o') {
                        if (value[5] == 'n') {
                            if (value[6] == 'u') {
                                if (value[7] == 'm') {
                                    if (value[8] == 'e') {
                                        if (value[9] == 'r') {
                                            if (value[10] == 'o') {
                                                if (value[11] == '\0')
                                                    return init_token(
                                                        PUNTO, value, lexer->l,
                                                        startPos);
                                                else
                                                    goto SCAN_INDENTIFIER;
                                            } else
                                                goto SCAN_INDENTIFIER;
                                        } else
                                            goto SCAN_INDENTIFIER;
                                    } else
                                        goto SCAN_INDENTIFIER;
                                } else
                                    goto SCAN_INDENTIFIER;
                            } else
                                goto SCAN_INDENTIFIER;
                        } else if (value[5] == '\0') {
                            return init_token(PUNTO, value, lexer->l, startPos);
                        } else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else
            goto SCAN_INDENTIFIER;
    }
    // LAHAT - LAHAT |  //STRING - LINYA
    else if (value[0] == 'l') {
        if (value[1] == 'a') {
            if (value[2] == 'h') {
                if (value[3] == 'a') {
                    if (value[4] == 'd') {
                        if (value[5] == '\0')
                            return init_token(LAHAD, value, lexer->l, startPos);
                    } else if (value[4] == 't') {
                        if (value[5] == '\0')
                            return init_token(LAHAT, value, lexer->l, startPos);
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else if (value[1] == 'i') {
            if (value[2] == 'n') {
                if (value[3] == 'y') {
                    if (value[4] == 'a') {
                        if (value[5] == '\0')
                            return init_token(LINYA, value, lexer->l, startPos);
                        else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else
            goto SCAN_INDENTIFIER;
    }
    // TOTOO - TOTOO
    else if (value[0] == 't') {
        if (value[1] == 'o') {
            if (value[2] == 't') {
                if (value[3] == 'o') {
                    if (value[4] == 'o') {
                        if (value[5] == '\0')
                            return init_token(TOTOO, value, lexer->l, startPos);
                        else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else if (value[1] == 'i') {
            if (value[2] == 'g') {
                if (value[3] == 'i') {
                    if (value[4] == 'l') {
                        if (value[5] == '\0')
                            return init_token(TIGIL, value, lexer->l, startPos);
                        else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else if (value[1] == 'u') {
            if (value[2] == 'l') {
                if (value[3] == 'o') {
                    if (value[4] == 'y') {
                        if (value[5] == '\0')
                            return init_token(TULOY, value, lexer->l, startPos);
                        else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else
            goto SCAN_INDENTIFIER;
    }
    // NUMERO - NUMERO / NUMERO_LIT, | //NASA - NASA
    else if (value[0] == 'n') {
        if (value[1] == 'u') {
            if (value[2] == 'm') {
                if (value[3] == 'e') {
                    if (value[4] == 'r') {
                        if (value[5] == 'o') {
                            if (value[6] == '\0')
                                return init_token(NUMERO, value, lexer->l,
                                                  startPos);
                            else
                                goto SCAN_INDENTIFIER;
                        } else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else if (value[3] == '\0') {
                    return init_token(NUMERO, value, lexer->l, startPos);
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else if (value[1] == 'a') {
            if (value[2] == 's') {
                if (value[3] == 'a') {
                    if (value[4] == '\0') {
                        return init_token(NASA, value, lexer->l, startPos);
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else
            goto SCAN_INDENTIFIER;
    }
    // KAR - KARAKTER/KAR | //KUNDI - KUNDI | //KUNG - KUNG |
    // //KUHA - KUHA
    else if (value[0] == 'k') {
        if (value[1] == 'a') {
            if (value[2] == 'r') {
                if (value[3] == 'a') {
                    if (value[4] == 'k') {
                        if (value[5] == 't') {
                            if (value[6] == 'e') {
                                if (value[7] == 'r') {
                                    if (value[8] == '\0') {
                                        return init_token(KAR, value, lexer->l,
                                                          startPos);
                                    } else
                                        goto SCAN_INDENTIFIER;
                                } else
                                    goto SCAN_INDENTIFIER;
                            } else
                                goto SCAN_INDENTIFIER;
                        } else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else if (value[3] == '\0') {
                    return init_token(KAR, value, lexer->l, startPos);
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else if (value[1] == 'u') {
            if (value[2] == 'n') {
                if (value[3] == 'd') {
                    if (value[4] == 'i') {
                        if (value[5] == '\0')
                            return init_token(KUNDI, value, lexer->l, startPos);
                        else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else if (value[3] == 'g') {
                    if (value[4] == '\0')
                        return init_token(KUNG, value, lexer->l, startPos);
                    else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else if (value[2] == 'h') {
                if (value[3] == 'a') {
                    if (value[4] == '\0')
                        return init_token(KUHA, value, lexer->l, startPos);
                    else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else
            goto SCAN_INDENTIFIER;
    }
    // DEP - DEPINISYON/DEP - //DOBLE - DOBLENUMERO/DOBLE
    else if (value[0] == 'd') {
        if (value[1] == 'e') {
            if (value[2] == 'p') {
                if (value[3] == 'i') {
                    if (value[4] == 'n') {
                        if (value[5] == 'i') {
                            if (value[6] == 's') {
                                if (value[7] == 'y') {
                                    if (value[8] == 'o') {
                                        if (value[9] == 'n') {
                                            if (value[10] == '\0')
                                                return init_token(DEP, value,
                                                                  lexer->l,
                                                                  startPos);
                                            else
                                                goto SCAN_INDENTIFIER;
                                        } else
                                            goto SCAN_INDENTIFIER;
                                    } else
                                        goto SCAN_INDENTIFIER;
                                } else
                                    goto SCAN_INDENTIFIER;
                            } else
                                goto SCAN_INDENTIFIER;
                        } else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else if (value[3] == '\0') {
                    return init_token(DEP, value, lexer->l, startPos);
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else if (value[1] == 'o') {
            if (value[2] == 'b') {
                if (value[3] == 'l') {
                    if (value[4] == 'e') {
                        if (value[5] == 'n') {
                            if (value[6] == 'u') {
                                if (value[7] == 'm') {
                                    if (value[8] == 'e') {
                                        if (value[9] == 'r') {
                                            if (value[10] == 'o') {
                                                if (value[11] == '\0') {
                                                    return init_token(
                                                        DOBLE, value, lexer->l,
                                                        startPos);
                                                } else
                                                    goto SCAN_INDENTIFIER;
                                            } else
                                                goto SCAN_INDENTIFIER;
                                        } else
                                            goto SCAN_INDENTIFIER;
                                    } else
                                        goto SCAN_INDENTIFIER;
                                } else
                                    goto SCAN_INDENTIFIER;
                            } else
                                goto SCAN_INDENTIFIER;
                        } else if (value[5] == '\0')
                            return init_token(DOBLE, value, lexer->l, startPos);
                        else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else
            goto SCAN_INDENTIFIER;
    }
    // ANGKAT - ANGKAT | //AT - AT
    else if (value[0] == 'a') {
        if (value[1] == 'n') {
            if (value[2] == 'g') {
                if (value[3] == 'k') {
                    if (value[4] == 'a') {
                        if (value[5] == 't') {
                            if (value[6] == '\0')
                                return init_token(ANGKAT, value, lexer->l,
                                                  startPos);
                            else
                                goto SCAN_INDENTIFIER;
                        } else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else if (value[1] == 't') {
            if (value[2] == '\0')
                return init_token(AT, value, lexer->l, startPos);
            else
                goto SCAN_INDENTIFIER;
        } else
            goto SCAN_INDENTIFIER;
    }
    // MALIBAN - MALIBAN | //MALI - MALI | //MULA - MULA
    else if (value[0] == 'm') {
        if (value[1] == 'a') {
            if (value[2] == 'l') {
                if (value[3] == 'i') {
                    if (value[4] == 'b') {
                        if (value[5] == 'a') {
                            if (value[6] == 'n') {
                                if (value[7] == '\0') {
                                    return init_token(MALIBAN, value, lexer->l,
                                                      startPos);
                                } else
                                    goto SCAN_INDENTIFIER;
                            } else
                                goto SCAN_INDENTIFIER;
                        } else
                            goto SCAN_INDENTIFIER;
                    } else if (value[4] == '\0') {
                        return init_token(MALI, value, lexer->l, startPos);
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else if (value[1] == 'u') {
            if (value[2] == 'l') {
                if (value[3] == 'a') {
                    if (value[4] == '\0') {
                        return init_token(FROM, value, lexer->l, startPos);
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else
            goto SCAN_INDENTIFIER;
    }
    // WAKAS - WAKAS | //WALA - WALA
    else if (value[0] == 'w') {
        if (value[1] == 'a') {
            if (value[2] == 'k') {
                if (value[3] == 'a') {
                    if (value[4] == 's') {
                        if (value[5] == '\0') {
                            return init_token(WAKAS, value, lexer->l, startPos);
                        } else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else if (value[2] == 'l') {
                if (value[3] == 'a') {
                    if (value[4] == '\0')
                        return init_token(WALA, value, lexer->l, startPos);
                    else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else
            goto SCAN_INDENTIFIER;
    }
    // LAHAD - LAHAD
    else if (value[0] == 'l') {
        if (value[1] == 'a') {
            if (value[2] == 'h') {
                if (value[3] == 'a') {
                    if (value[4] == 'd') {
                        if (value[5] == '\0')
                            return init_token(LAHAD, value, lexer->l, startPos);
                        else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else
            goto SCAN_INDENTIFIER;
    }
    // TIGIL - TIGIL | //TULOY - TULOY
    else if (value[0] == 't') {
        if (value[1] == 'i') {
            if (value[2] == 'g') {
                if (value[3] == 'i') {
                    if (value[4] == 'l') {
                        if (value[5] == '\0') {
                            return init_token(TIGIL, value, lexer->l, startPos);
                        } else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else if (value[1] == 'u') {
            if (value[2] == 'l') {
                if (value[3] == 'o') {
                    if (value[4] == 'y') {
                        if (value[5] == '\0') {
                            return init_token(TULOY, value, lexer->l, startPos);
                        } else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else
            goto SCAN_INDENTIFIER;
    }
    // BALIK - BALIK | //BOLYAN - BOLYAN
    else if (value[0] == 'b') {
        if (value[1] == 'a') {
            if (value[2] == 'l') {
                if (value[3] == 'i') {
                    if (value[4] == 'k') {
                        if (value[5] == '\0')
                            return init_token(BALIK, value, lexer->l, startPos);
                        else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else if (value[1] == 'o') {
            if (value[2] == 'l') {
                if (value[3] == 'y') {
                    if (value[4] == 'a') {
                        if (value[5] == 'n') {
                            if (value[6] == '\0')
                                return init_token(BOLYAN, value, lexer->l,
                                                  startPos);
                            else
                                goto SCAN_INDENTIFIER;
                        } else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else
            goto SCAN_INDENTIFIER;
    }
    // O - O
    else if (value[0] == 'o') {
        if (value[1] == '\0')
            return init_token(O, value, lexer->l, startPos);
        else
            goto SCAN_INDENTIFIER;
    }
    // IBURA - IBURA
    else if (value[0] == 'i') {
        if (value[1] == 'b') {
            if (value[2] == 'u') {
                if (value[3] == 'r') {
                    if (value[4] == 'a') {
                        if (value[5] == '\0')
                            return init_token(IBURA, value, lexer->l, startPos);
                        else
                            goto SCAN_INDENTIFIER;
                    } else
                        goto SCAN_INDENTIFIER;
                } else
                    goto SCAN_INDENTIFIER;
            } else
                goto SCAN_INDENTIFIER;
        } else
            goto SCAN_INDENTIFIER;
    } else {
    SCAN_INDENTIFIER:;
        // john  -> return as id
        // j&&hn -> return as unknown or invalid
        // JOHN = 6 -> return as CONSTNT
        // count the number of capital in the string
        int counter_caps = 0;
        int flag_illegal_symbol = 0;
        int counter_underscore = 0;

        for (int i = 0; i < strlen(value); i++) {
            if (value[i] == '_')
                counter_underscore++;
            if (isupper(value[i]))
                counter_caps++;
            else if ((isalnum(value[i]) == 0 && value[i] != '_') ||
                     isdigit(value[i]) == 1)
                flag_illegal_symbol = 1;
        }
        if ((counter_caps + counter_underscore) == strlen(value)) {
            return init_token(CAPITAL, value, lexer->l, startPos);
        }
        if (flag_illegal_symbol == 0)
            return init_token(ID, value, lexer->l, startPos);
        else
            return init_token(UNKNOWN, value, lexer->l, startPos);
    }
}

char *lexer_get_current_char_as_string(lexer_t *lexer) {
    char *str = calloc(2, sizeof(char));
    str[0] = lexer->c;
    str[1] = '\0';

    return str;
};

// temporary way to compare
int compare_to_keyword(char *identifier, char *keyword) {
    int index;
    // turns the keyword into a finite state machine, checks character by
    // character
    for (index = 0; index < strlen(keyword); index++) {
        if (identifier[index] != keyword[index]) {
            return 0;
        }
    }
    return 1;
}

// converts digit into string
token_t *lexer_collect_number(lexer_t *lexer) {
    // value is the allocation of the memory for a string
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    // while the character is an alphanumeric
    int decimal_count = 0;
    int flag_invalid = 0;
    int startPos = lexer->lc;
    while ((isdigit(lexer->c) == 1 || lexer->c == '.')) {
        if (lexer->c == '.') {
            decimal_count++;
        }
        char *s = lexer_get_current_char_as_string(lexer);
        // reallocate the string length of the value by adding the length of s
        // to update it and fit the string.
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);  // append the current character to value string.
        lexer_advance(lexer);
    }
    // advance the token.
    // return the value by calling the init_token function wherein it will be a
    // STRING as type and added into the struct.
    if (decimal_count == 0) {
        return init_token(NUMERO_LIT, value, lexer->l, startPos);
    } else if (decimal_count <= 1) {
        return init_token(PUNTO_LIT, value, lexer->l, startPos);
    } else {
        return init_token(UNKNOWN, value, lexer->l, startPos);
    }
}

token_t *lexer_collect_comment_single(lexer_t *lexer) {
    int startPos = lexer->lc;
    lexer_advance(lexer);
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    while (lexer->c != '\n' && lexer->c != '\0') {
        char *s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);  // append the current character to value string.
        lexer_advance(lexer);
    }
    return init_token(COMMENT_VALUE_SINGLE, value, lexer->l, startPos - 1);
};

token_t *lexer_collect_comment_multi(lexer_t *lexer) {
    int startPos = lexer->lc;
    int startLinePos = lexer->l;
    lexer_advance(lexer);
    lexer_advance(lexer);
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';
    while (!(lexer->c == '*' && lexer->contents[lexer->i + 1] == '#') &&
           lexer->c != '\0') {
        char *s = lexer_get_current_char_as_string(lexer);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);
        if (lexer->c == '\n') {
            lexer->l += 1;
        }
        lexer_advance(lexer);
    }
    if (lexer->c == '*' && lexer->contents[lexer->i + 1] == '#') {
        lexer_advance(lexer);  // for *
        lexer_advance(lexer);  // for #
    } else {
        return init_token(UNKNOWN, value, startLinePos, startPos - 1);
    }

    return init_token(COMMENT_VALUE_MULTI, value, startLinePos, startPos - 1);
};

token_t *lexer_collect_char_lit(lexer_t *lexer) {
    int startPos = lexer->lc;
    lexer_advance(lexer);
    char *value = calloc(2, sizeof(char));
    value[0] = lexer->c;
    value[1] = '\0';
    if (lexer->contents[lexer->i + 1] == 39) {
        lexer_advance(lexer);
        lexer_advance(lexer);
        return init_token(CHAR_LIT, value, lexer->l, startPos);
    } else {
        lexer_advance(lexer);
        lexer_advance(lexer);
        return init_token(UNKNOWN, value, lexer->l, startPos);
    }
};