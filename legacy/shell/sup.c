#include <sup.h>

#define isBlank(c) ((c==' ') || (c=='\t') || (c=='\n'))
#define BUFFER_STEP 512

enum{
        PARSER_TOO_MANY_ARGUMENTS=1,
        PARSER_TOO_MANY_COMMANDS=2,
        PARSER_STUFF_AFTER_AMP=4
}parser_ERROR_T;

BUF_T *NewCmd(){
        BUF_T *rt;
        rt=(BUF_T *)malloc(sizeof(BUF_T));
        sysfail(!rt,NULL);
        rt->size=BUFFER_STEP;
        rt->buffer=(char *)malloc(BUFFER_STEP *sizeof(char));
        if(rt->buffer==NULL){
                free(rt);
                sysfail(1,NULL);
        }
        return rt;
}
void Free_cmd(BUF_T **tgt){
        if(tgt==NULL || *tgt==NULL) return;
        free((*tgt)->buffer);
        free(*tgt);
}

int read_cmd(BUF_T *tgt){
        int read_bytes, i, tmp;
        int keep_reading = 1;
        int how_many;
        char *offset, *p;

        while(keep_reading){
                read_bytes = read(1,offset,how_many);
                i+=read_bytes;

                if(offset[0]=='\n'){
                        offset[0]='\0';
                        tgt->lenght=0;
                        return --i;
                }
                keep_reading=0;

                if (read_bytes < how_many) offset[read_bytes-1]='\0';
                else{
                        if(offset[read_bytes-1]=='\n') offset[read_bytes-1]='\0';
                        else{
                                tmp =tgt->size;
                                tgt->size+=BUFFER_STEP;
                                how_many=BUFFER_STEP;
                                keep_reading=1;
                        }

                }
        }
        tgt->lenght=i;
        return i;
}


PIPELINE_T *new_pipeline(){
        PIPELINE_T *rt;
        int i,j;
        rt=(PIPELINE_T *)malloc(sizeof(PIPELINE_T));
        sysfail(!rt, NULL);
        rt->cmd=(char ***)malloc((MAX_COMMANDS+1)*sizeof(char **));
        if(!rt->cmd){
                sysdebug(1);
                free(rt);
                return NULL;
        }

        for(i=0;i<MAX_COMMANDS+1;i++){
                rt->cmd[i]=(char **)malloc((MAX_COMMANDS+1)*sizeof(char *));
                if(!rt->cmd[i]){
                        sysdebug(1);
                        for(j=0;j<i;j++) free(rt->cmd[j]);
                        free(rt->cmd);
                        free(rt);
                        return NULL;
                }
        }
        rt->ground=FOREGROUND;
        rt->N_Input[0]='\0';
        rt->N_Output[0]='\0';
        return rt;

}
void pipeline_free(PIPELINE_T *tgt){
        int i;
        for(i=0;i<MAX_COMMANDS;i++){
                free(tgt->cmd[i]);
        }
}

int find_modifiers(BUF_T *cmd,PIPELINE_T *tgt){
        int amp;
        int lt, gt;
        int len, truncated;
        int i,j,k;

        tgt->ground=FOREGROUND;
        tgt->N_Input[0]='\0';
        tgt->N_Output[0]='\0';

        truncated = 0;
        len=cmd->lenght;

        amp=strcspn(cmd->buffer,"&");

        if(amp!= (len-1)) tgt->ground = BACKGROUND;

        for(i=amp+1;i<len-1;i++)
                if(!isBlank(cmd->buffer[i])) truncated |= PARSER_STUFF_AFTER_AMP;
        cmd->buffer[amp]='\0';
        cmd->lenght=amp;
        len=amp;

        lt=strcspn(cmd->buffer,"<");
        if(lt!=len){
                j=0;
                while((i<len)&&(i<MAX_FILENAME)&&
                                (!isBlank(cmd->buffer[i]))&&(cmd->buffer[i]='<'))
                        tgt->N_Input[j++]=cmd->buffer[i++];
                tgt->N_Input[j]='\0';

                gt=strcspn(cmd->buffer, ">");
                if(gt!=len){
                        j=0;
                        while((i<len)&&(i<MAX_FILENAME)&&
                                        (!isBlank(cmd->buffer[i]))&&(cmd->buffer[i]='>'))
                                tgt->N_Output[j++]=cmd->buffer[i++];
                        tgt->N_Output[j]='\0';
                }

        }
        k= lt<gt ? lt: gt;
        k= k<len ? k:k+1;
        cmd->buffer[k-1]='\0';
        cmd->lenght=k;

        debug (truncated && PARSER_STUFF_AFTER_AMP, "Nothing allowed after '&'");

        return truncated;
}


char * stringdup (const char *str){
        char *p;
        int n;

        n = strlen (str)+1;
        p = malloc (n*sizeof(char));
        sysfail (!p, NULL);
        strcpy (p, str);
        return p;
}


int parse_cmd(BUF_T *src, PIPELINE_T *tgt){
        int i=0,j,truncated;
        char *token, *token2, *bkstring;
        truncated = 0;
        truncated |= find_modifiers(src, tgt);

        i=0;
        while((i<MAX_COMMANDS) && (token=strtok_r(i==0 ? src->buffer:NULL,"|",&bkstring))){
                while(token[0]==' ' || token[0]=='\t') token++;
                j=0;
                while((j<MAX_ARGS) && (token2 = strtok(j==0 ? token :NULL, " \t"))){
                        tgt->cmd[i][j]=token2;
                        j++;
                }
                tgt->cmd[i][j]=NULL;
                i++;
                truncated |= strtok (NULL, " \t") ? PARSER_TOO_MANY_ARGUMENTS : 0;
                tgt->n_args[i-1]=j;
        }
        tgt->cmd[i][0]=NULL;
        truncated |= strtok (NULL, " \t") ? PARSER_TOO_MANY_ARGUMENTS : 0;
        tgt->n_cmd=i;

        debug (truncated & PARSER_TOO_MANY_ARGUMENTS, "Too many arguments in a command");
        debug (truncated & PARSER_TOO_MANY_COMMANDS, "Too many commands in a pipeline");

        return truncated;

}
