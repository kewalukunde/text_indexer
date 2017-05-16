#include <stdio.h>
#include <string.h>
#include "../include/text_indexer.h"
#include <unistd.h>

int main(int argc,char *argv[])
{
    FILE *fp1,*fp2;
    char buff[20],word[30],b_file[30];
    int choice,i = 1,z = 1,status = 0,b_file_state = 0;
    
    data_base_t *head = NULL;
    file_i *ptr;
    
    /*create data base basic list a-z*/
    create_data_base(&head);

    printf("\n\t\t\t\t%-25s\n","TEAXT INDEXER");
    printf("\n%-25s\t%-25s\t%-25s\n","1. create data base","2. Search data","3. Display data base");
    printf("%-25s\t%-25s\t%-25s\n","4. Update data base","5. Save data base","6. Exit");

    do
    {
	printf("\nEnter a choice : ");
	scanf("%d",&choice);
	/*switch case for opration*/
	switch(choice)
	{
	    /*create data base for passed file*/
	    case 1: printf("\n# create data base \n");

		    if(argc > 1)
		    {
			/*conditions for options*/
			while (argv[i] != NULL)
			{
			    if ((!strcmp(argv[i],"-o")) || (!strcmp(argv[i],"-c")))
				status++;
			    else if(status == 2)
				break;
			    i++;
			}
			if (status != 2)
			{
			    printf("NOT enouph option passed !!\n");
			    return 0;
			}
			i = 1;

			/*file open loop*/
			while (argv[i] != NULL)
			{
			    if (!strcmp(argv[i],"-o"))
			    {
				strcpy(b_file,argv[i+1]);
				break;
			    }
			    else if (strcmp(argv[i],"-c"))
			    {    
				if ((fp1 = fopen(argv[i],"r")) == NULL)
				{	
				    printf("File can not be open!!\n");
				    return 0;
				}
				while (fscanf(fp1,"%s",buff) != EOF)
				    insert_data_base(&head,buff,argv[i]);
				fclose(fp1);
			    }
			    i++;
			} 
		    }
		    /*if no command line argument passsed*/
		    else if ((argc < 2) && (b_file_state != 1))
		    {
		        b_file_state = 1;
			printf("Enter new file name :");
			scanf("%s",word);
			printf("Enter backup file name :");
			scanf("%s",b_file);
			if ((fp1 = fopen(word,"r")) == NULL)
			{	
			    printf("File can not be open!!\n");
			    return 0;
			}
			while (fscanf(fp1,"%s",buff) != EOF)
			    insert_data_base(&head,buff,word);
			fclose(fp1);

		    }
		    __fpurge(stdin);
		    break;
		
	    case 2:/*data search opration for perticular word from back up file*/
		    printf("\n# Search data \n");

		    if ((argc < 2) && (b_file_state != 1))
		    {
			b_file_state = 1;
			printf("Enter backup file name :");
			scanf("%s",b_file);
		    }
		   
		    if ((fp1 = fopen(b_file,"r")) == NULL)
		    {	
			printf("File can not be open!!\n");
			return 0;
		    }
		    printf("\nEnter word for search: ");
		    scanf("%s",word);

		    status = search_data(&head,fp1,word);

		    if (!status)
			printf("\n# NO data base found !!\n");
			
		    fclose(fp1);

		    __fpurge(stdin);
		    break;

	    case 3:/*display all the data base saved in back up*/
		    printf("\n# Display data base\n");

		    if ((argc < 2) && (b_file_state != 1))
		    {
			b_file_state = 1;
			printf("Enter backup file name :");
			scanf("%s",b_file);
		    }

		    if ((fp1 = fopen(b_file,"r")) == NULL)
		    {	
			printf("File can not be open!!\n");
			return 0;
		    }

		    status = display_data_base(&head,fp1);

		    if (!status)
			printf("\n# NO data base found !!\n");

		    fclose(fp1);
		    __fpurge(stdin);
		    break;
#if 1
	    case 4:/*update data base by new file*/
		    printf("\n# Update data base \n");

		    printf("Enter new file name :");
		    scanf("%s",word);

		    if ((argc < 2) && (b_file_state != 1))
		    {
			b_file_state = 1;
			printf("Enter backup file name :");
			scanf("%s",b_file);
		    }

		    if ((fp1 = fopen(word,"r")) == NULL)
		    {	
			printf("File can not be open!!\n");
			return 0;
		    }

		    if ((fp2 = fopen(b_file,"r+")) == NULL)
		    {	
			printf("File can not be open!!\n");
			return 0;
		    }

		    status = update_data_base(&head,fp1,fp2,word);

		    if (status)
			printf("\n# Data base updated.\n");
		    else
			printf("\n# NO data base found !!\n");

		    fclose(fp1);
		    fclose(fp2);
		    __fpurge(stdin);
		    break;
#endif
	    case 5:/*save data base after creating data base*/
		    printf("\n# save data base\n");

		    if ((fp1 = fopen(b_file,"w")) == NULL)
		    {	
			printf("File can not be open!!\n");
			printf("pass output file!!\n");
			return 0;
		    }

		    status = save_data_base(head,fp1);
		    if(status == 1)
			printf("File successfully saved!!\n");
		    else
			printf("File not saved, may be no data found!!\n");

		    fclose(fp1);
		    __fpurge(stdin);
		    break;

	    case 6: z = 0;
		    break;

	    default:/*for wrong choice*/
		    printf("\nYou entered wrong choice \n");
		    break;
	}
	__fpurge(stdin);
    }while(z == 1);

    return 0;
}
