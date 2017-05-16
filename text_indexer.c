#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/text_indexer.h"

/*create data base function is create a list of structurs 
 from A-Z there we can store index and pointer for word info*/
int create_data_base(data_base_t **head)
{
    data_base_t *ptr;
    int i = 2,idx = 0;
    char *argv_op;

    ptr = calloc(1,sizeof(data_base_t)); 
    *head = ptr;

    while (1)
    {
	ptr -> index = idx;

	ptr -> data = NULL;

	if (MAX_IDX == idx++)
	{
	    ptr -> d_next = NULL; 
	    break;
	}
	else
	{
	    ptr -> d_next = calloc(1,sizeof(data_base_t)); 
	}

	ptr = ptr -> d_next;
    }
    return 0;
}

/*this function display data of back_up file*/
int display_data_base(data_base_t **head,FILE *fp1)
{
    data_base_t *ptr;
    char char_idx = '`';
    file_i *temp1;
    word_i *temp2;

    re_create_data_base(head,fp1);

    ptr = *head;
    
    if (ptr == NULL)
	return 0;
    /*from head traveling forword and printiong the info
     if ptr->data if not NULL*/
    while (ptr != NULL)
    {
	char_idx++;
	if(ptr -> data != NULL)
	{
	    printf("%c] : \n",char_idx);

	    printf("==========================================================\n");
	    temp1 = ptr -> data;
	    temp2 = temp1 -> file_table;
	    while (temp1)
	    {
		printf("word        : %s\n",temp1 -> word);
		printf("File count  : %d\n",temp1 -> file_count);

		printf("------------------------------\n");
		printf("%-15s  %-12s\n","File name","word count");
		printf("------------------------------\n");
		temp2 = temp1 -> file_table;
		/*loop for mulltiple file*/
		while (temp2)
		{
		    printf("%-15s  %6d\n",temp2 -> file_name,temp2 -> word_count);
		    temp2 = temp2 -> next;
		}
		printf("------------------------------\n\n");
		temp1 = temp1 -> f_next;
	    }
	    printf("==========================================================\n\n");
	}
	ptr = ptr -> d_next;
    }
    return 1;
}

/*this function insert the word info into the data base*/
int insert_data_base(data_base_t **head,char *str,char *argv_file)
{ 
    data_base_t *ptr;
    file_i *temp;
    word_i *w_i;
    int st;

    char ch = str[0];

    int num = index_conv(ch);

    ptr = *head;

    if (ptr == NULL)
	return 0;

    /*searching for index*/
    while (ptr -> index != num)
    {
	ptr = ptr -> d_next;
    }

    /*loop compare word and file present or not
     if present then go for file, if file present go for word count*/
    if(ptr -> data == NULL)//first time when ptr->data is NULL
    {
	ptr -> data = calloc(1, sizeof(file_i));
	strcpy((ptr -> data)-> word, str);
	(ptr -> data) -> file_count = 1;

	(ptr -> data -> file_table) = calloc(1, sizeof(word_i));
	strcpy((ptr -> data -> file_table) -> file_name,argv_file);
	ptr -> data -> file_table -> word_count = 1;
	ptr -> data -> file_table -> next = NULL;

	(ptr -> data) -> f_next = NULL;
    }
    else
    {
	temp = ptr -> data;
	w_i = temp -> file_table;

	/*compare for word*/
	while((st = strcmp( temp -> word, str)) && (temp -> f_next != NULL))
	{
	    temp = temp -> f_next;
	}

	if (st == 0)
	{
	    /*compare for file name*/
	    while ((st = strcmp(w_i -> file_name,argv_file)) && (w_i -> next != NULL))
	    {
		w_i = w_i -> next;
	    }

	    if(!st) 
	    {
		(w_i -> word_count)++;
	    }
	    else if (w_i -> next == NULL)
	    {
		(temp -> file_count)++;
		w_i -> next = calloc(1, sizeof(word_i));
		strcpy(w_i -> next -> file_name,argv_file);
		( w_i -> next -> word_count) = 1;
		w_i -> next -> next = NULL;
	    }
	}
	else
	{
	    temp -> f_next = calloc(1, sizeof(file_i));
	    strcpy((temp -> f_next) -> word, str);
	    (temp -> f_next -> file_count)++;

	    temp = temp -> f_next;

	    temp -> file_table = calloc(1, sizeof(word_i));
	    strcpy(temp -> file_table-> file_name,argv_file);
	    ( temp -> file_table -> word_count) = 1;
	    temp -> file_table -> next = NULL;

	    temp -> f_next = NULL;
	}
    }

    return 1;
}

