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
//#include<readline/readline.h>
//#include<readline/history.h>

#define clear() printf("\033[H\033[J")

#define MAXCOM 1000
#define MAXLIST 100
char parsed[1024][1024];
char dirnameparsed[1024][1024];

/* STERGE ASTA*/
void add_history(char* buf)
{

}

int takeInput(char* str)
{
    char buf[1000];
    printf("\n> ");
    gets(buf);
    //buf = readline("\n> ");
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
        else
        {
          j = strlen(params[file_name_index])-1;
          while(j > 0 && params[file_name_index][j] != '/')
            j--;
          strcpy(aux, params[i]);
          aux[strlen(params[i])] = '/';
          aux[strlen(params[i])+1] = '\0';
          strcat(aux, params[file_name_index]+j);
          strcpy(params[i], aux);
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

void loop(char* source, char* destination)
{
  DIR *dir;
  int fd_r = -1, fd_w = -1;
  unsigned char c;
  struct dirent *entry;
  struct stat st;
  dir = opendir(source);
  char new_source[1000], new_destination[1000];
  while ((entry = readdir(dir)) != NULL) {
    //printf("%s\n", entry->d_name);
    if(entry->d_name[0] != '.' && entry->d_name[1] != '.')
    {
      strcpy(new_source, source);
      new_source[strlen(new_source)+1] = '\0';
      new_source[strlen(new_source)] = '/';
      strcat(new_source, entry->d_name);

      strcpy(new_destination, destination);
      new_destination[strlen(new_destination)+1] = '\0';
      new_destination[strlen(new_destination)] = '/';
      strcat(new_destination, entry->d_name);

      stat(new_source, &st);
      if((st.st_mode & S_IFMT) == S_IFDIR)
      {
        mkdir(new_destination, st.st_mode);
        loop(new_source, new_destination);
      }
      else
      {
        fd_r = open(new_source, O_RDONLY);
        fd_w = open(new_destination, O_CREAT | O_WRONLY | O_TRUNC, st.st_mode);
        while(read(fd_r, &c, 1) == 1)
        {
          write(fd_w, &c, 1);
        }
        close(fd_w);
        close(fd_r);
      }
    }
  }
  closedir(dir);
}

int copy(int nr_params, char params[][1024])
{
  int i, fd_r = -2, j, file_name_index, destination_status = 1, destination_name_index, fd_w, interactive = 0, recursive = 0, verbose = 0, target = 0;
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
        /*This should ask before overwriting files*/
        interactive = 1;
      }
      else if(params[i][1] == 'r' || params[i][1] == 'R')
      {
        /*Cod pentru parametru -r -R*/
        /*This should copy all the files and sub-folders of a specified directory*/
        recursive = 1;
      }
      else if(params[i][1] == 't')
      {
        /*Cod pentru parametru -t*/
        /*Not sure what this does, maybe from multiple source files/folders, copy to a single destination (many-to-one)*/
        target = 1;
      }
      else if(params[i][1] == 'v')
      {
        /*Cod pentru parametru -v*/
        verbose = 1;
      }
    }
    else
    {
      if(fd_r == -2)
      {
          file_name_index = i;
          fd_r = stat(params[i], &st);
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
    printf("cp: missing file operand\nTry 'mv --help' for more information.\n");
    return 1; // some error code, usually needed even if not used
  }
  else if(fd_r != -2 && destination_status == 1)
  {
    printf("cp: missing destination file operand after '%s'\nTry 'mv --help' for more information.\n", params[file_name_index]);
    return 2; // other error code
  }
  else if(destination_status == -1)
  {
    j = strlen(params[file_name_index])-1;
    while(params[file_name_index][j] != '/')
      j--;
    printf("cp: cannot move '%s' to '%s': No such file or directory\n", params[file_name_index]+j+1, params[destination_name_index]);
  }
  else if(fd_r == -1)
  {
    j = strlen(params[file_name_index])-1;
    while(j >= 0 && params[file_name_index][j] != '/')
      j--;
    if((st.st_mode & S_IFMT) == S_IFDIR)
    {
      printf("cp: cannot stat '%s': No such file or directory\n", params[file_name_index]+j+1);
      return 3; // other error code
    }
  }
  else
  {
    if((recursive == 1) && ((st.st_mode & S_IFMT) == S_IFDIR))
    {
      stat(params[file_name_index], &st); // could have used 2 struct stat variables, too lazy
      if((st.st_mode & S_IFMT) == S_IFDIR)
      {
        loop(params[file_name_index], params[destination_name_index]);
      }
      else
        goto regular_copy; // also, goto is something wow, it should just jump your code to regular_copy tag
    }
    else
    {
      regular_copy:
        stat(params[file_name_index], &st);
        if((st.st_mode & S_IFMT) == S_IFDIR)
        {
          printf("cp: -r not specified; omitting directory '%s'\n", params[file_name_index]);
          return 4; // other error code
        }
        else
          fd_r = open(params[file_name_index], O_RDONLY);
        i = strlen(params[destination_name_index]);
        params[destination_name_index][i] = '/';
        params[destination_name_index][i+1] = '\0';
        strcat(params[destination_name_index], params[file_name_index]+j+1);
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
