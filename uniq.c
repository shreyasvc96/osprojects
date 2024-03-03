//NOW WORKING


#include "types.h"
#include "user.h"
#include "stat.h"
#include "fs.h"
#include "syscall.h"
#include "fcntl.h"

#define MAX_LINE_LENGTH 1000

void unique(int file_Descriptor,int argc,char *argv[]);
void argumentchecker(int argc,char *argv[]);
char *xv6_strdup(const char *s);
char** getUniqueSentences(char **sentences, int sentenceCount, int *uniqueCount);
int isUnique(char *sentence, char **uniqueArray, int uniqueCount);
void countRepetitions(char **allSentences, char **uniqueSentences, int repetitionCount[], int numSentences, int numUniqueSentences);
void uniq_w(char *input[], int size, char *output[], int *output_size, int count[],int WIDTH);
int mystrncmp(const char *s1, const char *s2, int n);
char* mystrncpy(char *dest, const char *src, int n);

int C_Flag=0;
int U_Flag=0;
int W_Flag=0;
int arg_counter=0;
int width=0;
//int file;


int main(int argc, char * argv[])
{
int fileopenstatus;
if(argc>2)
{
//printf(1,"Argument count >2");
argumentchecker(argc,argv);
arg_counter =arg_counter+1;
//printf(1,"File Name - %s\n",argv[arg_counter]);
//printf(1,"%s",argv[arg_counter]);
fileopenstatus =open(argv[arg_counter],0);
//file =open(argv[arg_counter],0);
if(fileopenstatus>=0)
{
//printf(1,"File successfully opened");
//printf(1,"filestatus= %d\n",fileopenstatus);
unique(fileopenstatus,argc,argv);
exit();
}
else
{
printf(1,"Error :Not able to open file %s\n",argv[arg_counter+1]);
exit();
}
}
if(argc==1)
{
//printf(1,"uniq: Invalid Input, Pass arguments\n example: uniq filename");
fileopenstatus = 0 ;
//file=0;
unique(fileopenstatus,argc,argv);
exit();
}
if(argc==2)
{
arg_counter=1;
fileopenstatus =open(argv[1],0);
//file =open(argv[arg_counter],0);
if(fileopenstatus>=0)
{
//printf(1,"filestatus= %d\n",fileopenstatus);
//printf(1,"File Name=%s\n",argv[1]);
unique(fileopenstatus,argc,argv);
exit();
}
else
{
printf(1,"Error :Not able to open file %s\n",argv[1]);
exit();
}
}

exit();
return 0;
}


void argumentchecker(int argc,char *argv[])
{
//int w;
int i;
for(i=0;i<argc;i++)
{
if(strcmp("-c",argv[i])==0)
{
C_Flag = 1;
arg_counter = arg_counter+1;
//printf(1,"Count needed=%d \n",count_Needed);
}
else if(strcmp("-u",argv[i])==0)
{
U_Flag=1;
arg_counter= arg_counter+1;
//printf(1,"unique_Lines=%d \n",unique_Lines);
}
else if(strcmp("-w",argv[i])==0)
{
W_Flag=1;
//printf(1,"Count_char=%d \n",check_Char);
arg_counter= arg_counter+1;
//printf(1,"\n Width before conversion- %s",argv[arg_counter]);
width = atoi(argv[arg_counter+1]);
//printf(1,"\n Width- %d",width);
arg_counter= arg_counter+1;
}
}
//printf(1,"%s\n",argv[arg_counter+1]);
}

