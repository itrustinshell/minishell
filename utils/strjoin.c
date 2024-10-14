#include "../minishell.h"

char *strjoin(char *str, char *str_toadd)
{
	int lenstr;
	int len_strtoadd;
	char *str_toret;
	int i;
	int j;

	lenstr = ft_strlen(str);
	len_strtoadd = ft_strlen(str_toadd);
	str_toret = (char *)malloc((lenstr + len_strtoadd + 1) * sizeof(char));
	i = 0;
	while(str[i])
	{	
		str_toret[i] = str[i];
		i++;
	}
	j = 0;
	while(str_toadd[j])
	{
		str_toret[i + j] = str_toadd[j];
		j++;
	}
	return str_toret;
}


