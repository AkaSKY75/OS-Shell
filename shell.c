 
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/wait.h>
#include<errno.h>
#include<readline/readline.h>
#include<readline/history.h>
 
#define clear() printf("\033[H\033[J")
 
#define MAXCOM 1000
#define MAXLIST 100
char parsed[1024][1024];
char dirnameparsed[1024][1024];
 
 
int takeInput(char* str)
{
    char* buf;
 
    buf = readline("\n> ");
    if (strlen(buf) != 0) {
        add_history(buf);
        strcpy(str, buf);
        return 0;
    } else {
        return 1;
    }
}
 
// Function to print Current Directory.
void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\n %s", cwd);
}
 
 
 
 
void dirname()
{
  char cwd[1024];
  char path[1024];
  getcwd(cwd, sizeof(cwd));
  strcpy(path, parsed[1]);
  int i = 0;
  char * token = strtok(path, "/");
  while( token != NULL )
  {
   strcpy(dirnameparsed[i], token);
   token = strtok(NULL, "/");
   i++;
  }
  if(i == 1)
    printf("\n.");
  else {
  	for(int j = 0; j < i-1; j++)
  	{
  		printf("/");
  		printf("%s", dirnameparsed[j]);
  	}
  	}
}
int move(int nr_params, char params[][1024])
{
  int i, fd_r = -2, j, file_name_index, destination_status = 1, destination_name_index, fd_w;
  char aux[1000];
  unsigned char c;
  struct stat st;
  for(i = 1; i < nr_params; i++)
  {
    if(strlen(params[i]) == 2 && params[i][0] == '-') // verifica daca o fost dat un parametru (-i, -t, -S (-b))
    {
      if(params[i][1] == 'i')
      {
        /*Cod pentru parametru -i*/
      }
      else if(params[i][1] == 't')
      {
        /*Cod pentru parametru -t*/
      }
      else if(params[i][1] == 'S')
      {
        /*Cod pentru parametru -S*/
      }
    }
    else
    {
      if(fd_r == -2)
      {
          file_name_index = i;
          fd_r = open(params[i], O_RDONLY);
          /*if(fd_r == -1)
          {
            printf("mv: cannot move '%s' to '%s': No such file or directory", params[i]);
            return 1;
          }*/
      }
      else
      {
        destination_name_index = i;
        destination_status = stat(params[i], &st);
        if(destination_status == -1)
        {
          j = strlen(params[i])-1;
          while(params[i][j] != '/')
            j--;
            strncpy(aux, params[i], j);
            aux[j] = '\0';
          destination_status = stat(aux, &st);
        }
      }
 
    }
  }
 
  if(fd_r == -2 && destination_status == 1)
  {
    printf("mv: missing file operand\nTry 'mv --help' for more information.\n");
    return 1; // some error code, usually needed even if not used
  }
  else if(fd_r != -2 && destination_status == 1)
  {
    printf("mv: missing destination file operand after '%s'\nTry 'mv --help' for more information.\n", params[file_name_index]);
    return 2; // other error code
  }
  else if(fd_r == -1)
  {
    j = strlen(params[file_name_index])-1;
    while(params[file_name_index][j] != '/')
      j--;
    printf("mv: cannot stat '%s': No such file or directory\n", params[file_name_index]+j+1);
    return 3; // other error code
  }
  else if(destination_status == -1)
  {
    j = strlen(params[file_name_index])-1;
    while(params[file_name_index][j] != '/')
      j--;
    printf("mv: cannot move '%s' to '%s': No such file or directory\n", params[file_name_index]+j+1, params[destination_name_index]);
  }
  else
  {
    fstat(fd_r, &st);
    fd_w = open(params[destination_name_index], O_CREAT | O_WRONLY | O_TRUNC, st.st_mode);
    while(read(fd_r, &c, 1) == 1)
    {
      write(fd_w, &c, 1);
    }
    close(fd_r);
    close(fd_w);
    remove(params[file_name_index]);
    return 0; // return 0 on success
  }
 
}
int copy1(int nr_params, char params[][1024])
{
  int i, fd_r = -2, j, file_name_index, destination_status = 1, destination_name_index, fd_w;
  char aux[1000];
  unsigned char c;
  struct stat st;
  for(i = 1; i < nr_params; i++)
  {
      if(fd_r == -2)
      {
          file_name_index = i;
          fd_r = open(params[i], O_RDONLY);
          /*if(fd_r == -1)
          {
            printf("mv: cannot move '%s' to '%s': No such file or directory", params[i]);
            return 1;
          }*/
      }
      else
      {
        destination_name_index = i;
        destination_status = stat(params[i], &st);
        if(destination_status == -1)
        {
          j = strlen(params[i])-1;
          while(params[i][j] != '/')
            j--;
            strncpy(aux, params[i], j);
            aux[j] = '\0';
          destination_status = stat(aux, &st);
        }
      }
 
 
  }
 
 
    fstat(fd_r, &st);
    fd_w = open(params[destination_name_index], O_CREAT | O_WRONLY | O_TRUNC, st.st_mode);
    while(read(fd_r, &c, 1) == 1)
    {
      write(fd_w, &c, 1);
    }
    close(fd_r);
    close(fd_w);
    return 0; // return 0 on success
    }
