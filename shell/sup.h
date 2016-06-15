#ifndef SUP_H_
#define SUP_H_
/*MACROS*/
#define MAX_COMMANDS 256 
#define MAX_ARGS 512  
#define MAX_FILENAME 1024

#define FOREGROUND 1
#define BACKGROUND 0
#define SetForeground(PipeLine) (PipeLine->ground==FOREGROUND)
#define SetBackground(PipeLine) (PipeLine->ground==BACKGROUNDGROUND)

#define Redirect_STDin(PipeLine) (PipeLine->file_in[0] != '\0')
#define REDIRECT_STDout(PipeLine) (PipeLine->file_out[0] != '\0')



/*BUF_T
*  current buffer size;
*  current string lenght;
*  string.
*/

typedef struct BUF_T{
  int size; 
  int lenght;
  char *buffer;
}BUF_T;

/*BUF_T Commands*/

BUF_T *NewCmd();
void Free_cmd(BUF_T **tgt);
int read_cmd(BUF_T *tgt);

/*PIPELINE_T
*  command line
*  input redirection
*  output redirection
*  ground: Background or Foreground.  
*  Number of Commands
*  Number of Arguments
* 
*/

typedef struct PIPELINE_T{
  char ***cmd;
  char N_Input[MAX_FILENAME];
  char N_Output[MAX_FILENAME];
  int ground;
  int n_cmd;
  int n_args[MAX_ARGS];
}PIPELINE_T;


/*PIPELINE_T commands*/
PIPELINE_T *new_pipeline();
void pipeline_info(PIPELINE_T *tgt);
int parse_cmd(BUF_T *src, PIPELINE_T *tgt);



#endif