void unique(int file_Descriptor,int argc,char *argv[])
{

//printf(1,"Unique  successfully opened\n");
char buf[1024];
struct stat s;
if(file_Descriptor>0)
{
int stat_status = fstat(file_Descriptor,&s);
if(stat_status<0)
{
//printf(1,"Error : File Descriptor Stat\n");
//close(file_Descriptor);
exit();
}
}
//printf(1,"size - %d \n",s.size);
//printf(1,"Fstat successfull\n");
int sentence_count=0;
int n;
int i;
int j=0;
char **sentences = '\0';

//int line_Length_Max=10000;
//line_Length_Max=s.size;
while((n=read(file_Descriptor,buf,sizeof(buf)))>0)
{
   //printf(1,"N value - %d\n",n);
 for(i=0;i<n;i++)
{
//printf(1,"%c",buf[i]);
if(buf[i]=='\n')
{
sentence_count++;
}
}
//printf(1,"%d",sentence_count);
if(sentences=='\0')
{
 sentences = (char **)malloc(sentence_count * sizeof(char *));
     //printf(1,"Sentence Allocation is done");
}
char *newline = strchr(buf, '\n');
        while (newline != '\0') 
        {
            //printf(1,"New Line  and making sentences");
            *newline = '\0'; // Replace newline with null terminator
            sentences[j] = xv6_strdup(buf); // Dynamically allocate memory for the line
            if (sentences[j] == '\0') {
                printf(1, "Memory allocation failed.\n");
                close(file_Descriptor);
                exit();
            }
            //printf(1, "--%s\n", sentences[j]);
            j++;
            strcpy(buf, newline + 1);

            //buffer = newline + 1;
            newline = strchr(buf, '\n');
        }

}
//sentence_count=10;
//printf(1,"Sentence_Count %d",sentence_count);
/*   char **sentences = (char **)malloc(sentence_count * sizeof(char *));
    //close(file_Descriptor);
// Read sentences from the file and store in the array
    
    char buffer[1024];
i = 0;
    while (read(file, buffer, sizeof(buffer)) > 0) 
    {
        //printf(1,"File Content Reading");
        char *newline = strchr(buffer, '\n');
        while (newline != '\0') 
        {
            *newline = '\0'; // Replace newline with null terminator
            sentences[i] = xv6_strdup(buffer); // Dynamically allocate memory for the line
            if (sentences[i] == '\0') {
                printf(1, "Memory allocation failed.\n");
                close(file);
                exit();
            }
            printf(1, "--%s\n", sentences[i]);
            i++;
            strcpy(buffer, newline + 1);

            //buffer = newline + 1;
            newline = strchr(buffer, '\n');
        }
    }
    close(file);
    */ 
    int uniqueCount;

    // Get unique sentences
    char **uniqueArray = getUniqueSentences(sentences, sentence_count, &uniqueCount);

    //printf(1,"C--%d,U--%d,W--%d",C_Flag,U_Flag,W_Flag);
    //Basic Functionality of Uniq
    if(argc==1 || argc ==2)
    {
        
        for (i = 0; i < uniqueCount; ++i)
     {
        printf(1,"%s\n", uniqueArray[i]);
        free(uniqueArray[i]); // Free memory allocated for each unique sentence
      }
      exit();
      
    }

    //C Flag Implementation
    int *repetitionCount = (int *)malloc(uniqueCount * sizeof(int));
     if (repetitionCount == '\0') {
        printf(1, "Memory allocation failed.\n");
        exit();
    }
   memset(repetitionCount, 0, uniqueCount * sizeof(int));
   countRepetitions(sentences, uniqueArray, repetitionCount, sentence_count, uniqueCount);

    if(C_Flag==1)
    {
         // Output repetition count
   // printf(1,"\nRepetition Count:\n");
    for (i = 0; i < uniqueCount; i++)
     {
        printf(1,"%d\t%s\n", repetitionCount[i],uniqueArray[i]);
    }
    exit();
    }
    // U_Flag Implementation
    if(U_Flag==1)
    {
        //printf(1,"\n U flag- unique line:\n");
    for(i=0;i<uniqueCount;i++)
    {
        if(repetitionCount[i]==1)
        {
            printf(1,"%s\n",uniqueArray[i]);
        }   
    }
    exit();
    }

    //W Flag Implementation
    int uniqueCountWidth;
    int *count;
    //int width=3;
    count = (int *)malloc(sentence_count * sizeof(int));
    if (count == '\0') 
    {
        printf(1, "Memory allocation failed for count.\n");
        free(sentences);
        exit();
    }
    char **uniqueArrayWithWidth = (char **)malloc(sentence_count * sizeof(char *));
    if (uniqueArrayWithWidth == '\0') {
        printf(1, "Memory allocation failed for uniqueArrayWithWidth.\n");
        free(sentences);
        free(count);
        exit();
    }

    uniq_w(sentences, sentence_count, uniqueArrayWithWidth, &uniqueCountWidth, count,width);
    if(W_Flag==1)
    {
       // printf(1,"Unique Sentences with Width Information:\n");
    for (i = 0; i < uniqueCountWidth; ++i) 
    {
        printf(1,"%s\n", uniqueArrayWithWidth[i]);
        free(uniqueArrayWithWidth[i]); // Free memory allocated for each unique sentence
    }
    }



}

