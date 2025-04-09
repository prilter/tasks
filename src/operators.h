/* EVERY FLAG OPERATIONS */

/* LIBS */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* CONFIG AND PATH.H */
#include "../config.h"
#include "../mylibs/paths/paths.h"

/* EXTERN */
extern int edit(const char *, const char *, size_t); /* FROM e.c */
extern int replace(const char *, size_t, size_t);    /* FROM r.c */

/* MACROSES */
#define DIRLEN         100
#define BUFLEN         1024
#define RESLEN         1024
#define COMLEN	       1024
#define NONPATH_TRASH  " \tㅤ\tㅤㅤ"

#ifndef TASKSLEN
	#define TASKSLEN 100
#endif

#ifndef TASKLEN
	#define TASKLEN  100
#endif

#ifndef NAMELEN
	#define NAMELEN  100
#endif

#ifndef NOTASKS	
	#define NOTASKS	 "No tasks"
#endif

#ifndef EDITOR
	#define EDITOR "nano"
#endif

#ifndef NONPATH
	#define NONPATH  "Nothing"
#endif

#define make(fn)      (fclose(fopen(fn, "w")))
#define equal(s1, s2) (strcmp(s1, s2) == 0)

/* DEFAULT FLAGS */
/* PRINT ALL TASKS FROM CURRENT TASK LIST */
#define p()	{ 															   \
	FILE *r;                                     \
	char *buf, *fn;  													   \
	size_t id;                                   \
																						   \
	buf = malloc(BUFLEN);                        \
	fn  = malloc(DIRLEN);                        \
                                               \
	get_current_list_dir(fn);                    \
	if ((r = fopen(fn, "r"))) {                  \
		if (!(fgets(buf, BUFLEN, r)))              \
			puts(NOTASKS);													 \
		else {                                     \
			puts("Tasks:");                          \
			printf("1: %s", buf); /* 1 line */       \
			for (id = 2; fgets(buf, BUFLEN, r);)     \
				printf("%zd: %s", id++, buf);          \
		}							                             \
		fclose(r);                                 \
	} else puts("Choose the usable task list");  \
	free(buf);                                   \
}


/* PRINT ALL TASKS FROM CHOOSED TASK LIST */
#define s(fn) {                               \
	FILE *r;                                    \
	char *ldir, *buf;                           \
	size_t id;                                  \
                                              \
	ldir = malloc(DIRLEN);                      \
	buf  = malloc(BUFLEN);                      \
				                                      \
	get_work_dir(ldir);                         \
	strcat(ldir, fn);                           \
																						  \
	if ((r = fopen(ldir, "r"))) {               \
		if (!(fgets(buf,BUFLEN,r)))	              \
			puts(NOTASKS);													\
		else {                                    \
			puts("Tasks:");                         \
			printf("1: %s", buf); /* 1 line */      \
			for (id = 2; fgets(buf, BUFLEN, r);)    \
				printf("%zd: %s", id++, buf);         \
		}							                            \
		fclose(r);                                \
	} else puts("Invalid task list name");      \
                                              \
	free(ldir); free(buf);                      \
}


/* ADD NEW TASK */
#define n(newtask) {                          \
	FILE *r, *a;                                \
	char *listpath;                             \
                                              \
	listpath = malloc(DIRLEN);                  \
	get_current_list_dir(listpath);             \
                                              \
	if ((r = fopen(listpath, "r"))) {           \
		a = fopen(listpath, "a");                 \
		fprintf(a, "%s\n", newtask);              \
		fclose(r); fclose(a);                     \
	}                                           \
                                              \
	free(listpath);                             \
}


/* COMPLETE TASK */
#define c(id) {                  \
	char *listdir;                 \
                                 \
	listdir = malloc(DIRLEN);      \
	get_current_list_dir(listdir); \
                                 \
	edit(listdir, NULL, id);       \
	free(listdir);                 \
} 


/* REPLACE 2 TASKS */
#define r(l1, l2) {              \
	char *listdir;                 \
                                 \
	listdir = malloc(DIRLEN);      \
	get_current_list_dir(listdir); \
                                 \
	replace(listdir, l1, l2);      \
}


/* EDIT 1 LINE IN CURRENT TASK LIST */
#define e(l) {										                \
	FILE *r;                                        \
	char *b, *fn;																		\
                                                  \
	fn = malloc(DIRLEN);                            \
	get_current_list_dir(fn);                       \
	                                                \
	if ((r = fopen(fn, "r"))) {                     \
		fgets((b = malloc(BUFLEN)), BUFLEN, stdin);  	\
		edit(fn, b, l);													      \
		free(b); fclose(r); 													\
	} else puts("Choose the usable task list");     \
}


