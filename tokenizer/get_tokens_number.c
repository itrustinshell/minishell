#include "minishell.h"
// get_tokens_number
/*
AAA quando dopo < metto solo una lettera ad es <d
non riconosce la d come token separato da <

 */


/*given a char it checks if it is a space*/
int	ft_isspace(char char_to_check)
{
	if (char_to_check == SPACE)
		return (IS_SPACE);
	else if(char_to_check == TABULATION)
		return (IS_SPACE);
	else if (char_to_check == NEW_LINE)
		return (IS_SPACE);
	return (IS_NOT_SPACE);
}

/*given a char it checks if it is a given symbol*/
int	ft_issymbol(char char_to_check)
{
	if (char_to_check == INPUT_REDIRECTION)
		return (IS_SYMBOL);
	else if(char_to_check == OUTPUT_REDIRECTION)
		return (IS_SYMBOL);
	else if (char_to_check == PIPE)
		return (IS_SYMBOL);
	else if (char_to_check == DOLLAR_SIGN)
		return (IS_SYMBOL);
	else if (char_to_check == '\'')
		return (IS_SYMBOL);
	else if (char_to_check == DOUBLE_QUOTE)
		return (IS_SYMBOL);
	return (IS_NOT_SYMBOL);
}


int symbol_iteration(char *str, int *iterator, char token)
{
	int	todo; //da cancellare in futuro
	int i;
	
	i = *iterator;
	if (token == '\'' || token == DOUBLE_QUOTE)
	{
		i++; //mi sposo a destra dell'apice di apertura
		while (str[i] && str[i] != token)
			i++; //quando esce dovrebbe essere sull'apice di chiusura a meno che non esista
		if (str[i] != token) //quando le virgolette non si chiudono
		{
			write(1,"Error message: unclosed quotes.\n", 32);
			return (-1);
		}
	}
	else if (token == DOLLAR_SIGN)
	{
		if (str[i] == DOLLAR_SIGN && !ft_isspace(str[i + 1]))
		{
			i++; //mi sposto a destra del simbolo di dollaro (aaa stiamo parlanod di variabili)
			while (str[i] && !ft_isspace(str[i]) && !ft_issymbol(str[i]))
				i++;
			if (ft_issymbol(str[i]))
				i--; //quando esce dal hilw si trova su un simbolo o su uno spazio
		}
	}
	*iterator = i;
	return (1);
}

/*trova un simbolo e aggiorna l'indice lungo la stringa da tokenizzare ritornando il numero di tokens*/
int	check_symbols(char *str_tocheck, int *iterator)
{
	int n_tokens;
	int padlock;
	int i;

	i = *iterator;
	n_tokens = 0;
	padlock = UNLOCKED;
	if (ft_issymbol(str_tocheck[i]))
	{
		n_tokens += 1;
		if (str_tocheck[i] == '\'')
		{
			if(symbol_iteration(str_tocheck, &i, '\'') < 0)
				return (-1);
		}
		else if (str_tocheck[i] == DOUBLE_QUOTE)
		{
			if (symbol_iteration(str_tocheck, &i, DOUBLE_QUOTE) < 0)//AAA manage solo quando c'è il dollar sign
				return (-1);
		}
		else if (str_tocheck[i] == DOLLAR_SIGN)
			symbol_iteration(str_tocheck, &i, DOLLAR_SIGN);
		else if (str_tocheck[i] == INPUT_REDIRECTION && str_tocheck[i + 1] == INPUT_REDIRECTION)
			i += 1;
		else if (str_tocheck[i] == OUTPUT_REDIRECTION && str_tocheck[i + 1] == OUTPUT_REDIRECTION)
			i += 1;
		//else
		//	i++;
	}
	*iterator = i;
	return (n_tokens);
}

/*given a string, it calculates how many words it is composed of*/
int get_num_of_tokens(char *str_to_tokenize)
{
	int n_tokens;
	int padlock;
	int i;
	int is_symbol;

	n_tokens = 0;
	i = -1;
	while (str_to_tokenize[++i])
	{
		while (ft_isspace(str_to_tokenize[i]))
			i++;
		padlock = UNLOCKED;
		while (str_to_tokenize[i] && !ft_isspace(str_to_tokenize[i]) && !ft_issymbol(str_to_tokenize[i]))
		{
			if (padlock == UNLOCKED)
			{
				n_tokens += 1;
				padlock = LOCKED;
			}
			i++;
		}
		if ((is_symbol = check_symbols(str_to_tokenize, &i)) >= 0)
			n_tokens += is_symbol;
		else if (is_symbol < 0)
			return (-55);				
	}
	return (n_tokens);
}

