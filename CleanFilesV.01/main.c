#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int ac, char **av)
{    
    
    printf("\nDossier cible : ");
    printf("\n======================\n");


    if (ac == 2)
    {
        printf("%s\n", av[1]);
        DIR *dir = opendir(av[1]);
        struct stat st;
        struct dirent *entry;
        char fullpath[1024];
        long long total_size = 0;
        long long unused_size = 0;

        int dir_count = 0;
        int file_count = 0;
        int unused_count = 0;

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
            { 
                printf("%s [DIR]\n", entry->d_name);
                dir_count++;
            }
            else if (S_ISREG(st.st_mode))
            {
                file_count++;
                total_size += st.st_size;
                const char *ext = strrchr(entry->d_name, '.');
                int is_unused = 0;

                if (ext &&  (
                     strcmp(ext, ".tmp") == 0 ||
                     strcmp(ext, ".log") == 0 ||
                     strcmp(ext, ".bak") == 0 ||
                     strcmp(ext, ".old") == 0))
                {
                    is_unused = 1;
                    unused_size += st.st_size;
                    unused_count++;
                }

                if (is_unused)
                    printf("%s [UNUSED] (%lld bytes)\n", entry->d_name, (long long)st.st_size);
                else if(!is_unused)
                    printf("%s [FILE]\n", entry->d_name);
    
                
            }
            else
            {
                printf("%s [OTHER]\n", entry->d_name);
            }

            

            

        }

        printf("\n======================\n");
        printf("======================\n\n");

        printf("Dossier --> %d\n", dir_count);
        printf("Fichier --> %d\n", file_count);
        printf("A supprimer --> %d\n", unused_count);
        printf("TAILLE TOTAL : %lld bytes\n", total_size);
        printf("Inutile taille --> %lld\n", unused_size);

        printf("\n======================\n");
        printf("======================\n");

        closedir(dir);
    }
    else
    {
        printf("Utilisation : ./cleanfiles <chemin_du_dossier>\n");
    }

    return (EXIT_SUCCESS);
}