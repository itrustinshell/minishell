#include "minishell.h"

//fill token
/*
AAA quando dopo < metto solo una lettera ad es <d
non riconosce la d come token separato da <

 */
char **matrixalloc(int n_tokens)
{
	char **matrix_to_ret;

	matrix_to_ret = NULL;
	matrix_to_ret = (char **)malloc((n_tokens + 1) * sizeof(char *));
	if (!matrix_to_ret)
		return (NULL);
	return (matrix_to_ret);
}

char *create_token(char *str, int i, int len)
{
	char *token;
	int t;

	token = (char *)malloc((len + 1) * sizeof(char));
	if (token == NULL)
		return (NULL);
	t = 0;
	while (t < len)
	{
		token[t] = str[i + t];
		t++;
	}
	token[t] = '\0';
	printf("token_len: %d\n",t);
	return (token);
}

void free_matrix(char **tokens_matrix)
{
	int t;

	t = 0;
	while (tokens_matrix[t])
	{
		free(tokens_matrix[t]);
			t++;
	}
	free(tokens_matrix);
}

int malloc_if_token_is_str(char **tokens_matrix, int *t_index, char *str_to_tokenize, int *iterator)
{
		int i;
		int t;
		int len;
		
		i = *iterator;
		t = *t_index;
		len = 0;
		while (str_to_tokenize[i + len] && !ft_isspace(str_to_tokenize[i + len]) && !ft_issymbol(str_to_tokenize[i + len]))
			len++;

		if (len > 0)
		{
			tokens_matrix[t] = create_token(str_to_tokenize, i, len);
			if (tokens_matrix[t] == NULL)
			{
				free_matrix(tokens_matrix);
				return (-1);
			}
			i = i + len;
			t++;
		}
		*iterator = i;
		*t_index = t;
		return (1);
}

int  malloc_if_token_is_symbol(char **tokens_matrix, int *t_index, char *str_to_tokenize, int *iterator)
{
	int i;
	int t;
	int len;
	int starting_index;

	i = *iterator;
	t = *t_index;
	starting_index = i;
	if (check_symbols(str_to_tokenize, &i) > 0) //if a symbol was find
	{	
		len = i - starting_index;//nota 33
		//starting_index = i - len;//nota 66
		/*if (str_to_tokenize[starting_index] == DOUBLE_QUOTE || str_to_tokenize[starting_index] == SINGLE_QUOTE || str_to_tokenize[starting_index] == DOLLAR_SIGN || str_to_tokenize[starting_index] == INPUT_REDIRECTION || str_to_tokenize[starting_index] == OUTPUT_REDIRECTION)
			len = len + 1; //nota XXX
			*/
		/*if (str_to_tokenize[starting_index] == DOLLAR_SIGN)
			len = len + 1;
		if(str_to_tokenize[starting_index] == INPUT_REDIRECTION || str_to_tokenize[starting_index] == OUTPUT_REDIRECTION)
			len = len + 1;//nota 44*/
		if (ft_issymbol(str_to_tokenize[starting_index])) 
			len = len + 1;
	//	if (len == 0)
	//		len = 1; //nota 88
		tokens_matrix[t] = create_token(str_to_tokenize, starting_index, len);
		//todo return -1
		t++;
	}
	*iterator = i;
	*t_index = t;
	return (1);
}


/*given a string, it calculates how many words it is composed of*/
char **create_tokenmatrix(char *str_to_tokenize, int n_tokens)
{
	char **tokens_matrix;
	int tmp;
	int i;
	int t;
	int len;

	tokens_matrix = matrixalloc(n_tokens);
	if (!tokens_matrix)
		return (NULL);
	t = 0;
	i = 0;
	while (t < n_tokens)
	{
		while (ft_isspace(str_to_tokenize[i]))
			i++;
		if (malloc_if_token_is_str(tokens_matrix, &t, str_to_tokenize, &i) < 0)
			return (NULL);
		malloc_if_token_is_symbol(tokens_matrix, &t, str_to_tokenize, &i); 
		//TODO anche qui gestisci una eventual mancata malloccamento
		i++;
	}
	tokens_matrix[t] = NULL;
	return tokens_matrix;
}

// nota 33 la i potenzialmente è andata avanti in chec_symbol. La sua precedente posizione è salvata in tmp prima di eseguire check_symbol. se la i non è andata avanti il risultato di questa len sarà quello che deve eessere ovvero 0. Significa che non vi erano simboli
	
/*XXX sappiamo che in checksymbol avviene anche il check su double single e altri element
 i...
 nel caso di double e single il chek consiste nell'iterazione al successivo single o 
 double per accertare la chiusura delle quotes. 
 Pertanto il cursore si fermerebbe sull'ultimo apice...ecop perchè la len qui 
 deve essere aggiunta...perchè io devo includere anche quell'apice e poi inserire un \0..
 .questo ai fini del token.Per intenderci quando hai il $ciao l'iterazione 
 non si ferma sulla o ma su un eventuale spazio  e un eventuaesegno...
 questi utlimi non li vogliao mettere nel token che terminerà appunto con la o di ciao e il nullterminator predisposto dalla funzione create_token. Nell'altro caso, con le quotes,,il termine è appunto un altra quotes " (che se ci pensi sarebbe un simbolo...proprio come il dollar sign finiva o con uno spazio o cono un siimbolo...in questo caso termina con il simbolo i quotes...., ma questa volta noi vogliamo inserire questa nel token...pertanto in questa funzione daremo una len aumentata di uno da passare a create token che partendo dallo strting point che gli abbiamo dato, si posiazione sullo starting nella stringa e itera per quella len...aggiungendo poi a sua volta il suo spazio per \0.
*/	

	/*nota 44 anche in tal caso io so che se c'è un < possono accadere due casi: o che è singolo o che è doppio. Nel primo caso avrò una len di zero...e non mi basta perchè poi in create_tken io aggiungo solo una posizione...in questa funzione infatti ogni volta che la len è zero e contiene un valore che devo restituire allora devo già qui aumentarla di uno...ovvero di quella posizione da riservare appunto al char che voglio restituire come token. Nell'altro caso di cui sopra avremo due consecutivi <<. in tal caso io so che avrò avuto una iterazione prima in checksymbol che avrà mandato la i proprio sullìultimo <...ma come nel caso dele virgolette io questo ultimo voglio inserirlo nel token...quindi aumenterò di uno la len anche in tal caso.*/


 //nota 88 qui sostanzilamente sono nella condizione piu generale in qui ho un simbolo. Quinsdi entrando qui io so che ho un simbolo. Ora se ho len zero devo sempre aumentarla di uno per riservare appunto spazioe a quel simbolo da mandare in stampa.

 //nota 66 l'eventuale starting point da cui copiare il token è appunto i - la lunghezza appena trovata. Se la lunghezza è 0 sarà allora la i stessa (in tal caso la i non sarà andata avanti)
	