/* SEE ALL TASK LISTS */
#include <dirent.h> /* TO WORK WITH DIRECTORIES INFORMATION */
#define l() {                                   \
	/* VARIABLES */                               \
	DIR  *dir;                                    \
	char *listsdir;                               \
	struct dirent *ent;                           \
                                                \
	/* GET DIR */                                 \
	listsdir = malloc(DIRLEN);                    \
	get_work_dir(listsdir);                       \
                                                \
	/* OPEN THE DIR */                            \
	if ((dir = opendir(listsdir)) != NULL) {      \
		for (;(ent = readdir(dir));)                \
			if (strcmp(ent->d_name, ".usable") && *(ent->d_name) != '.')\
				{puts(ent->d_name);}                    \
	} else                                        \
		{puts("We lost your tasks lists. Sorry");}  \
                                                \
	closedir(dir);                                \
	free(listsdir);                               \
}


/* MAKE A TASK LIST */
#define b(list) {               \
	char *listdir;                \
	                              \
	if (*list != '.') {           \
		listdir = malloc(DIRLEN);   \
		get_list_dir(listdir, list);\
		                      			\
		make(listdir);              \
		free(listdir);              \
	} else                        \
		puts("Dot is illegal");     \
}


/* MOVE TO OTHER TASK LIST */
#include <unistd.h> /* TO CHECK EXTISTING OF FILE IN DIRECTORY */
#define m(list) {\
	FILE *w;                              \
	char *listdir, *usabledir;            \
                                        \
	listdir   = malloc(DIRLEN);           \
	usabledir = malloc(DIRLEN);           \
                                        \
	get_list_dir(listdir, list);          \
                                        \
	get_work_dir(usabledir);              \
	strcat(usabledir, ".usable");         \
                                        \
	if (access(listdir, F_OK) != -1) {    \
		w = fopen(usabledir, "w");          \
		fputs(list, w);                     \
		fclose(w);                          \
	} else                                \
		printf("%s doesn't exist. Use -M\n", list);\
                                        \
	free(listdir);                        \
	free(usabledir);                      \
}


/* KILL TASK LIST */
#define k(list) {                                    \
	char *listdir, *usinglist;                         \
                                                     \
	listdir   = malloc(DIRLEN);                        \
	usinglist = malloc(NAMELEN);                       \
                                                     \
	get_current_list_name(usinglist, NAMELEN);         \
	                                                   \
	get_list_dir(listdir, list);                       \
                                                     \
	if (!equal(usinglist, list))                       \
		remove(listdir);                                 \
	else                                               \
		puts("You can't. Change the tasks list");        \
                                                     \
	free(listdir);                                     \
	free(usinglist);                                   \
}


/* ALIAS TASK LIST */
#define a(oldn, newn) {                     \
	FILE *dot_usable_w;                       \
	char *curdir, *usablefile;                \
	char *oldpath, *newpath;                  \
                                            \
	curdir  = malloc(DIRLEN);                 \
	oldpath = malloc(DIRLEN);                 \
	newpath = malloc(DIRLEN);                 \
                                            \
	get_current_list_dir(curdir);             \
	get_list_dir(oldpath, oldn);              \
	get_list_dir(newpath, newn);              \
                                            \
	rename(oldpath, newpath);                 \
	if (equal(curdir, oldpath)) {             \
		usablefile = malloc(DIRLEN);            \
                                            \
		get_list_dir(usablefile, ".usable");    \
		dot_usable_w = fopen(usablefile, "w");  \
		fputs(newn, dot_usable_w);              \
                                            \
		free(usablefile); fclose(dot_usable_w); \
	}                                         \
                                            \
	free(curdir);                             \
	free(oldpath);                            \
	free(newpath);                            \
}


/* CHECK USABLE TASK LIST */
#define u()	{                             \
	char *res;                              \
                                          \
	res = malloc(NAMELEN);			            \
	get_current_list_name(res, NAMELEN);    \
                                          \
	puts(res);                              \
	free(res);                              \
}


/* QUIT FROM TASK LIST */
#define q() {                                    \
	FILE *edit;                                    \
	char *usablefile;                              \
                                                 \
	usablefile = malloc(DIRLEN);                   \
	get_list_dir(usablefile, ".usable");           \
	                                               \
	edit = fopen(usablefile, "w");                 \
	fprintf(edit, "%s%s", NONPATH, NONPATH_TRASH); \
                                                 \
	fclose(edit);                                  \
	free(usablefile);                              \
}


