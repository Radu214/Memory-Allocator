#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alloclist.h"

void InputSep(char *input, char args[40][600], int *nr_args)
{
	*nr_args = 0;
	char *p;
	int i = 0;
	p = strtok(input, " ");
	while (p)
	{
		strcpy(args[i], p);
		p = strtok(NULL, " ");
		(*nr_args)++;
		i++;
	}
}

void Command(char args[40][600], int *cmd, int nr_args)
{
	if (strcmp(args[0], "INIT_HEAP") == 0)
		*cmd = 1;
	if (strcmp(args[0], "MALLOC") == 0)
		*cmd = 2;
	if (strcmp(args[0], "FREE") == 0)
		*cmd = 3;
	if (strcmp(args[0], "READ") == 0)
		*cmd = 4;
	if (strcmp(args[0], "WRITE") == 0)
	{
		*cmd = 5;
		if (args[2][strlen(args[2]) - 1] != '\"') //Pentru propozitie din mai multe cuvinte
		{
			int len = 0;
			for(int i = 0; i < nr_args; i++) 
				len = len + strlen(args[i]);
			char *sentance = calloc(len + nr_args + 1, sizeof(char));
			int i;
			for (i = 2; i < nr_args; i++)
			{
				strcat(sentance, args[i]);
				if (strstr(args[i], "\"") != NULL && i != 2)
				{
					char *s = malloc(sizeof(char) * strlen(sentance) + 1);
					strncpy(s, sentance, strlen(sentance) + 1);
					strcpy(sentance, s + 1);
					sentance[strlen(sentance) - 1] = '\0';
					free(s);
					break;
				}
				strcat(sentance, " ");
			}
			strcpy(args[2], sentance);
			strcpy(args[3], args[++i]);
			free(sentance);
		}
		else
		{
			char *s = malloc(sizeof(char) * strlen(args[2]));
			strcpy(s, args[2] + 1);
			strcpy(args[2], s);
			args[2][strlen(args[2]) - 1] = '\0';
			free(s);
		}
	}
	if (strcmp(args[0], "DUMP_MEMORY") == 0)
		*cmd = 6;
	if (strcmp(args[0], "DESTROY_HEAP") == 0)
		*cmd = 7;
}

int Convert(const char *hexa)
{
	int len = strlen(hexa);
	int number = 0;
	int base = 1;

	for (int i = len - 1; i >= 0; i--)
	{
		if (hexa[i] >= '0' && hexa[i] <= '9')
		{
			number += (hexa[i] - '0') * base;
		}
		else if (hexa[i] >= 'a' && hexa[i] <= 'f')
		{
			number += (hexa[i] - 'a' + 10) * base;
		}
		else if (hexa[i] >= 'A' && hexa[i] <= 'F')
		{
			number += (hexa[i] - 'A' + 10) * base;
		}

		base *= 16;
	}

	return number;
}

void Dump(int total_mem, int alloc_mem, int avail_memory, int free_blocks, int malloc_times, int frag_times,
		  int free_times, int list_number, doubly_linked_list_t **free_vect, alloc_doubly_linked_list_t *alloc_vect)
{
	dll_node_t *temp;
	printf("+++++DUMP+++++\n");
	printf("Total memory: %d bytes\n", total_mem);
	printf("Total allocated memory: %d bytes\n", alloc_mem);
	printf("Total free memory: %d bytes\n", avail_memory);
	printf("Free blocks: %d\n", free_blocks);
	printf("Number of allocated blocks: %u\n", alloc_vect->size);
	printf("Number of malloc calls: %d\n", malloc_times);
	printf("Number of fragmentations: %d\n", frag_times);
	printf("Number of free calls: %d\n", free_times);
	for (int i = 0; i < list_number; i++)
	{
		if (dll_get_size(free_vect[i]) == 0)
			continue;
		printf("Blocks with %d bytes - %u free block(s) :", free_vect[i]->data_size, dll_get_size(free_vect[i]));
		temp = free_vect[i]->head;
		while (temp != NULL)
		{
			printf(" 0x%x", temp->adress);
			temp = temp->next;
		}
		printf("\n");
	}
	printf("Allocated blocks :");
	temp = alloc_vect->head;
	while (temp != NULL)
	{
		printf(" (0x%x - %d)", temp->adress, temp->data_size);
		temp = temp->next;
	}
	printf("\n");
	printf("-----DUMP-----\n");
}

