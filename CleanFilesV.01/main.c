#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int ac, char **av)
{    
    printf("Dossier cible : ");
    printf("\n======================\n");


    if (ac == 2)
    {
        printf("%s\n", av[1]);
        DIR *dir = opendir(av[1]);
        struct stat st;
        struct dirent *entry;
        char fullpath[1024];

        if (dir == NULL)
        {
            printf("Erreur d'ouverture de dossier !");
            
            exit(EXIT_FAILURE);    
        }

        else
        {
            printf("Dossier ouvert avec succès");
            printf("\n======================\n");

        }

        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            snprintf(fullpath, sizeof(fullpath), "%s/%s", av[1], entry->d_name);
            stat(fullpath, &st);

            if (S_ISDIR(st.st_mode))
                printf("%s [DIR]\n", entry->d_name);
            else if (S_ISREG(st.st_mode))
                printf("%s [FILE]\n", entry->d_name);
            else
                printf("%s [OTHER]\n", entry->d_name);

        }


        closedir(dir);
    }
    else
    {
        printf("Utilisation : ./cleanfiles <chemin_du_dossier>\n");
    }

    return (EXIT_SUCCESS);
}