/* HELP MENU */
#define h()	{\
	puts("\
CTM - useful CLI Task Manager\n\
\n\
Usage: ctm [flag] [flag arguments]  for flags who need arguments\n\
   or: ctm [flag]                   for flags who doesn't need arguments\n\
\n\
Arguments can be normal and super. Super arguments is modificated normal argument. Usually this is a uppercased normal argument. For example, \"-C\" - super argument(modificated \"-c\"). In contrast to \"-c\", \"C\" complete all tasks in list\n\
\n\
Arguments:\n\
\tNormal:\n\
\t-p                                          print all tasks\n\
\t-s <fn>                                     print all tasks from argument list\n\
\t-n <new task text>                          make new task\n\
\t-c <id>                                     complite task as commit\n\
\t-r <id1> <id2>                              replace task 1 and task2\n\
\t-e <id>                                     edit task info by id\n\
\t-l                                          see all task lists\n\
\t-b <list name>                              make a new task list\n\
\t-m <list name>                              change task list\n\
\t-k <list name>                              remove the task list\n\
\t-a <list name 1> <list name 2>              rename a task list\n\
\t-u                                          print using task list name\n\
\t-q                                          quit from task list\n\
\t-h                                          help menu\n\
\n\
\tSuper:\n\
\t-P <start> <end(not necessary)>             print from a line to b line\n\
\t-S <list name> <start> <end(not necessary)> print a-b lines from argument list\n\
\t-N <number of task>                         make +n new tasks\n\
\t-C                                          complite all tasks\n\
\t-E <list name>                              edit with editor\n\
\t-R <number of replacements>                 n-times use r\n\
\t-M <list name>                              like -m, can make non-existent list\n\
\t-K <list name>                              like -k, can delete usable task list\
	");\
}

/* Super */
/* PRINT SOME TASKS FROM CURRENT TASK LIST */
#define P(l1, l2) {																	                                  \
	FILE *read;                                                                         \
	char *fn;                                                                           \
	char c[COMLEN];																				                              \
	char b[BUFLEN];	  																		                              \
                                                                                      \
	fn = malloc(DIRLEN);                                                                \
	get_current_list_dir(fn);                                                           \
                                                                                      \
	if ((read = fopen(fn, "r"))) {                                                      \
		if (!(fgets(b, BUFLEN, read))) /*NO TASKS*/																				\
			puts(NOTASKS);																			                            \
		else {																								                            \
			if (l1 <= l2)																				                            \
				sprintf(c, "nl -w 1 -s ': ' -n ln \"%s\" | sed -n '%zd,%zdp'", fn, l1, l2);		\
			else																								                            \
				sprintf(c,"nl -w 1 -s ': ' -n ln \"%s\" | sed -n '%zd,%zdp' | tac",fn,l2,l1);	\
																												  \
			system(c);																					\
		}																											\
		fclose(read);\
	} else puts("Choose task list");                        \
}


/* PRINT SOME TASKS FROM CHOOSED TASK LIST */
#define S(fn, l1, l2) {                                                                 \
	FILE *read;                                                                           \
	char *ldir, *com, *buf;                                                               \
                                                                                        \
	ldir = malloc(DIRLEN);                                                                \
	buf  = malloc(BUFLEN);                                                                \
	com  = malloc(COMLEN);                                                                \
                                                                                        \
	get_list_dir(ldir, fn);                                                               \
	if ((read = fopen(ldir, "r"))) {                                                      \
		if (!(fgets(buf, BUFLEN, read))) /*NO TASKS*/                                       \
			puts(NOTASKS);																			                              \
		else {																								                              \
			if (l1 <= l2)																				                              \
				sprintf(com, "nl -w 1 -s ': ' -n ln \"%s\" | sed -n '%zd,%zdp'", ldir, l1, l2);	\
			else																								                              \
				sprintf(com,"nl -w 1 -s ': ' -n ln \"%s\" | sed -n '%zd,%zdp' | tac",ldir,l2,l1);\
																												                                \
			system(com);  																			                              \
		}																											                              \
		fclose(read);                                                                       \
	} else puts("Choose task list");                                                      \
                                                                                        \
	free(ldir); free(buf); free(com);                                                     \
}


