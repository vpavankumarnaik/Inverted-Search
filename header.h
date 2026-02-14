#ifndef HEADER_H
#define HEADER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SUCCESS 0
#define FAILURE -1
#define LIST_EMPTY -2

typedef struct file_node			//structure for file name storing
{
	char name[50];
	struct file_node *link;
}fnode;

typedef struct sub_node				//structure for sub node
{
	int word_count;
	char file_name[50];
	struct sub_node *next_sub_node_link;
}snode;

typedef struct main_node			//structure for main node
{
	char word[50];
	int file_count;
	struct sub_node *sub_node_link;
	struct main_node *next_main_node_link;
}mnode;

//helper functions
int validate_files(char *argv[], int argc, fnode **fhead);
int validate_extn(char *fname);
int check_file_valid(char *fname);
int check_duplicate(fnode **fhead, char *fname);
int find_index(char *word);
int delete_fhead(fnode **fhead);
int delete_hashtable(mnode *hash_table[]);

//operation functions
int create_DB(fnode **fhead, mnode *hash_table[]);
int search_DB(mnode *hash_table[], char *word);
int display_DB(mnode *hash_table[]);
int save_DB(mnode *hash_table[], char *file_name);
int update_DB(mnode *hash_table[], char *file_name, fnode **fhead);

#endif
