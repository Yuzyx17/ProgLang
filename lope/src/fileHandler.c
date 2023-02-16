#include "include/fileHandler.h"

#include "include/parser.h"
#include "include/token.h"

char *token_type[100] = {"Identifier",
                         "Equal Sign",
                         "Semi-Colon",
                         "Left Parenthesis",
                         "Right Parenthesis",
                         "Left Bracket",
                         "Right Bracket",
                         "Left Brace",
                         "Right Brace",
                         "Comment",
                         "+",
                         "-",
                         "/",
                         "~",
                         "*",
                         "%",
                         "^",
                         "<",
                         ">",
                         "!",
                         "Colon",
                         "Underscore",
                         "++",
                         "--",
                         "==",
                         "<=",
                         ">=",
                         "!=",
                         "+=",
                         "-=",
                         "*=",
                         "/=",
                         "%=",
                         "~=",
                         "Habang",
                         "Kung",
                         "Sakali",
                         "Kundi",
                         "Para",
                         "Function",
                         "Import",
                         "Try",
                         "Except",
                         "Finally",
                         "Break",
                         "Continue",
                         "Return",
                         "At",
                         "O",
                         "In",
                         "Global",
                         "Boolean",
                         "Totoo",
                         "Mali",
                         "Numero",
                         "Numero_lit",
                         "Linya",
                         "Linya_lit",
                         "None",
                         "Punto",
                         "Doble",
                         "Karakter",
                         "Invalid",
                         "Single Comment",
                         "Multiple Comment",
                         "Constant",
                         "Comma",
                         "Karakter_lit",
                         "Puntonumero_lit",
                         "Scan",
                         "Print",
                         "Delete",
                         "From",
                         "Address"};

int *find_file() {
    // file -> fseek -> from start to finish -> amount of character length to
    // memory ->
    fseek(input_ptr, 0L, SEEK_END);
    fileLen = ftell(input_ptr);
    fseek(input_ptr, 0L, SEEK_SET);

    fileContent = calloc(fileLen, sizeof(char));
    fread(fileContent, sizeof(char), fileLen, input_ptr);
}
int check_Extension(char *file) {
    char *extension;
    extension = strrchr(file, '.');
    if (compare_to_keyword(extension, ".lp") == 1) {
        return 1;
    }
    return 0;
}
void create_TokenOutput(char *file) {
    memset(fileName, 0, sizeof(fileName));
    strncpy(fileName, file, strrchr(file, '.') - file);
    strcat(fileName, "_SymbolTable");
    strcat(fileName, ".txt");
    output_ptr = fopen(fileName, "w");
}