/* MAKE N-TIMES TASKS IN CURRENT TASK LIST*/
#define N(len_mac) {          			     \
	FILE *tasks, *read;                    \
	char b[BUFLEN];									       \
	char *filename;                        \
	size_t len;												     \
																		     \
	filename = malloc(DIRLEN);             \
	get_current_list_dir(filename);        \
                                         \
	if (len_mac > 0) {                     \
		if ((read = fopen(filename, "r"))) { \
			tasks = fopen(filename, "a");      \
			for (len=(size_t)len_mac;len--;) { \
				fgets(b, TASKLEN, stdin);				 \
				fputs(b, tasks);								 \
			}																	 \
			fclose(tasks); fclose(read);       \
		} else                               \
			puts("Choose a task list");        \
	}                                      \
}


/* COMPLETE ALL TASKS FROM CURRENT TASK LIST */
#define C() {	        					        \
	FILE *r;                              \
	char *fn;                             \
                                        \
	fn = malloc(DIRLEN);                  \
	get_current_list_dir(fn);             \
				                                \
	if ((r = fopen(fn, "r"))) {           \
		remove(fn);		      					      \
		make(fn);                   	      \
		fclose(r);                          \
	} else                                \
		puts("Choose an usable task list"); \
	                                      \
	free(fn);                             \
}


/* EDIT TASK LIST WITH EDITOR(DEFAULT IS NANO) */
#define E(list) {                            \
	FILE *r;                                   \
	char *listdir, *com;                       \
                                             \
	com     = malloc(COMLEN);                  \
	listdir = malloc(DIRLEN);                  \
                                             \
	if (list) /* CHOOSED -> USE SOME LIST */   \
		get_list_dir(listdir, list);             \
	else /* DIDN'T CHOOSED -> CURRENT */       \
		get_current_list_dir(listdir);           \
	                                           \
	if ((r = fopen(listdir, "r"))) {           \
		sprintf(com,"%s \"%s\"",EDITOR,listdir); \
		system(com);                             \
		fclose(r);                               \
	} else puts("Choose a task list");         \
                                             \
	free(com); free(listdir);                  \
}


/* REPLACE SOME TASKS N-TIMES FROM CURRENT TASK LIST */
#define R(times) {    									\
	size_t l1, l2, i;											\
	char *fn;                             \
                                        \
	fn = malloc(DIRLEN);                  \
	get_current_list_dir(fn);             \
                                        \
	for (i = 1; i <= times; i++) {				\
		scanf("%zu %zu", &l1, &l2);					\
		replace(fn, l1, l2);								\
	}																			\
                                        \
	free(fn);                             \
}																				\


/* MOVE TO OTHER TASK LIST. IF IT DOESN'T EXIST TASKS WOULD MADE IT */
#define M(list) {                       \
	FILE *w;                              \
	char *listdir, *usabledir;            \
                                        \
	listdir   = malloc(DIRLEN);           \
	usabledir = malloc(DIRLEN);           \
                                        \
	get_list_dir(listdir, list);          \
	get_list_dir(usabledir, ".usable");   \
                                        \
	if (access(listdir, F_OK) == -1)      \
		{make(listdir);}                    \
	w = fopen(usabledir, "w");						\
	fputs(list, w);												\
                                        \
	fclose(w);                            \
	free(listdir);                        \
	free(usabledir);                      \
}


/* KILL TASK LIST WITHOUT A RESTRICTIONS */
#define K(tokill) {                                  \
	FILE *usablef;                                     \
	char *listdir, *usinglist, *usabledir;             \
	char *usepath;                                     \
                                                     \
	listdir   = malloc(DIRLEN);                        \
	usabledir = malloc(DIRLEN);                        \
	usinglist = malloc(NAMELEN);                       \
	usepath   = malloc(DIRLEN);                        \
                                                     \
	get_list_dir(usabledir, ".usable");                \
	get_current_list_name(usinglist, NAMELEN);         \
	get_list_dir(listdir, tokill);                     \
	get_current_list_dir(usepath);                     \
                                                     \
	if (equal(usinglist,tokill)) {                     \
		puts("Complete! Change your task list");         \
		usablef = fopen(usabledir, "w");                 \
		fprintf(usablef, "%s%s",NONPATH,NONPATH_TRASH);  \
		fclose(usablef);                                 \
	}                                                  \
	remove(listdir);                                   \
	                                                   \
	free(listdir);                                     \
	free(usinglist);                                   \
	free(usabledir);                                   \
	free(usepath);                                     \
}
