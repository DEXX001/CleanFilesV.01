#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "scan.h"

void print_indent(int niveau)
{
    for (int i = 0; i < niveau; i++)
    {
        printf("    ");
    }
    
}

void afficher_element(const char *type, const char *nom, int niveau)
{
    print_indent(niveau);
    printf("%s : %s\n", type, nom);
}

void scan_dir(const char * path,
              long long * total_size,
              long long * unused_size,
              int * dir_count,
              int * file_count,
              int * unused_count,
              int deepth)
{
    DIR *dir = opendir(path);
    struct stat st;
    struct dirent *entry;
    char fullpath[2000];

    if (dir == NULL)
    {
        printf("Erreur d'ouverture de dossier !");
        exit(EXIT_FAILURE);
    }

    /* 
    else
    {
        printf("Dossier ouvert avec succès");
        printf("\n======================\n");
    } 
    */

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;
        
        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);
        if (stat(fullpath, &st) != 0)
        {
            continue;
        }

        if (S_ISDIR(st.st_mode))
        {
            afficher_element("DIR", entry->d_name, deepth);
            (*dir_count)++;
            scan_dir(fullpath, total_size, unused_size, 
                     dir_count, file_count, unused_count, deepth + 1);
            
        }

        else if (S_ISREG(st.st_mode)) 
        {
            int is_unused = 0;
            const char *look = strrchr(entry->d_name, '.');
            total_size += st.st_size;

            if (look && (strcmp(look, ".tmp") == 0 ||
                         strcmp(look, ".log") == 0 ||
                         strcmp(look, ".bak") == 0 ||
                         strcmp(look, ".old") == 0))
            {
                is_unused = 1;
                unused_size += st.st_size;
                (*unused_count)++;
            }

            if (is_unused)
                afficher_element("UNUSED", entry->d_name, deepth);
            else
            {
                afficher_element("FILE", entry->d_name, deepth);
                (*file_count)++;
            }
        }
        
        else
        {
            printf("[OTHER] -- > %s", entry->d_name);
        }
    }

    closedir(dir);
}

int main(int ac, char **av)
{
    if (ac != 2)
    {
        printf("Utilisation : ./cleanfiles <chemin_du_dossier>\n");
        return (EXIT_FAILURE);
    }

    long long total_size = 0;
    long long unused_size = 0;
    int dir_count = 0;
    int file_count = 0;
    int unused_count = 0;

    printf("\nDossier cible : %s", av[1]);
    printf("\n======================\n");


    if (ac == 2)
    {
        scan_dir(av[1], &total_size, &unused_size, &dir_count, 
                 &file_count, &unused_count, 0);  

        printf("\n=======================\n");
        printf("=======================\n\n");

        printf("Dossier --> %d\n", dir_count);
        printf("Fichier --> %d\n", file_count);
        printf("A supprimer --> %d\n", unused_count);
        printf("TAILLE TOTAL : %lld bytes\n", total_size);
        printf("Inutile taille --> %lld\n", unused_size);

        printf("\n=======================\n");
        printf("=======================\n");
        
    }
      
    else
    {
        printf("Utilisation : ./cleanfiles <chemin_du_dossier>\n");
    }

    return (EXIT_SUCCESS);
}
