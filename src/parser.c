#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

static char *trim(char *str)
{
    while (isspace(*str))
        str++;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace(*end))
        end--;
    *(end + 1) = '\0';
    return str;
}

ParsedLine parse_line(const char *line)
{
    ParsedLine pl = {0};
    char *copy = strdup(line);
    if (!copy)
        return pl;

    // Remove comments starting with # or ;
    char *comment = strpbrk(copy, "#;");
    if (comment)
        *comment = '\0';

    char *trimmed = trim(copy);
    if (!trimmed || strlen(trimmed) == 0)
    {
        free(copy);
        return pl;
    }

    // Tokenize by whitespace and comma
    char *tokens[32];
    int tok_count = 0;
    char *tok = strtok(trimmed, " \t\r\n,");
    while (tok && tok_count < 32)
    {
        tokens[tok_count++] = tok;
        tok = strtok(NULL, " \t\r\n,");
    }

    if (tok_count == 0)
    {
        free(copy);
        return pl;
    }

    int idx = 0;
    // Label detection: first token ending with ':'
    size_t tlen = strlen(tokens[idx]);
    if (tlen > 0 && tokens[idx][tlen - 1] == ':')
    {
        tokens[idx][tlen - 1] = '\0';
        pl.label = strdup(tokens[idx]);
        idx++;
        if (idx >= tok_count)
        {
            free(copy);
            return pl;
        }
    }

    pl.mnemonic = strdup(tokens[idx]);
    idx++;

    while (idx < tok_count)
    {
        pl.operands = realloc(pl.operands, (pl.operand_count + 1) * sizeof(char *));
        pl.operands[pl.operand_count++] = strdup(tokens[idx]);
        idx++;
    }

    free(copy);
    return pl;
}

void free_parsed_line(ParsedLine *pl)
{
    if (pl->label)
        free(pl->label);
    if (pl->mnemonic)
        free(pl->mnemonic);
    for (int i = 0; i < pl->operand_count; i++)
    {
        free(pl->operands[i]);
    }
    free(pl->operands);
}