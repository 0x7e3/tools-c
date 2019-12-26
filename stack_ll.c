/* NAME: stack_ll - check for balanced brackets {} in files.
 * 
 * SYNOPSIS: stack_ll [FILE] 
 * 
 * DESCRIPTION: Check for balanced brackets {} in file and print 
 * on the standart output warning with number of line where an error is.
 * 
 * Program to implement a STACK using a LINKED LIST.
 * 
 * RETURN VALUE: If brackets are balanced return 1, else return 0.
 *  
 * COPYRIGHT: MIT Style License
 * 
 * */

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>


/* define stack
 * */

struct item {
	char data;
	int line;
	struct item *link;
};

typedef struct item item_p;

item_p *push(item_p **stack_point, char symbol, int line)
{
	item_p *tmp;
	tmp = malloc(sizeof(item_p));
	tmp->data = symbol;
	tmp->line = line;
	tmp->link = *stack_point;
	*stack_point = tmp;
	return *stack_point;
}

char pop(item_p **stack_point)
{
	char ch;
	if (*stack_point == NULL) { return 1; }
	item_p *tmp = *stack_point;
	ch = (*stack_point)->data;
	*stack_point = (*stack_point)->link;
	free(tmp);
	return ch;
}


/* linked list for numbers of lines, where is an errors
 * */

struct lines {
	int line;
	struct lines *link;
};

void add_lines(struct lines **err_lines, int line)
{
	
	struct lines *tmp;
	tmp = malloc(sizeof(struct lines));
	tmp->line = line;
	tmp->link = *err_lines;
	*err_lines = tmp;
}

int display_error(struct lines **err_lines)
{
	if (*err_lines == NULL){ 
		printf("File is correct: brackets are balanced!\n");
		return 0;
	} else {
		int i;
		struct lines *tmp;
		tmp = malloc(sizeof(struct lines));
		for(i = 0; *err_lines; i++){
			printf("Error on line: %d\n", (*err_lines)->line);
			tmp = *err_lines;
			*err_lines = (*err_lines)->link;
			free(tmp);
		}
		printf("Number of errors: %d\n", i);
		return 1;
	}
}


/* main algorithm 
 * */

enum {
	open_bracket  = '{', 
	close_bracket = '}'
};

void parse(FILE *fd, item_p **stack_point, struct lines **err_lines)
{
	int c, line;
	char ch;
	line = 1;
	
	/* parse the file and create stack */
	
	while ((c = fgetc(fd)) != EOF){
		ch = (char) c;
		
		if (ch == open_bracket){
			push(stack_point, ch, line);
		} else
		if ( (ch == close_bracket) && ((pop(stack_point)) == 1) ){
			add_lines(err_lines, line);
		} else
		if (ch == '\n'){ line++; }
	}
	
	/* end checking and free stack */

	item_p *tmp;
	tmp = malloc(sizeof(item_p));
	while(*stack_point){
		add_lines(err_lines, (*stack_point)->line);
		tmp = *stack_point;
		*stack_point = (*stack_point)->link;
		free(tmp);
	}
}

FILE *file_open (int argc, char **argv)
{
	if (argc < 2){
		perror("Warning: missing argument (file operand)");
		exit(1);
	}
	
	FILE *fd;
	fd = fopen(argv[1], "r");
	if (!fd){
		perror(argv[1]);
		exit(1);
	}
	printf("Open file: %s\n", argv[1]);
	
	return fd;
}

/* MAIN PROGRAMM
 * */

int main(int argc, char **argv)
{
	item_p *stack = NULL;
	struct lines *err_lines = NULL;
	FILE *fd;
	
	fd = file_open(argc, argv);
	parse(fd, &stack, &err_lines);
	fclose(fd);
	
	return display_error(&err_lines);
}
