/***************************************************************
*    ░▒▓███████▓▒░░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░     *     
*    ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░     *
*    ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░     *
*    ░▒▓█▓▒░░▒▓█▓▒░▒▓██████▓▒░  ░▒▓██████▓▒░ ░▒▓██████▓▒░      *
*    ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░     *
*    ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░     *
*    ░▒▓███████▓▒░░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░     *
*                                                              *
*                     -- CleanfilesV.02 --                     *
*                      ----22/11/2025----                      *
*                                                              *
***************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "scan.h"

#define SEUIL_TAILLE (1024 * 1024)

typedef struct FileEntry
{
    char fullpath[1024];
    long long size;
    int is_dir;
    int is_unused;
    int deepth;

}FileEntry;

FileEntry *entries_tab;
int entry_count = 0;
int entry_capacity = 256;

void afficher_top10_par_taille()
{
    FileEntry temp_tab[entry_count];

    for (int i = 0; i < entry_count; i++)
    {
        temp_tab[i] = entries_tab[i];
    }

    for (int i = 0; i < entry_count - 1; i++)
    {
        int max_index = i;

        for (int j = i + 1; j < entry_count; j++)
        {
            if (temp_tab[j].size > temp_tab[max_index].size)
            {
                max_index = j;
            }
            
        }

        if (max_index != i)
        {
            FileEntry tmp = temp_tab[i];
            temp_tab[i] = temp_tab[max_index];
            temp_tab[max_index] = tmp;
        }
    }
}


void ajouter_entree(const char *fullpath,
                    long long size,
                    int is_dir,
                    int is_unused,
                    int deepth)
{
    if (entry_count == entry_capacity)
    {
        entry_capacity *= 2;
        FileEntry *new_entries_tab = realloc(entries_tab, 
                                             entry_capacity * sizeof(FileEntry));
        
        if (new_entries_tab == NULL)
        {
            printf("Erreur realloc !");
            exit(EXIT_FAILURE);
        }

        entries_tab = new_entries_tab;
    }

    strcpy(entries_tab[entry_count].fullpath, fullpath);
    entries_tab[entry_count].size = size;
    entries_tab[entry_count].is_dir = is_dir;
    entries_tab[entry_count].is_unused = is_unused;
    entries_tab[entry_count].deepth = deepth;

    entry_count++;

}


void afficher_resume(int dir_count, int file_count,
                     long long total_size, long long unused_size,
                     int unused_count)
{
    double valeur;
    double valeur_unused;
    int taille;
    const char *unite;
    const char *unite_unused;
    double unused_pourcentage = 0.0;


    printf("\n=======================");
    printf("\n=======================\n\n");

    printf("Dossiers     : %d\n", dir_count);
    printf("Fichiers     : %d\n", file_count);
    printf("UNUSED       : %d\n", unused_count);

    if (total_size < 1024)
    {
        valeur = total_size;
        unite = "B";
    }

    else if (total_size < 1024 * 1024)
    {
        valeur = (double)total_size / 1024;
        unite = "KB";
    }
    else if(total_size < 1024 * 1024 * 1024)
    {
        valeur = (double)total_size / (1024 * 1024);
        unite = "MB";
    }
    else
    {
        valeur = (double)total_size / (1024 * 1024 * 1024);
        unite = "GB";
    }

    if (unused_size < 1024)
    {
        valeur_unused = unused_size;
        unite_unused = "B";
    }

    else if (unused_size < 1024 * 1024)
    {
        valeur_unused = (double)unused_size / 1024;
        unite_unused = "KB";
    }
    else if(unused_size < 1024 * 1024 * 1024)
    {
        valeur_unused = (double)unused_size / (1024 * 1024);
        unite_unused = "MB";
    }
    else
    {
        valeur_unused = (double)unused_size / (1024 * 1024 * 1024);
        unite_unused = "GB";
    }

    if (total_size == 0)
    {
        printf("Total size   : 0 B\n");
        printf("Unused size  : 0 B\n");
        printf("INUTILE      : 0 %%\n");
    }
    else
    {
        unused_pourcentage = (double)unused_size / (double)total_size * 100.0;
        printf("Total size   : %.2f - %s\n", valeur, unite);
        printf("Unused size  : %.2f - %s\n", valeur_unused, unite_unused);
        printf("INUTILE      : %.2f %%\n", unused_pourcentage);
    }

    printf("\n======================");
    printf("\n======================\n");
    
}

void print_indent(int niveau)
{
    for (int i = 0; i < niveau; i++)
    {
        printf("    ");
    }
    
}

void afficher_element(const char *type, const char *nom, int taille,int niveau)
{
    double valeur;
    const char *unite;
    print_indent(niveau);
    struct stat st;

    if (strcmp("DIR", type) == 0)
    {
        printf("DIR : %s\n", nom);
        return;
    }
    
    else
    {
        if (taille < 1024)
        {
            valeur = taille;
            unite = "B";
        }
        else if (taille < 1024 * 1024)
        {
            valeur = (double)taille / 1024;
            unite = "KB";
        }
        else if (taille < 1024 * 1024 * 1024)
        {
            valeur = (double)taille / (1024 * 1024);
            unite = "MB";
        }
        else
        {
            valeur = (double)taille / (1024 * 1024 * 1024);
            unite = "GB";
        }

        printf("%s : %s (%.2f - %s)\n", type, nom, valeur, unite);
    }

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
            afficher_element("DIR", entry->d_name, st.st_size,deepth);
            (*dir_count)++;
            scan_dir(fullpath, total_size, unused_size, 
                     dir_count, file_count, unused_count, deepth + 1);
            
            ajouter_entree(fullpath, st.st_size, 1, 0, deepth);
        }

        else if (S_ISREG(st.st_mode)) 
        {
            int is_unused = 0;
            const char *look = strrchr(entry->d_name, '.');
            *total_size += st.st_size;
            long file_size = st.st_size;

            if (look && (strcmp(look, ".tmp") == 0 ||
                         strcmp(look, ".log") == 0 ||
                         strcmp(look, ".bak") == 0 ||
                         strcmp(look, ".old") == 0))
            {
                if (file_size > SEUIL_TAILLE)
                {
                    is_unused = 1;
                    *unused_size += st.st_size;
                    (*unused_count)++;
                }
            }

            if (is_unused)
                afficher_element("UNUSED", entry->d_name, st.st_size, deepth);
            else
            {
                afficher_element("FILE", entry->d_name, st.st_size, deepth);
                (*file_count)++;
            }
            ajouter_entree(fullpath, st.st_size, 0, is_unused, deepth);
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
    
    entries_tab = malloc(entry_capacity * sizeof(FileEntry));

    if (entries_tab == NULL)
        exit(EXIT_FAILURE);

    printf("\nDossier cible : %s", av[1]);
    printf("\n======================\n");
    printf("======================\n\n");

    
    scan_dir(av[1], &total_size, &unused_size, &dir_count, 
             &file_count, &unused_count, 0);
             
    printf("\n======= VERIFICATION DU TABLEAU =========\n\n");
    printf("Nombre total d'entrees = %d\n", entry_count);

    int max = 0;
    
    if (entry_count < 10)
        max = entry_count;
    else
        max = 10;

    for (int i = 0; i < max; i++)
    {
        FileEntry e = entries_tab[i];
        printf("[%d] %s | size=%lld | dir=%d | unused=%d | depth=%d\n",
           i, e.fullpath, e.size, e.is_dir, e.is_unused, e.deepth);
    }
    

    afficher_resume (dir_count, file_count,
                     total_size, unused_size,
                     unused_count);     

    free(entries_tab);
    return (EXIT_SUCCESS);
}
