void print_indent(int deepth);
void scan_dir(const char * path,
              long long * total_size,
              long long * unused_size,
              int * dir_count,
              int * file_count,
              int * unused_count,
              int deepth);

void ajouter_entree(const char *fullpath,
                    long long size,
                    int is_dir,
                    int is_unused,
                    int deepth);