char *xv6_strdup(const char *s) {
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }

    char *new_str = (char *)malloc(len + 1); // +1 for the null terminator
    if (new_str == '\0') {
        return '\0'; // Memory allocation failure
    }

    int i;
    for (i = 0; i <= len; i++) {
        new_str[i] = s[i];
    }

    return new_str;
}

char** getUniqueSentences(char **sentences, int sentenceCount, int *uniqueCount) {
    // Initialize an array to store unique sentences
    char **uniqueArray = (char **)malloc(sentenceCount * sizeof(char *));
    
    if (uniqueArray == '\0') {
        printf(1, "Memory allocation failed.\n");
        exit();
    }

    *uniqueCount = 0;
int i;
    for (i = 0; i < sentenceCount; ++i) {
        // Check if the sentence is unique
        if (isUnique(sentences[i], uniqueArray, *uniqueCount)) {
            // Allocate memory for the sentence and copy it to the unique array
            int len = 0;
            while (sentences[i][len] != '\0') {
                ++len;
            }

            uniqueArray[*uniqueCount] = (char *)malloc((len + 1) * sizeof(char));
            
            if (uniqueArray[*uniqueCount] == '\0') {
                printf(1, "Memory allocation failed.\n");
                exit();
            }

            int j = 0;
            while (sentences[i][j] != '\0') {
                uniqueArray[*uniqueCount][j] = sentences[i][j];
                ++j;
            }

            uniqueArray[*uniqueCount][j] = '\0';
            (*uniqueCount)++;
        }
    }

    return uniqueArray;
}

int isUnique(char *sentence, char **uniqueArray, int uniqueCount) {
    int i;
    for ( i = 0; i < uniqueCount; ++i) {
        int j = 0;
        while (uniqueArray[i][j] != '\0' && sentence[j] != '\0') {
            if (uniqueArray[i][j] != sentence[j]) {
                break;
            }
            ++j;
        }

        if (uniqueArray[i][j] == '\0' && sentence[j] == '\0') {
            return 0; // Not unique
        }
    }
    return 1; // Unique
}

void countRepetitions(char **allSentences, char **uniqueSentences, int repetitionCount[], int numSentences, int numUniqueSentences)
{
int i,j;
    for (i = 0; i < numSentences; i++) {
        for ( j = 0; j < numUniqueSentences; j++) {
            if (strcmp(allSentences[i], uniqueSentences[j]) == 0) {
                repetitionCount[j]++;
            }
        }
    }
}

void uniq_w(char *input[], int size, char *output[], int *output_size, int count[],int WIDTH) 
{
    char previous_line[MAX_LINE_LENGTH] = "";
    int current_count = 0;
    int result_index = 0;
    int i;
    for ( i = 0; i < size; ++i) {
        // Compare only the first 'WIDTH' characters
        if (mystrncmp(input[i], previous_line, WIDTH) != 0) {
            if (current_count > 0) {
                // Store count for the previous line
                count[result_index] = current_count;
                result_index++;
                current_count = 0; // Reset count for the new line
            }

            // Allocate memory for the current line in the output array
            output[result_index] = xv6_strdup(input[i]);
            mystrncpy(previous_line, input[i], MAX_LINE_LENGTH);
        }

        // Check if the line starts with the specified width
        if (mystrncmp(input[i], previous_line, WIDTH) == 0) {
            current_count++;
        }
    }

    // Store the last line count
    count[result_index] = current_count;
    result_index++;

    *output_size = result_index;
}

int mystrncmp(const char *s1, const char *s2, int n) 
{
    while (n && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }

    if (n == 0) {
        return 0;  // Strings are equal up to the first n characters
    } else {
        return (*(unsigned char *)s1 - *(unsigned char *)s2);
    }
}
char* mystrncpy(char *dest, const char *src, int n)
 {
    char *originalDest = dest;

    while (n > 0 && (*dest++ = *src++) != '\0') {
        n--;
    }

    while (n > 0) {
        *dest++ = '\0';
        n--;
    }

    return originalDest;
}
