#include"header.h"

//function for create the data base
int create_DB(fnode **fhead, mnode *hash_table[])
{
	fnode *ftemp = *fhead;
	while(ftemp != NULL)						//traverse upto last file
	{
		FILE *fp = fopen(ftemp->name, "r");			//open the file in read mode
		if(fp == NULL)
		{
			return FAILURE;
		}

		char word[50];
		while(fscanf(fp, "%s", word) != EOF)			//read the word one by one upto last word in the file
		{
			int index = find_index(word);			//find the index of the word

			if(hash_table[index] == NULL)			//insert word into hash table particular index
			{
				mnode *mnew = malloc(sizeof(mnode));
				snode *snew = malloc(sizeof(snode));
				if(!mnew || !snew)
				{
					return FAILURE;
				}

				strcpy(mnew->word, word);		//update the values into main node
				mnew->file_count = 1;
				mnew->next_main_node_link = NULL;

				strcpy(snew->file_name, ftemp->name);	//update the values into subnode
				snew->word_count = 1;
				snew->next_sub_node_link = NULL;

				mnew->sub_node_link = snew;
				hash_table[index] = mnew;		//if it's first node update mainnode address into hash table index
			}
			else						//if first index is not NULL
			{
				mnode *mtemp = hash_table[index];
				mnode *mprev = NULL;

				while(mtemp != NULL)					//traverse upto word found
				{
					if(strcmp(mtemp->word, word) == 0)		//if word found  
					{
						snode *stemp = mtemp->sub_node_link;
						snode *sprev = NULL;
						
						while(stemp != NULL)			//check if the file name is same or not
						{
							if(strcmp(stemp->file_name, ftemp->name) == 0)
							{
								(stemp->word_count)++;	//if filename same just increment the word count
								break;
							}
							sprev = stemp;
							stemp = stemp->next_sub_node_link;
						
						}
						if(stemp == NULL)		//if file name not match create a new subnode and update the values
						{
							snode *snew2 = malloc(sizeof(snode));
							if(!snew2)
							{
								return FAILURE;
							}
		
							strcpy(snew2->file_name, ftemp->name);
							snew2->word_count = 1;
							snew2->next_sub_node_link = NULL;

							(mtemp->file_count)++;
							sprev->next_sub_node_link = snew2;
						}
						break;
					}
					mprev = mtemp;
					mtemp = mtemp->next_main_node_link;
				}
				if(mtemp == NULL)				//if word not found create a new main node and sub node update the values
				{
					mnode *mnew = malloc(sizeof(mnode));
					snode *snew = malloc(sizeof(snode));
					if(!mnew || !snew)
					{
						return FAILURE;
					}
				
					strcpy(mnew->word, word);
					mnew->file_count = 1;
					mnew->next_main_node_link = NULL;

					strcpy(snew->file_name, ftemp->name);
					snew->word_count = 1;
					snew->next_sub_node_link = NULL;

					mnew->sub_node_link = snew;
					mprev->next_main_node_link = mnew;	//if it's not first main node update address into prev main node address
				}
			}
		}
		fclose(fp);
		ftemp = ftemp->link;						//traverse files
	}
	return SUCCESS;
}

//function for search a word into data base
int search_DB(mnode *hash_table[], char *word)
{
	int index = find_index(word);					//find the index of word

	if(hash_table[index] == NULL)					//if hash table index is NULL there is no words are available into it
	{	
		return FAILURE;
	}
	
	mnode *mtemp = hash_table[index];
	while(mtemp != NULL)						//traverse main node upto last 
	{
		if(strcmp(mtemp->word, word) == 0)			//if word is present print datails of word 
		{
			printf("------------------------------------------------------------------\n");
			printf("|%s\t| %-11s| %-11s| %-16s| %-10s |\n", "Index", "Word", "File Count", "File Name", "Word Count");
			printf("------------------------------------------------------------------\n");
	
			printf("|%4d\t| %-11s|\t    %-4d  ", index, mtemp->word, mtemp->file_count);
			snode *stemp = mtemp->sub_node_link;
			int flag = 1;

			while(stemp != NULL)				//loop for subnodes into main node
			{
				if(flag == 0)
				{
					printf("|%4s\t|%12s|\t   %-4s   ", " ", " ", " ");
				}
				flag = 0;
				printf("| %-16s|\t  %-7d|\n", stemp->file_name, stemp->word_count);
				stemp = stemp->next_sub_node_link;
			}
			printf("------------------------------------------------------------------\n");
			return SUCCESS;
		}
		mtemp = mtemp->next_main_node_link;
	}
	return FAILURE;

}

