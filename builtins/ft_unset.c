#include "../minishell.h"

int ft_unset(char *namevar, t_env **list)
{  
    t_env *tmp;
    t_env *variable;

    tmp = *list;
    

    variable = access_envar(namevar, tmp); //verifica se esiste la variabile
    //TODO: ho verificato l'accesso e restituito il nodo ...
    //se c0p nodo questo lo devo togliere dalla lista...qui scrivo la funzione
    if (variable)
    {
        if (variable->next != NULL && variable->prev != NULL)
        {
            variable->prev->next = variable->next; //TODO hai messo il prevo ? 
            variable->next->prev = variable->prev;
            free(variable);
            variable = NULL;
        }
        else if (variable->next == NULL)
        {
            variable->prev->next = NULL;
            free(variable);
            variable = NULL;
        }
        else if (variable->prev == NULL)
        {
            variable = variable->next;
            free(variable->prev);
            variable->prev = NULL;
            *list = variable;
        }
    }
    return (1);
}
