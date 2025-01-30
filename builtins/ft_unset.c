#include "../minishell.h"

int ft_unset(char *namevar, t_env **list)
{  
    access_envar(namevar, *list); //verifica se esiste la variabile
    //TODO: ho verificato l'accesso e restituito il nodo ...
    //se c0p nodo questo lo devo togliere dalla lista...qui scrivo la funzione
    return (1);
}
