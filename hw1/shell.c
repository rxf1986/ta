#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <pwd.h>

#define FALSE 0
#define TRUE 1
#define INPUT_STRING_SIZE 80
#define MAX_PATH_SIZE 200

#include "io.h"
#include "parse.h"
#include "process.h"
#include "shell.h"


char buffer[MAX_PATH_SIZE];

int cmd_quit(tok_t arg[]) {
  printf("Bye\n");
  exit(0);
  return 1;
}

int cmd_help(tok_t arg[]);
int cmd_cd(tok_t arg[]);
int cmd_ls(tok_t arg[]);
/* Command Lookup table */
typedef int cmd_fun_t (tok_t args[]); /* cmd functions take token array and return int */
typedef struct fun_desc {
  cmd_fun_t *fun;
  char *cmd;
  char *doc;
} fun_desc_t;

fun_desc_t cmd_table[] = {
  {cmd_help, "?", "show this help menu"},
  {cmd_quit, "quit", "quit the command shell"},
  {cmd_cd, "cd", "change to a specific dir"},
  {cmd_ls, "ls", "list the dirent"}
};

int cmd_help(tok_t arg[]) {
  int i;
  for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
    printf("%s - %s\n",cmd_table[i].cmd, cmd_table[i].doc);
  }
  return 1;
}

int cmd_cd(tok_t arg[]) {

  if(chdir(arg[0]) == 0) {
  //int fd = open(arg[0], O_RDONLY);
  //fchdir(fd);
    getcwd(buffer, MAX_PATH_SIZE);
    printf("the current dir is %s\n", buffer);
  }
  else {
    printf("change dir failedi,please check the input path\n");
    return 0;
  }
  return 1;
}
 
int cmd_ls(tok_t arg[]) {
  DIR * pdir;
  struct dirent *dirent;
  char d_name[20];

  if(arg[0] == NULL) {
    d_name[0]='.';
    d_name[1]='\0';
    pdir = opendir(d_name);
  }
  else {
    pdir = opendir(arg[0]);
  }

  if(pdir == NULL) {
    printf("error opendir %s\n", arg[0]);
    return -1;
  }

  while((dirent = readdir(pdir)) != NULL) {
    printf(" %s\n", dirent->d_name);
  }
   closedir(pdir);   
   return 1;
}
int lookup(char cmd[]) {
  int i;
  for (i=0; i < (sizeof(cmd_table)/sizeof(fun_desc_t)); i++) {
    if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0)) return i;
  }
  return -1;
}

void init_shell()
{
  /* Check if we are running interactively */
  shell_terminal = STDIN_FILENO;

  /** Note that we cannot take control of the terminal if the shell
      is not interactive */
  shell_is_interactive = isatty(shell_terminal);

  if(shell_is_interactive){

    /* force into foreground */
    while(tcgetpgrp (shell_terminal) != (shell_pgid = getpgrp()))
      kill( - shell_pgid, SIGTTIN);

    shell_pgid = getpid();
    /* Put shell in its own process group */
    if(setpgid(shell_pgid, shell_pgid) < 0){
      perror("Couldn't put the shell in its own process group");
      exit(1);
    }

    /* Take control of the terminal */
    tcsetpgrp(shell_terminal, shell_pgid);
    tcgetattr(shell_terminal, &shell_tmodes);
  }
  /** YOUR CODE HERE */
}

/**
 * Add a process to our process list
 */
void add_process(process* p)
{
  /** YOUR CODE HERE */
}

/**
 * Creates a process given the inputString from stdin
 */
process* create_process(char* inputString)
{
  /** YOUR CODE HERE */
  return NULL;
}



int shell (int argc, char *argv[]) {
  char *s = malloc(INPUT_STRING_SIZE+1);			/* user input string */
  tok_t *t;			/* tokens parsed from input */
  int lineNum = 0;
  int fundex = -1;
  pid_t pid = getpid();		/* get current processes PID */
  pid_t ppid = getppid();	/* get parents PID */
  pid_t cpid, tcpid, cpgid;
  char env[MAX_PATH_SIZE], *pathenv;

  init_shell();

  printf("%s running as PID %d under %d\n",argv[0],pid,ppid);

  lineNum=0;
  if(getcwd(buffer, MAX_PATH_SIZE) < 0) {
    printf("error get cwd \n");
    return -1;
  }
  fprintf(stdout, "%d:%s ", lineNum, buffer);

  while ((s = freadln(stdin))){
    t = getToks(s); /* break the line into tokens */
    fundex = lookup(t[0]); /* Is first token a shell literal */
    if(fundex >= 0) {
      printf("enter if\n");
     cmd_table[fundex].fun(&t[1]);
    }
    else {
      printf("enter else\n");
      cpid = fork();
      if(cpid == 0) {
         memset(env,0,sizeof(env));
         pathenv = getenv("PATH");
         strcat(env, pathenv);
         //strcat(env, getpwuid(getuid())->pw_dir);
         strcat(env, ":/pintos/code/ta/hw1");
         setenv("PATH",env,1);

         if(execv(t[0], t) == -1) {
            printf("error exec %s\n", t[0]);
            printf(stdout, "This shell only supports built-ins. Replace this to run programs as commands.\n");
            exit(1);
         }
         
         //exit(0);
      }
      else if(cpid > 0) {
         waitpid(cpid, NULL, 0);
      }
      else printf("fork error\n");
    }
    fprintf(stdout, "%d:%s ", lineNum, buffer);
 
    }
  return 0;
}