int copy(int nr_params, char params[][1024])
{
  int i, fd_r = -2, j, file_name_index, destination_status = 1, destination_name_index, fd_w;
  char aux[1000];
  char aux2[1000];
  unsigned char c;
  struct stat st;
  for(i = 1; i < nr_params; i++)
  {
    if(strlen(params[i]) == 2 && params[i][0] == '-') // verifica daca o fost dat un parametru (-i, -t, -S (-b))
    {
      if(params[i][1] == 'i')
      {
        int ok = 1;
        char* buf=readline("Wanna overwrite?: ");
        if(strcmp(buf, "y") == 0)
        {
           copy1(nr_params, params);
        }
        else
            break;  
        /*Cod pentru parametru -i*/
      }
      else if(params[i][1] == 'r' || params[i][1] == 'R')
      {
        /*Cod pentru parametru -r*/
      }
      else if(params[i][1] == 'R')
      {
        /*Cod pentru parametru -R*/
      }
      else if(params[i][1] == 't')
      {
        strcpy(aux, params[2]);
        strcpy(params[2], params[3]);
        strcpy(params[3], aux);
        /*Cod pentru parametru -t*/
      }
      else if(params[i][1] == 'v')
      {
        printf("\'%s\'", params[2]);
        printf(" - > ");
        printf("\'%s\'", params[3]);
        /*Cod pentru parametru -v*/
      }
    }
    else
    {
      if(fd_r == -2)
      {
          file_name_index = i;
          fd_r = open(params[i], O_RDONLY);
          /*if(fd_r == -1)
          {
            printf("mv: cannot move '%s' to '%s': No such file or directory", params[i]);
            return 1;
          }*/
      }
      else
      {
        destination_name_index = i;
        destination_status = stat(params[i], &st);
        if(destination_status == -1)
        {
          j = strlen(params[i])-1;
          while(params[i][j] != '/')
            j--;
            strncpy(aux, params[i], j);
            aux[j] = '\0';
          destination_status = stat(aux, &st);
        }
      }
 
    }
  }
 
  if(fd_r == -2 && destination_status == 1)
  {
    //printf("cp: missing file operand\nTry 'mv --help' for more information.\n");
    return 1; // some error code, usually needed even if not used
  }
  else if(fd_r != -2 && destination_status == 1)
  {
    printf("cp: missing destination file operand after '%s'\nTry 'mv --help' for more information.\n", params[file_name_index]);
    return 2; // other error code
  }
  else if(fd_r == -1)
  {
    j = strlen(params[file_name_index])-1;
    while(params[file_name_index][j] != '/')
      j--;
    printf("cp: cannot stat '%s': No such file or directory\n", params[file_name_index]+j+1);
    return 3; // other error code
  }
  else if(destination_status == -1)
  {
    j = strlen(params[file_name_index])-1;
    while(params[file_name_index][j] != '/')
      j--;
    printf("cp: cannot move '%s' to '%s': No such file or directory\n", params[file_name_index]+j+1, params[destination_name_index]);
  }
  else
  {
    fstat(fd_r, &st);
    fd_w = open(params[destination_name_index], O_CREAT | O_WRONLY | O_TRUNC, st.st_mode);
    while(read(fd_r, &c, 1) == 1)
    {
      write(fd_w, &c, 1);
    }
    //close(fd_r);
    //close(fd_w);
    //return 0; // return 0 on success
  }
 
}
 
/* E mai ok daca returnezi si numarul de parametri aici */
int Parser(char * inputString)
{
 int i = 0;
 char * token = strtok(inputString, " ");
 while( token != NULL )
 {
 strcpy(parsed[i], token);
 token = strtok(NULL, " ");
 i++;
}
 return i;
}
 
int main()
{
    char inputString[MAXCOM];
    int nr_params;
    clear();
    while (1) {
        // print shell line
        printDir();
        // take input
        if (takeInput(inputString))
            continue;
        nr_params = Parser(inputString);
        if(strcmp(parsed[0], "dirname") == 0)
           dirname();
        else if(strcmp(parsed[0], "mv") == 0)
           {
             move(nr_params, parsed);
           }
         else if(strcmp(parsed[0], "cp") == 0)
            {
              copy(nr_params, parsed);
            }
        else if(strcmp(parsed[0], "exit") == 0)
           {
           clear();
           break;
           }
        else if (strcmp(parsed[0], "help") == 0)
                {
                  printf(" cp\n");
                  printf(" mv\n");
                  printf(" dirname\n");
                  printf(" help\n");
                  printf(" clear\n");
                  printf(" exit\n");
                }
        else if (strcmp(parsed[0], "clear") == 0)
                {
                 clear();
                }
        else
           {
           printf("\nUnrecognized command\n");
           }
 
    }
 
    return 0;
}
 
