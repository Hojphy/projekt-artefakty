#include "artifacts.h"

void init_archive(Archive *arch) 
{ 
    arch->count = 0;
    arch->head = NULL;
}

void free_archive(Archive *arch)
{
    Node *current = arch->head;
    
    while (current != NULL) 
    {
        Node *next_node = current->next;
        free(current);
        current = next_node;
    }

    arch->head = NULL;
    arch->count = 0;
}

int add_artifact(Archive *arch, Artifact new_art) 
{
    Node* new = calloc(1, sizeof(Node));
    if (new == NULL)
    {
        return 0;
    }
    new->data = new_art;
    new->next = arch->head;
    arch->head = new;
    ++arch->count;
    return 1;
}

Artifact* find_artifact_by_name(Archive *arch, const char *name)
{
    Node *current = arch->head;
    while (current != NULL && strcmp(current->data.name, name) != 0) 
    {
        current = current->next;
    }
    if(current == NULL) return NULL;
    return &current->data;
}

int edit_artifact(Archive *arch, const char* name, Artifact new_art)
{
    Artifact *target = find_artifact_by_name(arch, name);

    if (target == NULL) return 0;

    char original_name[100];
    strcpy(original_name, target->name);

    *target = new_art;

    strcpy(target->name, original_name);
    return 1;
}

int remove_artifact_by_name(Archive *arch, const char *name)
{
    Node *current = arch->head;
    Node *prev = NULL;
    while (current != NULL && strcmp(current->data.name, name) != 0)
    {
        prev = current;
        current = current->next;
    }

    if (current == NULL) return 0;
    if (current->data.threat_level >= 8) return -1;

    if (prev == NULL)
    {
        arch->head = current->next;
    }
    else 
    {
        prev->next = current->next;
    }
    free(current);
    --arch->count;
    return 1;
}

int save_to_file(const Archive *arch, const char *filename) 
{
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;

    fwrite(&arch->count, sizeof(int), 1, fp);

    Node *current = arch->head;
    while (current != NULL) 
    {
        fwrite(&current->data, sizeof(Artifact), 1, fp);
        current = current->next;
    }

    fclose(fp);
    return 1; 
}

int load_from_file(Archive *arch, const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) return 0;

    if (arch->head != NULL) 
    {
        free_archive(arch);
    }

    int arch_size = 0;
    if (fread(&arch_size, sizeof(int), 1, fp) != 1) 
    {
        fclose(fp);
        return 0;
    }

    Artifact temp_art;
    for (int i = 0; i < arch_size; i++)
    {
        if (fread(&temp_art, sizeof(Artifact), 1, fp) == 1) 
        {
            add_artifact(arch, temp_art);
        }
    }

    fclose(fp);
    return 1;
}

static int compare_artifacts(Artifact a, Artifact b, int sort_mode) 
{
    switch (sort_mode) 
    {
        case NAME_SORT:    return strcmp(a.name, b.name);
        case ORIGIN_SORT:  return strcmp(a.origin, b.origin);
        case CREATOR_SORT: return strcmp(a.creator_civilization, b.creator_civilization);
        case STATUS_SORT:  return strcmp(a.status, b.status);
        
        case THREAT_SORT:  return a.threat_level - b.threat_level;
        case YEAR_SORT:    return a.discovery_year - b.discovery_year;
        
        default: return 0;
    }
}

static int should_swap(int cmp_result, int is_descending) 
{
    if (is_descending) 
    {
        return cmp_result > 0;
    }
    return cmp_result < 0;
}

Node* insert_node(Node* sorted_head, Node* new_node, int mode) 
{
    if (sorted_head == NULL) 
    {
        new_node->next = NULL;
        return new_node;
    }

    int sort_mode = mode & SORT_MASK; // 0001 0100 & 0000 1111 => 0000 0100 (usuwamy kierunek sortowania)
    int is_descending = mode & DESCENDING_SORT; // (ASC) 0000 0100 & (DESC) 0001 0000 => 0000 0000 - falsz
  
    int comparator = compare_artifacts(new_node->data, sorted_head->data, sort_mode);
  
    if (should_swap(comparator, is_descending)) 
    {
        new_node->next = sorted_head;
        return new_node;
    }

    int found = 0;  
    Node* p = sorted_head;
    while (p->next != NULL && !found) 
    {
        comparator = compare_artifacts(new_node->data, p->next->data, sort_mode);
        if (should_swap(comparator, is_descending))
        {
            found = 1;
        } 
        else 
        {
            p = p->next;
        }
    }
    new_node->next = p->next;
    p->next = new_node;

    return sorted_head;
}

Archive* sort_archive(Archive* arch, int mode) 
{
    if (arch->head == NULL) return arch;

    Node* sorted = NULL;
    Node* current = arch->head;

    while (current != NULL)
    {
        Node* next = current->next;
        current->next = NULL;

        sorted = insert_node(sorted, current, mode);

        current = next;
    }
    arch->head = sorted;
    return arch;
}

void print_table_header() 
{
    printf("%-30s | %-15s | %-15s | %-5s | %-6s | %-15s\n", 
           "NAZWA", "POCHODZENIE",  "CYWILIZACJA", "ZAGR.", "ROK", "STATUS");
    printf("---------------------------------------------------------------------------------------------------\n");
}

void print_table_row(Artifact *a) 
{
    printf("%-30s | %-15s | %-15s | %-5d | %-6d | %-15s\n", 
           a->name, a->origin, a->creator_civilization, a->threat_level, a->discovery_year, a->status);
}

void print_artifacts_by_fragment(const Archive *arch, const char *fragment)
{
    Node* current = arch->head;
    int len = strlen(fragment);
    int not_empty = 0;

    printf("\nWyniki wyszukiwania dla '%s':\n", fragment);
    print_table_header();
    while (current != NULL)
    {
        if (strncmp(current->data.name, fragment, len) == 0)
        {
            print_table_row(&current->data);
            not_empty = 1;
        }
        current = current->next;
    }

    if (!not_empty)
    {
        printf("Brak wynikow.\n");
    }
}

void print_artifacts_by_threat(const Archive *arch, const int min_level)
{
    Node* current = arch->head;
    int not_empty = 0;
    printf("\nWyniki wyszukiwania dla poziomu '%d' i wyzszego:\n", min_level);
    print_table_header();
    while (current != NULL)
    {
        if (current->data.threat_level >= min_level)
        {
            print_table_row(&current->data);
            not_empty = 1;
        }
        current = current->next;
    }

    if (!not_empty)
    {
        printf("Brak wynikow.\n");
    }
}

void handle_artifact_status(Artifact* art)
{
    int level = art->threat_level;
    if (level >= 9) {
        strcpy(art->status, "ZAKAZANY");
    } 
    else if (level >= 7) {
        strcpy(art->status, "Wymaga kwarantanny");
    } 
    else if (level >= 5) {
        strcpy(art->status, "Niestabilny");
    } 
    else if (level >= 3) {
        strcpy(art->status, "W trakcie badan");
    } 
    else {
        strcpy(art->status, "Bezpieczny");
    }
}


void print_archive(const Archive *arch) 
{
    if (arch->head == NULL) {
        printf("[Baza danych jest pusta]\n");
        return;
    }

    print_table_header();

    Node *current = arch->head;
    while (current != NULL) 
    {
        print_table_row(&current->data);
        current = current->next;
    }
    printf("---------------------------------------------------------------------------------------------------\n");
    printf("Razem: %d obiektow.\n", arch->count);
}
