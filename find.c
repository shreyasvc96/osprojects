#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

int inumvalue = 0;

void substring(const char *inputString, char *outputString){
	int i;
	for(i=1; inputString[i]!='\0'; i++){
		outputString[i-1] = inputString[i];
	}

	outputString[i-1]= '\0';
}

void
concat(char *dest, const char *src){
	while(*dest){
		dest++;
	}

	while((*dest++ = *src++)){
	}
}


char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  //Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  //Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}



void
ls(char *path, char *file, int flag)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
   // printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
   // printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }


  switch(st.type){
  case T_FILE:
    //printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      //printf(1, "ls: path too long\n");
      break;
    }


    strcpy(buf, path);
    p = buf+strlen(buf);

    *p++ = '/';


    while(read(fd, &de, sizeof(de)) == sizeof(de)){

      if(de.inum == 0)
        continue;

      memmove(p, de.name, DIRSIZ);

      p[DIRSIZ] = 0;

      if(stat(buf, &st) < 0){
        //printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }

      if(strcmp(de.name,file)==0){
		//If we are loooking for Directory
		if((st.type==1) && (flag==1)){
			printf(1,"%s/%s\n",path,file);
		}

		//If we are looking for File
		if((st.type==2) && (flag==2)){
			printf(1,"%s/%s\n",path,file);
		}

		//For the  Printi Command
		if(flag==4){
			printf(1,"%d %s/%s\n",st.ino,path,file);
		}

		//For inum command Exact Values
		if(flag==6){
			if(st.ino==inumvalue){
			printf(1,"%d %s/%s\n",st.ino,path,file);
			}
		}

		//for inum command greater value
		if(flag==7){
			if(st.ino>inumvalue){
				printf(1,"%d %s/%s\n",st.ino,path,file);
			}
		}

		//for inum command smaller values
		if(flag==8){
			if(st.ino<inumvalue){
				printf(1,"%d %s/%s\n",st.ino,path,file);
			}
		}
	}

	// Following condition avoids folder with . and ..
	if(strcmp(de.name,".") == 0 || strcmp(de.name,"..") == 0){
		continue;
	}

	//If it is a folder we will enter it.
	if(st.type==1){
		ls(buf,file,flag);
	}
    }
    break;
  }
  close(fd);
}


int
main(int argc, char *argv[])
{

  char filename[100];
  char path[100];


  strcpy(path, argv[1]);


	if(strcmp("-name", argv[2])==0){
		strcpy(filename,argv[3]);
		//if No Flag is passed then we are looking for File
		//2 for File
		//1 for Directory
		//4 Printi Command
		//6 exact inum
		//7 greater inum
		//8 lesser inum
		if(argc==4){
                	ls(path, filename, 2);
		}else{
			if(argc>4){
				if(strcmp(argv[4],"-type")==0){
					if(strcmp(argv[5],"f")==0){
						ls(path,filename,2);
					}else if(strcmp(argv[5],"d")==0){
						ls(path,filename,1);
					}
				}else if(strcmp(argv[4],"-printi")==0){
					ls(path,filename,4);
				}else if(strcmp(argv[4],"-inum")==0){
					char *num = argv[5];
					if(num[0]!='-' && num[0]!='+'){
						inumvalue = atoi(num);
						ls(path,filename,6);
					}else{
						if(num[0]=='+'){
							const char *inputNum = num;
							char outputNum1[50];
							substring(inputNum, outputNum1);
							inumvalue = atoi(outputNum1);
							ls(path,filename,7);
						}else{
							const char *inputNum2 = num;
							char outputNum2[50];
							substring(inputNum2, outputNum2);
							inumvalue = atoi(outputNum2);
							ls(path,filename,8);
						}
					}
				}
			}
		}
	}
  
exit();
return 0;
}
