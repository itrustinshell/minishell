#include "../minishell.h"

char** shell_split(const char* input) {
	if (!input)
		return NULL;
	// Conta prima il numero di token per allocare l'array
	int num_tokens = 1;  // Almeno un token
	BOOL in_double_quotes = FALSE;  // "..."
	BOOL in_single_quotes = FALSE;  // '...'
	BOOL escaped = FALSE;
	
	for (int i = 0; input[i]; i++) {
		if (escaped) {
			escaped = FALSE;
			continue;
		}

		switch (input[i]) {
			case '\\':
				escaped = TRUE;
				break;
			case '"':
				if (!in_single_quotes) {
					in_double_quotes = !in_double_quotes;
				}
				break;
			case '\'':
				if (!in_double_quotes) {
					in_single_quotes = !in_single_quotes;
				}
				break;
			case '|':
				if (!in_double_quotes && !in_single_quotes) {
					num_tokens++;
				}
				break;
		}
	}

	// Alloca l'array di stringhe
	char** result = malloc(sizeof(char*) * num_tokens);
	if (!result) return NULL;

	// Buffer temporaneo per costruire ogni token
	int input_len = strlen(input);
	char* temp = malloc(input_len + 1);
	if (!temp) {
		free(result);
		return NULL;
	}

	// Split effettivo
	int token_idx = 0;
	int temp_idx = 0;
	in_double_quotes = FALSE;
	in_single_quotes = FALSE;
	escaped = FALSE;

	for (int i = 0; i <= input_len; i++) {
		if (escaped) {
			if (input[i] != '\0') {  // Preserva il carattere escaped
				temp[temp_idx++] = input[i];
			}
			escaped = FALSE;
			continue;
		}

		// Gestione fine stringa o delimiter
		if (input[i] == '\0' || 
			(input[i] == PIPE && !in_double_quotes && !in_single_quotes)) {
			
			// Fine del token
			temp[temp_idx] = '\0';
			
			// Trim degli spazi
			char* start = temp;
			char* end = temp + temp_idx - 1;
			while (*start == ' ') start++;
			while (end > start && *end == ' ') end--;
			*(end + 1) = '\0';

			// Alloca e copia il token
			result[token_idx] = strdup(start);
			if (!result[token_idx]) {
				// Gestione errore
				for (int j = 0; j < token_idx; j++) {
					free(result[j]);
				}
				free(result);
				free(temp);
				return NULL;
			}

			token_idx++;
			temp_idx = 0;
			
			if (input[i] == '\0') break;
			continue;
		}

		switch (input[i]) {
			case '\\':
				escaped = TRUE;
				break;
			case '"':
				if (!in_single_quotes) {
					in_double_quotes = !in_double_quotes;
				}
				temp[temp_idx++] = input[i];
				break;
			case '\'':
				if (!in_double_quotes) {
					in_single_quotes = !in_single_quotes;
				}
				temp[temp_idx++] = input[i];
				break;
			default:
				temp[temp_idx++] = input[i];
		}
	}

	free(temp);
	return result;
}

// Funzione di utilità per liberare la memoria
void free_shell_split(char** tokens, int count) {
	if (!tokens) return;
	for (int i = 0; i < count; i++) {
		free(tokens[i]);
	}
	free(tokens);
}
