
int pipex(t_command *cmdlist, int cmdlist_len, int **pipematrix) 
{
    pid_t pid;
    int i;
    int f = 1;
    int father = getpid();
    t_command *tmp_cmdlist = cmdlist;

    if (!tmp_cmdlist) 
    {
        printf("la lista è nulla\n");
        return (0);
    }
    printlist(tmp_cmdlist);
    i = -1;
    printf("lunghezza cmdlist: %d\n", cmdlist_len);

    // Ciclo per eseguire tutti i comandi della pipeline
    while (++i < cmdlist_len) 
    {
        printf("i: %d\n", i);
        if (i > 0) {
            tmp_cmdlist = tmp_cmdlist->next;
            if (tmp_cmdlist)
                printf("figlio: %d, cmd successivo: %s\n", i, tmp_cmdlist->cmd);
            else
                printf(RED"AAA: il comando successivo è nullo. Non si cicla più lungo i comandi\n"RESET);
        }

        printf("parent: sono nel comando %s\n", tmp_cmdlist->cmd);
        pid = fork();
        
        if (pid == 0) {  // Processo figlio
            printf("child %d: sono nel comando %s\n", i, tmp_cmdlist->cmd);

            if (i > 0) {  // Se non è il primo comando, leggi dalla pipe precedente
                printf(GREEN"leggerò dalla pipe\n"RESET);
                close(pipematrix[i - 1][WRITE_END]);
                dup2(pipematrix[i - 1][READ_END], STDIN_FILENO);
                close(pipematrix[i - 1][READ_END]);
            }

            if (tmp_cmdlist->next) {  // Se non è l'ultimo comando, scrivi nella pipe successiva
                printf(GREEN"sto per scrivere nella pipe\n"RESET);
                close(pipematrix[i][READ_END]);
                dup2(pipematrix[i][WRITE_END], STDOUT_FILENO);
                close(pipematrix[i][WRITE_END]);
            }

            // Chiudi tutte le pipe inutilizzate
            for (int n = 0; n < (cmdlist_len - 1); n++) {
                close(pipematrix[n][READ_END]);
                close(pipematrix[n][WRITE_END]);
            }

            tmp_cmdlist->path = get_cmdpath(tmp_cmdlist->cmd);
            printf("%d: eseguo\n", i);
            printf(MAGENTA"sto eseguendo il figlio  %d\n\n"RESET, i);
            execve(tmp_cmdlist->path, tmp_cmdlist->args, NULL);
            perror("execve fallita");
            exit(1);  // Se execve fallisce
        }
    }

    // Processo padre: man mano che si avanza chiude le pipe precedentemente aperte
    printf(MAGENTA"\nAAAAAAA Sono nel padre:\n"RESET);
    for (i = 0; i < (cmdlist_len - 1); i++) {
        printf("ho chiuso\n");
        close(pipematrix[i][READ_END]);
        close(pipematrix[i][WRITE_END]);
    }

    // Il padre aspetta la terminazione di tutti i figli
    while ((pid = wait(NULL)) > 0) {
        // printf("Figlio n. %d: terminato (exit-code: %d)\n", f, pid);
        f++;
    }
    return (0);
}