//function for display the hash table information
int display_DB(mnode *hash_table[])
{
	int flag = 1;
	for(int i=0; i <= 27; i++)					//run the loop for 28 times 
	{
		if(hash_table[i] == NULL)				//if hash table index is empty skip iteration
		{
			continue;
		}
		if(flag == 1)
		{
			printf("------------------------------------------------------------------\n");
			printf("|%s\t| %-11s| %-11s| %-16s| %-10s |\n", "Index", "Word", "File Count", "File Name", "Word Count");
			printf("------------------------------------------------------------------\n");
			flag = 0;
		}

		mnode *mtemp = hash_table[i];
		while(mtemp != NULL)					//run all main node into the particular index and print the data
		{
			printf("|%4d\t| %-11s|\t    %-4d  ", i, mtemp->word, mtemp->file_count);
			snode *stemp = mtemp->sub_node_link;
			int f_count = 1;
			while(stemp != NULL)				//loop for sub node data printing
			{
				if(f_count == 0)
				{
					printf("|%4s\t|%12s|\t   %-4s   ", " ", " ", " ");
				}
				
				f_count = 0;
				printf("| %-16s|\t  %-7d|\n", stemp->file_name, stemp->word_count);
				stemp = stemp->next_sub_node_link;
			}
			printf("------------------------------------------------------------------\n");
			mtemp = mtemp->next_main_node_link;
		}
	}
	if(flag == 0)							//check if list is empty or not
	{
		return SUCCESS;
	}
	else
	{
		return LIST_EMPTY;
	}
}

//function for save data into file
int save_DB(mnode *hash_table[], char *file_name)
{
	FILE *fp = fopen(file_name, "w");				//open the file into write mode
	if(!fp)
	{
		return FAILURE;
	}
	
	for(int i=0; i <= 27; i++)					//run upto 28 times
	{
		if(hash_table[i] == NULL)				//if hash table index is null skip the iteration
		{
			continue;
		}

		mnode *mtemp = hash_table[i];
		while(mtemp != NULL)					//upload the data into file
		{
			fprintf(fp, "#%d;%s;%d;", i, mtemp->word, mtemp->file_count);
			snode *stemp = mtemp->sub_node_link;
			while(stemp != NULL)
			{
				fprintf(fp, "%s;%d;", stemp->file_name, stemp->word_count);
				stemp = stemp->next_sub_node_link;
			}
			fprintf(fp, "#\n");
			mtemp = mtemp->next_main_node_link;
		}
	}
	fclose(fp);
	return SUCCESS;
}

//function for upload the saved file data into hash table
int update_DB(mnode *hash_table[], char *file_name, fnode **fhead)
{
	FILE *fp = fopen(file_name, "r");				//open the file into read mode
	if(!fp)
	{
		return FAILURE;
	}

	char check_char;
	if((check_char = fgetc(fp)) != '#')				//if the first char of file is not # then it's not a valid file
	{
		return FAILURE;
	}
	rewind(fp);							//goto again first index of file
	
	int index;
	char word[50];
	int file_count;

	while(fscanf(fp, "#%d;%[^;];%d;", &index, word, &file_count) == 3)	//check the current data is EOF or not
	{
		mnode *mnew = malloc(sizeof(mnode));			//update the data into main node
		if(!mnew)
		{
			return FAILURE;
		}

		strcpy(mnew->word, word);
		mnew->file_count = file_count;
		mnew->next_main_node_link = NULL;
		mnew->sub_node_link = NULL;

		int i=0;
		while(i < file_count)					//run loop for file count times
		{
			int w_count;
			char f_name[50];
			snode *snew = malloc(sizeof(snode));
			if(!snew)
			{
				return FAILURE;
			}

			fscanf(fp, "%[^;];%d;", f_name, &w_count);
			
			fnode *ftemp = *fhead;
			fnode *fprev = NULL;
			while(ftemp != NULL)				//if the file name is also present is the SLL file then delete the SLL file name 
			{
				if(strcmp(ftemp->name, f_name) == 0)	//if the file name also matching delete the file name
				{
					if(*fhead == ftemp)
					{
						*fhead = ftemp->link;
					}
					else
					{
						fprev->link = ftemp->link;
					}
					free(ftemp);
					break;
				}
				fprev = ftemp;
				ftemp = ftemp->link;
			}
			
			strcpy(snew->file_name, f_name);		//update the sub node data
			snew->word_count = w_count;
			snew->next_sub_node_link = NULL;

			if(mnew->sub_node_link == NULL)
			{
				mnew->sub_node_link = snew;
			}
			else
			{
				snode *stemp = mnew->sub_node_link;
				while(stemp->next_sub_node_link != NULL)
				{
					stemp = stemp->next_sub_node_link;
				}
				stemp->next_sub_node_link = snew;
			}
			i++;
		}
		fscanf(fp, "#\n");					//skip the line

		if(hash_table[index] == NULL)				//update the hash table index link with new main node address
		{
			hash_table[index] = mnew;
		}
		else
		{
			mnode *mtemp = hash_table[index];
			while(mtemp->next_main_node_link != NULL)
			{
				mtemp = mtemp->next_main_node_link;
			}
			mtemp->next_main_node_link = mnew;
		}

	}
	fclose(fp);
	return SUCCESS;
}