/*update data base recives new file and update the data in data base*/
int update_data_base(data_base_t **head,FILE *n_file,FILE *b_file,char *n_f_name)
{
    char word[30];
    int i,j,f_count,w_count;

    if (*head == NULL)
	return 0;
    re_create_data_base(head,b_file);
    
    while (fscanf(n_file,"%s",word) != EOF)
	insert_data_base(head,word,n_f_name);

    return 1;
}

/*this function forms the data base form the backup file*/
int re_create_data_base(data_base_t **head,FILE *b_file)
{
    char *t = ":";
    char line[100],word[30],f_name[30];
    int i,j,f_count,w_count;

   if (*head == NULL)
	return 0;
    
    while (fscanf(b_file,"%s",line) != EOF)
    {
	strcpy(word,strtok(line,t));
	f_count = atoi(strtok(NULL,t));

	for (i = 0; i < f_count;i++)
	{
	    strcpy(f_name,strtok(NULL,t));
	    w_count = atoi(strtok(NULL,t));
	    for (j = 0;j < w_count; j++ )
	    {
		insert_data_base(head,word,f_name);
	    }
	}
    }
    return 1;
}

/*search funtion search perticular word in back up file and dispaly information*/
int search_data(data_base_t **head,FILE *fp1,char *word)
{
    data_base_t *ptr;
    file_i *temp1;
    word_i *temp2;
    
    if (*head == NULL)
	return 0;

    char ch = word[0];
    int status;

    re_create_data_base(head,fp1);
    
    ptr = *head;

    int num = index_conv(ch);

    /*searching index*/
    while (ptr -> index != num)
    {
	ptr = ptr -> d_next;
    }

    if(ptr -> data != NULL)
    {
	printf("==========================================================\n");
	temp1 = ptr -> data;
	temp2 = temp1 -> file_table;
        /*search for perticular word*/
	while (temp1)
	{
	    /*compare word with word name*/
	    if (!strcmp(temp1-> word,word))
	    {
		printf("word        : %s\n",temp1 -> word);
		printf("File count  : %d\n",temp1 -> file_count);

		printf("------------------------------\n");
		printf("%-15s  %-12s\n","File name","word count");
		printf("------------------------------\n");
		temp2 = temp1 -> file_table;
		while ( temp2)
		{
		    printf("%-15s  %6d\n",temp2 -> file_name,temp2 -> word_count);
		    temp2 = temp2 -> next;
		}
		printf("------------------------------\n\n");
	    }
	    temp1 = temp1 -> f_next;
	}
	printf("==========================================================\n\n");
    }
    return 1;

}

/*this function just savethe data into back up file*/
int save_data_base(data_base_t *head,FILE *fp1)
{
    data_base_t *ptr;
    file_i *temp1;
    word_i *temp2;

    ptr = head;

    if (ptr == NULL)
	return 0;

    while (ptr != NULL)
    {
	if(ptr -> data != NULL)
	{
	    temp1 = ptr -> data;
	    temp2 = temp1 -> file_table;
	    while (temp1)
	    {
		copy_to_file(fp1,(temp1 -> word),(temp1 -> file_count));
		temp2 = temp1 -> file_table;
		while ( temp2)
		{
		    copy_to_file(fp1,(temp2 -> file_name),(temp2 -> word_count));
		    temp2 = temp2 -> next;
		}
		temp1 = temp1 -> f_next;
		fputc('\n',fp1);
	    }
	}
	ptr = ptr -> d_next;
    }
    return 1;
}

/*convertion of charecter to int value*/
int index_conv(char ch)
{
    static int num;

    if ((ch >= 97) && (ch <= 122))
    {
	num = ch % 97;
    }
    else if ((ch >= 65) && (ch <= 90))
    {
	num = ch % 65;
    }
    else
    {
	printf("\nname should start from character !!\n");
	return -1;
    }
    return num;	
}

/*this function copy data in perticular pattrn*/
int copy_to_file(FILE *fp1,char *str,int c)
{
    char ch = c + 48;
    fputs(str,fp1);
    fputc(':',fp1);
    fputc(ch,fp1);
    fputc(':',fp1);
    return 0;
}