int scanFile(lexer_t *lexer) {
    token_t **tokens = malloc(sizeof(token_t *));
    const char *token_code[] = {"identifier",
                                "equals",
                                "semi",
                                "lparen",
                                "rparen",
                                "lbracket",
                                "rbracket",
                                "lbrace",
                                "rbrace",
                                "comment",
                                "add",
                                "sub",
                                "div",
                                "intdiv",
                                "mult",
                                "mod",
                                "exp",
                                "less",
                                "greater",
                                "negate",
                                "colon",
                                "underscore",
                                "incr",
                                "decr",
                                "eq_to",
                                "ls_than_eq",
                                "gr_than_eq",
                                "not_eq",
                                "add_asgn",
                                "sub_asgn",
                                "mult_asgn",
                                "div_asgn",
                                "mod_asgn",
                                "intdiv_asgn",
                                "habang",
                                "kung",
                                "sakali",
                                "kundi",
                                "para",
                                "function",
                                "angkat",
                                "subukan",
                                "maliban",
                                "wakas",
                                "tigil",
                                "tuloy",
                                "balik",
                                "AT",
                                "o_kaya",
                                "nasa",
                                "lahat",
                                "bolyan",
                                "tama",
                                "mali",
                                "numero",
                                "bilang",
                                "linya",
                                "linya_lit",
                                "wala",
                                "puntonumero",
                                "doblenumero",
                                "kar",
                                "unknown",
                                "comment_value_single",
                                "comment_value_multi",
                                "capital",
                                "comma",
                                "char_lit",
                                "punto_lit",
                                "lahad",
                                "kuha",
                                "ibura",
                                "from",
                                "address"};

    // printf("TOKEN-NUMBER\tTOKEN-CODE\t\t\tTOKEN-INFO\t\tLEXEME\t\t\tPOS.(LINE,
    // CH)\n"); fprintf(output_ptr,
    // "TOKEN-NUMBER\tTOKEN-CODE\t\t\t\t\tTOKEN-INFO\t\t\tLEXEME\t\t\tPOS.(LINE,
    // CH)\n");

    // printf("---------------------------------------------------------------------------------------------------------------------\n");
    // fprintf(output_ptr,
    // "---------------------------------------------------------------------------------------------------------------------\n");
    int i = 0;
    while ((tokens[i] = lexer_get_next_token(lexer)) != (void *)0) {
        i++;
        tokens = realloc(tokens, (i + 1) * sizeof(token_t *));
        // fprintf(output_ptr, "%s\t%s\t[%d, %d]\n", i,
        // token_type[tokens[i]->type], tokens[i]->value, tokens[i]->lpos,
        // tokens[i]->cpos); printf("%s\t%s\t[%d, %d]\n", i,
        // token_type[tokens[i]->type], tokens[i]->value, tokens[i]->lpos,
        // tokens[i]->cpos); if(token->type == 0 || token->type == 3 ||
        // token->type == 4)
        // {
        //     printf("%d\t\t%s\t\t\t%s\t%s\t\t[%d, %d]\n", i,
        //     token_code[token->type], token_type[token->type], token->value,
        //     token->lpos, token->cpos); if(token->type == 3 || token->type ==
        //     4)
        //     {
        //         fprintf(output_ptr, "%d\t\t\t\t%s\t\t\t\t%s\t%s\t\t\t[%d,
        //         %d]\n", i, token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        //     else
        //     {
        //         fprintf(output_ptr, "%d\t\t\t\t%s\t\t\t\t\t%s\t%s\t\t\t[%d,
        //         %d]\n", i, token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        // }
        // else if(token->type == 1 || token->type == 2 || token->type == 57 ||
        // token->type == 55 || token->type == 65 || token->type == 67 ||
        // token->type == 45 || token->type == 7 || token->type == 8 ||
        // token->type == 39 || token->type == 22)
        // {
        //     printf("%d\t\t%s\t\t\t%s\t\t%s\t\t\t[%d, %d]\n", i,
        //     token_code[token->type], token_type[token->type], token->value,
        //     token->lpos, token->cpos); if(token->type == 1 || token->type ==
        //     7 || token->type == 8)
        //     {
        //         fprintf(output_ptr, "%d\t\t\t\t%s\t\t\t\t%s\t\t\t%s\t\t\t[%d,
        //         %d]\n", i, token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        //     else if(token->type == 22)
        //     {
        //         fprintf(output_ptr,
        //         "%d\t\t\t\t%s\t\t\t\t\t%s\t\t\t%s\t\t\t[%d, %d]\n", i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        //     else if(token->type == 2 || token->type == 55)
        //     {
        //         fprintf(output_ptr,
        //         "%d\t\t\t\t%s\t\t\t\t\t%s\t\t\t%s\t\t\t[%d, %d]\n", i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        //     else if(token->type == 65 || token->type == 39 || token->type ==
        //     45)
        //     {
        //         fprintf(output_ptr, "%d\t\t\t\t%s\t\t\t\t%s\t\t\t%s\t\t\t[%d,
        //         %d]\n", i, token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        //     else
        //     {
        //         fprintf(output_ptr, "%d\t\t\t\t%s\t\t\t\t%s\t\t%s\t\t\t[%d,
        //         %d]\n", i, token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        // }
        // else if(token->type == 63 || token->type == 64)
        // {
        //     if(token->type == 63)
        //     {
        //         printf("%d\t\t%s\t%s\t\t%s\t[%d, %d]\n", i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos); fprintf(output_ptr,
        //         "%d\t\t\t\t%s\t%s\t\t%s\t\t\t[%d, %d]\n", i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        //     else
        //     {
        //         for(int i=0; i<strlen(token->value); i++)
        //             if(token->value[i] == '\n')
        //                 token->value[i] = ' ';
        //         printf("%d\t\t%s\t%s\t%s\t[%d, %d] \n",  i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos); fprintf(output_ptr,
        //         "%d\t\t\t\t%s\t%s\t%s\t\t\t[%d, %d]\n", i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        // }
        // else
        // {
        //     if(token->type == 25)
        //     {
        //         printf("%d\t\t%s\t\t%s\t%s\t[%d, %d]\n", i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos); fprintf(output_ptr,
        //         "%d\t\t\t\t%s\t\t%s\t%s\t\t\t[%d, %d]\n", i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos);
        //     }
        //     else
        //     {
        //         printf("%d\t\t%s\t\t\t%s\t\t\t%s\t\t\t[%d, %d]\n", i,
        //         token_code[token->type], token_type[token->type],
        //         token->value, token->lpos, token->cpos); if(token->type == 62
        //         || token->type == 40 || token->type == 46 || token->type ==
        //         42 || token->type == 50)
        //         {
        //             fprintf(output_ptr,
        //             "%d\t\t\t\t%s\t\t\t\t%s\t\t\t\t%s\t\t\t[%d, %d]\n", i,
        //             token_code[token->type], token_type[token->type],
        //             token->value, token->lpos, token->cpos);
        //         }
        //         else if(token->type == 37)
        //         {
        //             fprintf(output_ptr,
        //             "%d\t\t\t\t%s\t\t\t\t\t%s\t\t\t\t%s\t\t\t[%d, %d]\n", i,
        //             token_code[token->type], token_type[token->type],
        //             token->value, token->lpos, token->cpos);
        //         }
        //         else if(token->type == 41 || token->type == 38)
        //         {
        //             fprintf(output_ptr,
        //             "%d\t\t\t\t%s\t\t\t\t\t%s\t\t\t\t\t%s\t\t\t[%d, %d]\n",
        //             i, token_code[token->type], token_type[token->type],
        //             token->value, token->lpos, token->cpos);
        //         }
        //         else if(token->type == 47 || token->type == 48 || token->type
        //         == 49)
        //         {
        //             fprintf(output_ptr,
        //             "%d\t\t\t\t%s\t\t\t\t\t%s\t\t\t\t\t%s\t\t\t[%d, %d]\n",
        //             i, token_code[token->type], token_type[token->type],
        //             token->value, token->lpos, token->cpos);
        //         }
        //         else
        //         {
        //             fprintf(output_ptr,
        //             "%d\t\t\t\t%s\t\t\t\t\t%s\t\t\t\t%s\t\t\t[%d, %d] \n", i,
        //             token_code[token->type], token_type[token->type],
        //             token->value, token->lpos, token->cpos);
        //         }
        //     }
        // }
        // i++;
        // printf("---------------------------------------------------------------------------------------------------------------------\n");
        // fprintf(output_ptr,
        // "---------------------------------------------------------------------------------------------------------------------\n");
    }

    for (int x = 0; x < i; x++) {
        if (tokens[x]->type == COMMENT_VALUE_MULTI ||
            tokens[x]->type == COMMENT_VALUE_SINGLE) {
            fprintf(output_ptr, "%s\t\t\t\t\t\t\t\t\t\t\t[%d, %d]\n",
                    token_code[tokens[x]->type], tokens[x]->lpos,
                    tokens[x]->cpos);
        } else {
            fprintf(output_ptr, "%s\t\t\t\t\t\t%s\t\t\t\t\t[%d, %d]\n",
                    token_code[tokens[x]->type], tokens[x]->value,
                    tokens[x]->lpos, tokens[x]->cpos);
        }
        // printf("%d\t\t%s\t\t\t%s\t%s\t\t[%d, %d]\n", x,
        // token_code[tokens[x]->type], token_type[tokens[x]->type],
        // tokens[x]->value, tokens[x]->lpos, tokens[x]->cpos);
    }
    printf("Symbol Table Generated..\n");
    exec_parser(parser_init(tokens));
}