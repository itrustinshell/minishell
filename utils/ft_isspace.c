#include "../minishell.h"

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


