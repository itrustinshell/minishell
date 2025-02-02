// #include "../minishell.h"

// #include <stdlib.h>
// #include <string.h>
// #include <stdbool.h>

// int count_pipes(const char *input)
// {
//     int     count;


//     if (!input)
//         return NULL;
//     count = 0;
//     while (*input)
//     {
//         if (*input == '\\')
//         {
//             input += 2;
//             continue ;
//         }
//         if (*input == '"')
//             while (*(++input) != '"');
//         if (*input[i] == '\'')
//             while (input[++i] != '\'');
//         if (input[i] == '|')
//             count++;
//     }
// }

// char** shell_split(const char* input) {
//     char    **result;
//     int     i;

//     if (!input)
//         return NULL;
//     i = 0;
//     while (input[i])
//     {
//         if ()
//     }
// }

// // Funzione di utilità per liberare la memoria
// void free_shell_split(char** tokens, int count) {
//     if (!tokens) return;
//     for (int i = 0; i < count; i++) {
//         free(tokens[i]);
//     }
//     free(tokens);
// }