void Free(int list_number, doubly_linked_list_t **free_vect, alloc_doubly_linked_list_t *alloc_vect, char *input)
{
	for (int i = 0; i < list_number; i++)
	{
		dll_free(&free_vect[i]);
	}
	free(free_vect);
	alloc_dll_free(&alloc_vect);
}

void HandleInput(char *input, char args[40][600], int nr_args, int *cmd)
{
	input = malloc(sizeof(char) * 600);
	fgets(input, 600, stdin);
	if (input[strlen(input) - 1] == '\n')
		input[strlen(input) - 1] = '\0';
	InputSep(input, args, &nr_args);
	Command(args, cmd, nr_args);
	free(input);
}

int main(void)
{
	char *input, args[40][600];
	int nr_args, cmd, start_heap, list_number, block_size, tip_rec;
	int malloc_times = 0, free_times = 0, avail_memory, frag_times = 0, free_blocks = 0, alloc_mem = 0, total_mem;
	doubly_linked_list_t **free_vect, **temp_vect;
	alloc_doubly_linked_list_t *alloc_vect;
	while (1)
	{
		HandleInput(input, args, nr_args, &cmd);
		switch (cmd)
		{
		case 1: //Init Heap
		{
			start_heap = Convert(args[1]);
			list_number = atoi(args[2]);
			block_size = atoi(args[3]);
			tip_rec = atoi(args[4]);
			avail_memory = list_number * block_size;
			free_vect = malloc(sizeof(doubly_linked_list_t *) * list_number);

			alloc_vect = alloc_dll_create();

			int data_size = 8, current_adress = start_heap;

			for (int i = 0; i < list_number; i++)
			{
				free_vect[i] = dll_create(data_size);
				for (int j = 0; j < block_size / data_size; j++)
				{
					dll_create_nth_empty_node(free_vect[i], j, current_adress, data_size);
					current_adress += data_size;
				}
				free_blocks = free_blocks + (block_size / data_size);
				data_size = data_size * 2;
			}
			total_mem = list_number * block_size;
			break;
		}
		case 2: //Malloc
		{
			unsigned int bytes;
			int found_node = 0;
			int j = 0;
			bytes = atoi(args[1]);
			for (int i = 0; i < list_number; i++)
			{
				if (free_vect[i]->data_size == bytes)
				{
					if (free_vect[i]->head == NULL)
					{
						found_node = 0;
						continue;
					}
					free_blocks--;
					dll_node_t *temp;
					temp = dll_remove_nth_node(free_vect[i], 0);
					dll_node_t *temp2;
					temp2 = alloc_vect->head;
					while (temp2 != NULL && temp2->adress < temp->adress)
					{
						temp2 = temp2->next;
						j++;
					}
					alloc_dll_add_nth_node(alloc_vect, j,
										   temp);
					found_node = 1;
					malloc_times++;
					alloc_mem += bytes;

					break;
				}
				if (free_vect[i]->data_size > bytes)
				{
					if (free_vect[i]->head == NULL)
					{
						found_node = 0;
						continue;
					}
					found_node = 1;
					dll_node_t *temp, *new_node;
					new_node = malloc(sizeof(dll_node_t));
					temp = dll_remove_nth_node(free_vect[i], 0);
					memcpy(new_node, temp, sizeof(dll_node_t));
					temp->data_size = bytes;

					dll_node_t *temp2;
					temp2 = alloc_vect->head;
					while (temp2 != NULL && temp2->adress < temp->adress)
					{
						temp2 = temp2->next;
						j++;
					}
					alloc_dll_add_nth_node(alloc_vect, j,
										   temp);
					new_node->adress = new_node->adress + bytes;
					new_node->data_size = new_node->data_size - bytes;
					for (int j = 0; j < list_number; j++)
					{
						if (free_vect[j]->data_size > new_node->data_size)
						{
							temp_vect = realloc(free_vect, sizeof(doubly_linked_list_t *) * (list_number + 1));
							if (temp_vect == NULL)
							{
								printf("Ups!");
							}
							else
							{
								free_vect = temp_vect;
							}
							list_number++;
							for (int k = list_number - 2; k >= j; k--)
							{
								free_vect[k + 1] = free_vect[k];
							}
							free_vect[j] = dll_create(new_node->data_size);
							dll_add_nth_empty_node(free_vect[j], 0, new_node);
							alloc_mem += bytes;
							break;
						}
						if (free_vect[j]->data_size == new_node->data_size)
						{
							int k = 0;
							temp2 = free_vect[j]->head;
							while (temp2 != NULL && temp2->adress < new_node->adress)
							{
								temp2 = temp2->next;
								k++;
							}
							dll_add_nth_empty_node(free_vect[j], k, new_node);
							alloc_mem += bytes;
							break;
						}
					}
					frag_times++;
					malloc_times++;

					break;
				}
			}
			if (found_node == 0)
			{
				printf("Out of memory\n");
			}
			else
			{
				avail_memory = avail_memory - bytes;
			}

			break;
		}
case 3: // Free
{
    int adress = Convert(args[1]);
    int i;
    i = 0;
    if (tip_rec == 0)
    {
        dll_node_t *temp;
        temp = alloc_vect->head;
        while (temp != NULL && temp->adress != adress)
        {
            temp = temp->next;
            i++;
        }
        if (temp == NULL)
        {
            printf("Invalid free\n");
            break;
        }
        else
        {
            free_blocks++;
            free_times++;
        }
        avail_memory += temp->data_size;
        temp = alloc_dll_remove_nth_node(alloc_vect, i);
        free(temp->data);
        alloc_mem -= temp->data_size;
        doubly_linked_list_t *templist;
        
        while (i < list_number && free_vect[i]->data_size < temp->data_size)
        {
            i++;
        }

        if (i < list_number && free_vect[i]->data_size == temp->data_size)
        {
            templist = free_vect[i];
            dll_node_t *temp2 = templist->head;
            while (temp2 != NULL && temp->adress > temp2->adress)
            {
                temp2 = temp2->next;
                i++;
            }
            dll_add_nth_empty_node(templist, i, temp);
        }
        else
        {
            doubly_linked_list_t **temp_vect = realloc(free_vect, sizeof(doubly_linked_list_t *) * (list_number + 1));
            if (temp_vect == NULL)
            {
                printf("Ups!");
            }
            else
            {
                free_vect = temp_vect;
            }
            list_number++;
            if (i == 0)
                i = 1;
            for (int j = list_number - 2; j >= i - 1; j--)
            {
                free_vect[j + 1] = free_vect[j];
            }
            free_vect[i - 1] = dll_create(temp->data_size);
            dll_add_nth_empty_node(free_vect[i - 1], 0, temp);
        }
    }
        break;
    }

		case 4: //Read
		{
			int adress = Convert(args[1]);
			int nr_bytes = atoi(args[2]);
			dll_node_t *temp;
			temp = alloc_vect->head;
			while (temp != NULL && temp->adress != adress)
			{
				if (adress > temp->adress && adress < (temp->adress + temp->data_size))
					break;
				temp = temp->next;
			}
			if (temp == NULL)
			{
				printf("Segmentation fault (core dumped)\n");
				Dump(total_mem, alloc_mem, avail_memory, free_blocks, malloc_times, frag_times, free_times, list_number,
					 free_vect, alloc_vect);
				Free(list_number, free_vect, alloc_vect, input);
				exit(0);
				break;
			}
			int cpysize = nr_bytes;
			dll_node_t *cpytemp = temp;
			int first = 0;
			while (cpysize > 0)
			{
				if (first == 0)
				{
					cpysize = cpysize - cpytemp->data_size + (adress - cpytemp->adress);
					first = 1;
				}
				else
					cpysize = cpysize - cpytemp->data_size;
				if (cpytemp->next != NULL)
					if ((cpytemp->adress + cpytemp->data_size != (cpytemp->next)->adress && cpysize > 0))
					{
						printf("Segmentation fault (core dumped)\n");
						Dump(total_mem, alloc_mem, avail_memory, free_blocks, malloc_times, frag_times, free_times,
							 list_number, free_vect, alloc_vect);
						Free(list_number, free_vect, alloc_vect, input);
						exit(0);
						break;
					}
				cpytemp = cpytemp->next;
				if (cpytemp == NULL && cpysize > 0)
				{
					printf("Segmentation fault (core dumped)\n");
					Dump(total_mem, alloc_mem, avail_memory, free_blocks, malloc_times, frag_times, free_times,
						 list_number, free_vect, alloc_vect);
					Free(list_number, free_vect, alloc_vect, input);
					exit(0);
					break;
				}
			}
			first = 0;
			while (nr_bytes > 0)
			{
				unsigned char *byte;
				if (first == 0)
				{
					byte = (unsigned char *)(temp->data + (adress - temp->adress));
					first = 1;
				}
				else
				{
					byte = (unsigned char *)(temp->data);
				}

				for (int i = 0; i < temp->data_size && nr_bytes > 0; i++)
				{
					printf("%c", byte[i]);
					nr_bytes--;
				}
				temp = temp->next;
			}
			printf("\n");

			break;
		}
		case 5://Write
		{
			int adress = Convert(args[1]);
			int nr_bytes = atoi(args[3]);
			int mal = 1;
			char *data;
			char *s;
			if ((int)strlen(args[2]) < nr_bytes)
				nr_bytes = strlen(args[2]);
			data = malloc(sizeof(char) * nr_bytes + 1);
			strncpy(data, args[2], nr_bytes);
			data[nr_bytes] = '\0';
			if ((int)strlen(data) < nr_bytes)
				nr_bytes = strlen(data);
			dll_node_t *temp;
			temp = alloc_vect->head;
			while (temp != NULL && temp->adress != adress)
			{
				if (adress > temp->adress && adress < (temp->adress + temp->data_size))
					break;
				temp = temp->next;
			}
			if (temp == NULL)
			{
				printf("Segmentation fault (core dumped)\n");
				free(data);
				Dump(total_mem, alloc_mem, avail_memory, free_blocks, malloc_times, frag_times, free_times, list_number,
					 free_vect, alloc_vect);
				Free(list_number, free_vect, alloc_vect, input);
				exit(0);
				break;
			}
			int cpysize = nr_bytes;
			dll_node_t *cpytemp = temp;
			int first = 0;
			while (cpysize > 0)
			{
				if (first == 0)
				{
					cpysize = cpysize - cpytemp->data_size + (adress - cpytemp->adress);
					first = 1;
				}
				else
					cpysize = cpysize - cpytemp->data_size;
				if (cpytemp->next != NULL)
					if ((cpytemp->adress + cpytemp->data_size != (cpytemp->next)->adress && cpysize > 0))
					{
						printf("Segmentation fault (core dumped)\n");
						free(data);
						Dump(total_mem, alloc_mem, avail_memory, free_blocks, malloc_times, frag_times, free_times,
							 list_number, free_vect, alloc_vect);
						Free(list_number, free_vect, alloc_vect, input);
						exit(0);
						break;
					}
				cpytemp = cpytemp->next;
				if ((cpytemp == NULL && cpysize > 0))
				{
					printf("Segmentation fault (core dumped)\n");
					free(data);
					Dump(total_mem, alloc_mem, avail_memory, free_blocks, malloc_times, frag_times, free_times,
						 list_number, free_vect, alloc_vect);
					Free(list_number, free_vect, alloc_vect, input);
					exit(0);
					break;
				}
			}
				first = 0;
				while (nr_bytes > 0)
				{
					int memsize;
					if (first == 0)
					{
						if (nr_bytes > temp->data_size - (adress - temp->adress))
							memsize = temp->data_size - (adress - temp->adress);
						else
							memsize = nr_bytes;
						memcpy(temp->data + (adress - temp->adress), data, memsize);
						first = 1;
					}
					else
					{
						if (nr_bytes > temp->data_size)
							memsize = temp->data_size;
						else
							memsize = nr_bytes;
						memcpy(temp->data, data, memsize);
					}
					nr_bytes = nr_bytes - memsize;
					if (nr_bytes > 0)
					{
						s = malloc(sizeof(char) * nr_bytes + 1);
						strncpy(s, data + memsize, nr_bytes);
						s[nr_bytes] = '\0';
						strcpy(data, s);
						mal = 0;
						free(s);
					}

					temp = temp->next;
				}
			free(data);
			break;
		}
		case 6://Dump
		{
			Dump(total_mem, alloc_mem, avail_memory, free_blocks, malloc_times, frag_times, free_times, list_number,
				 free_vect, alloc_vect);
			break;
		}
		case 7://Destroy
		{
			Free(list_number, free_vect, alloc_vect, input);
			exit(0);
		}
		default:
		{
			printf("Invalid command\n");
		}
		}
	}
}
