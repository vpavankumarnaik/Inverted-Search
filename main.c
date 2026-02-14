#if 0
Name        : V Pavan Kumar Naik
ID          : 25021_097
description : Inverted Search Project
#endif

#include"header.h"
//main function
int main(int argc, char *argv[])
{
	fnode *fhead = NULL;
	if(validate_files(argv, argc, &fhead) == FAILURE)			//validate all files
	{
		printf("ERROR: No valid files are uploaded\n");
		printf("./a.out file1.txt file2.txt ....\n");
		return 0;
	}
	printf("INFO: Validation Successfully Completed\n");
	
	mnode *hash_table[28] = {NULL};						//create a hash table for storing index based main node address
	int choice, flag_c = 0, flag_u = 0;
	char name[50];
	printf("1.Create DB\n2.Search DB\n3.Display DB\n4.Save DB\n5.Update DB\n6.Exit\n");

	while(1)
	{
		printf("\nEnter Choice: ");
		scanf("%d", &choice);

		switch(choice)							//in the updating hashtable we can only do update and create
		{
			case 1: if(flag_c == 0)					//for create DB purforms only one time
				{
					if(create_DB(&fhead, hash_table) == FAILURE)
					{
						printf("ERROR: Creating DB Failed\n");
					}
					else
					{
						printf("INFO: Data Base Created Successfully.\n");
						flag_c = 1;
					}
				}
				else
				{
					printf("ERROR: Not Possible\n");
				}
				break;

			case 2: char word[50];					//for search a word
				printf("Enter the Word: ");
				scanf("%s", word);
				
				if(search_DB(hash_table, word) == FAILURE)
				{
					printf("INFO: Word Not Found\n");
				}
				break;

			case 3: if(display_DB(hash_table) == LIST_EMPTY)	//for display all the information
				{
					printf("INFO: List Empty\n");
				}
				break;

			case 4: char file_name[50];				//for save the data into a .txt file
				do
				{
					printf("Enter File Name : ");
					scanf("%s", file_name);
				}while(validate_extn(file_name) == FAILURE);
					
				if(save_DB(hash_table, file_name) == FAILURE)
				{
					printf("ERROR: Save Data Base is Failed.\n");
				}
				else
				{
					printf("INFO: Data successfully Saved in %s file\n", file_name);
				}
				break;

			case 5: if(flag_u == 0 && flag_c == 0)			//for update data base from the saved file
				{
					printf("Enter file name : ");
					scanf("%s", name);
					if(validate_extn(name) == FAILURE)
					{
						printf("ERROR: File Name Extension Not Valid\n");
						delete_fhead(&fhead);
						delete_hashtable(hash_table);
						return FAILURE;
					}
					if(update_DB(hash_table, name, &fhead) == FAILURE)
					{
						printf("ERROR: Updating Data Base Failed\n");
					}
					else
					{
						printf("INFO: Data Base Updated Successfully\n");
						flag_u = 1;
					}
				}
				else
				{
					printf("ERROR: Not Possible\n");
				}
				break;

			case 6: delete_fhead(&fhead);
				delete_hashtable(hash_table);
				return SUCCESS;					//exit from program
			
			default : printf("INFO: Invalid Choice!!\n");
				 break; 
		}
	}

	return SUCCESS;
}

//validate all files
int validate_files(char *argv[], int argc, fnode **fhead)
{
	fnode *prev = NULL;
	int valid_file_count = 0;						//for valid file count

	for(int i=1; i<argc; i++)						//run a loop to validate and store the data into file SLL
	{
		if(validate_extn(argv[i]) == SUCCESS)				//validate extension
		{
			if(check_file_valid(argv[i]) == SUCCESS)		//validate the file if it is present and any data into it or not
			{
				if(check_duplicate(fhead, argv[i]) == SUCCESS)	//check if the file is duplicate or not
				{
					//if all true then insert it last
					fnode *new = malloc(sizeof(fnode));
					if(!new)
					{
						return FAILURE;
					}

					strcpy(new->name, argv[i]);
					new->link = NULL;
				
					if(*fhead == NULL)
					{
						*fhead = new;
					}
					else
					{
						prev->link = new;
					}
					prev = new;
					valid_file_count++;
				}
				else
				{
					printf("ERROR: %s duplicate file\n", argv[i]);
				}
			}
			else
			{
				printf("ERROR: %s file validation failed\n", argv[i]);
			}
		}
		else
		{
			printf("ERROR: %s file extension failed\n", argv[i]);
		}
	}

	if(valid_file_count == 0)						//if count is not 0 then we can purform the operations
	{
		return FAILURE;
	}

	return SUCCESS;
}

//function for validate extension
int validate_extn(char *fname)
{
	char *ext = strchr(fname, '.');
	if(ext == NULL)
		return FAILURE;

	if(strcmp(ext, ".txt") == 0)
		return SUCCESS;
	else
		return FAILURE;
}

//function for file is valid or not
int check_file_valid(char *fname)
{
	FILE *fp = fopen(fname, "r");
	if(!fp)
	{
		return FAILURE;
	}

	fseek(fp, 0, SEEK_END);
	if(ftell(fp) == 0)
	{
		fclose(fp);
		return FAILURE;
	}
	
	fclose(fp);
	return SUCCESS;
}

//function for duplicate validations
int check_duplicate(fnode **fhead, char *fname)
{
	fnode *temp = *fhead;
	while(temp != NULL)
	{
		if(strcmp(temp->name, fname) == 0)
			return FAILURE;
		temp = temp->link;
	}
	return SUCCESS;
}

//function for finding index
int find_index(char *word)
{
	if(word[0] >= 'A' && word[0] <= 'Z')
		return (word[0] - 'A');
	else if(word[0] >= 'a' && word[0] <= 'z')
                return (word[0] - 'a');
	else if(word[0] >= '0' && word[0] <= '9')
		return 26;
	else
		return 27;
}

//function for free the file names SLL
int delete_fhead(fnode **fhead)
{
	while(*fhead != NULL)
	{
		fnode *ftemp = *fhead;
		*fhead = ftemp->link;
		free(ftemp);
	}
	return SUCCESS;
}

//function for free the hash table
int delete_hashtable(mnode *hash_table[])
{
	for(int i=0; i <= 27; i++)					//run the loop 28 times
	{
		while(hash_table[i] != NULL)				//loop run untill hash table index is NULL
		{
			mnode *mtemp = hash_table[i];
			while(mtemp->sub_node_link != NULL)		//loop run untill main node, sub node link part NULL
			{
				snode *stemp = mtemp->sub_node_link;
				mtemp->sub_node_link = stemp->next_sub_node_link;
				free(stemp);				//update the link and free the sub node
			}
			hash_table[i] = mtemp->next_main_node_link;	//update the link and free the main node
			free(mtemp);
		}
		hash_table[i] = NULL;
	}
	return SUCCESS;
}
