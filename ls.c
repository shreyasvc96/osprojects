//LS


#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int t=0;

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;
int path_length;
  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
 path_length = strlen(p);
 if(t==1) 
{
  p[path_length] = '/';
  p[path_length+1] = '\0';
 }
  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
 memmove(buf, p, strlen(p));

  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));

  return buf;
}

void
ls(char *path,int a_bit)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
case T_FILE:
        t=st.type;
    char* fn = fmtname(buf);
      if(fn[0]!='.' || a_bit == 1 )
      {
          printf(1, "%s %d %d %d\n", fn , st.type, st.ino, st.size);
      }
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    //printf(1, "path %s \n", path);
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      t=st.type;
      char* fn = fmtname(buf);
      if(fn[0]!='.' || a_bit == 1)
      {
      printf(1, "%s %d %d %d\n", fn, st.type, st.ino, st.size);
      }
    }
    break;
  }
  close(fd);
}
// Function to chech whether a option is present or not
int hasAOption(int argc, char* argv[]) 
{
    int i;
    for(i=1; i<argc; i++)
    {
        if(strcmp(argv[i],"-a") == 0) 
        { 
            return 1;
            } 
        
    } return 0; } 
    // Separate function to get start position 
int getNext(int argc, char* argv[])
{ 
    if(hasAOption(argc, argv))
    { 
        return 2; 
        
    } 
    return 1; }
    
int
main(int argc, char *argv[])
{
  int i;
  int start;
  int a_bit=0;
  if(argc < 2){
    ls(".",a_bit);
    exit();
  }
 a_bit=hasAOption(argc,argv);
 if(a_bit==1 && argc==2)
 {
     ls(".",a_bit);
     exit();
 }
 if(a_bit==1)
 {
     start=getNext(argc,argv);
 }
  for(i=start ; i<argc; i++)
  {
    ls(argv[i],a_bit);
  }
  exit();
}

/*// Check if -a passed 
 a_bit = hasAOption(argc, argv); 
// Get start pos 
int start = getStartPos(argc, argv); 
// Loop through directories 
for(int i=start; i<argc; i++) 
{ 
    ls(argv[i]); 
    
} 
// Set flag and call ls on cwd 
a_bit = showHidden; 
if(showHidden) 
{
    ls(".");
    }*/


