#include <stdio.h>

#define MAX_IDX 25
#define WORD_LENGHT 50
#define FILE_NAME_LENGHT 40

typedef struct word_info
{
	char file_name[FILE_NAME_LENGHT];
	int word_count;
	struct word_info *next;
}word_i;

typedef struct file_info
{
	char word[WORD_LENGHT];
	int file_count;
	word_i *file_table;
	struct file_info *f_next;

}file_i;

typedef struct data_base
{
	int index;
	file_i *data;
	struct data_base *d_next;

}data_base_t;

int copy_to_file(FILE *fp1,char *str,int c);
int save_data_base(data_base_t *head,FILE *fp1);
int update_data_base(data_base_t **head,FILE *n_file,FILE *b_file,char *n_f_name);
int re_create_data_base(data_base_t **head,FILE *b_file);
int index_conv(char ch);
int create_data_base(data_base_t **head);
int display_data_base(data_base_t **,FILE *fp1);
int search_data_base(data_base_t **,FILE *fp1,char *word);
int insert_data_base(data_base_t **head,char *str,char *argv_file);

