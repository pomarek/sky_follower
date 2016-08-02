#include <stddef.h>
#include <stdint.h>
#include "uart.h"
#include "commands.h"
#include "debug.h"

#define COMMAND_BUF_SIZE 25 //must be a little longer than max command with parameters
#define COMMAND_PARAM_SIZE 20


#define COMMAND_ENUM(NAME, CMD, HANDLER) COMMAND_ ## NAME,
#define COMMAND_STR(NAME, CMD, HANDLER) CMD,
#define COMMAND_HANDLER(NAME, CMD, HANDLER) HANDLER,
    
#define OK_STR "OK\r\n"
#define FAIL_STR "COMMAND FAILED\r\n"

#define COMMAND COMMAND_ENUM
typedef enum
{
    COMMANDS
    COMMAND_NONE
}command_type_t;

#undef COMMAND
#define COMMAND COMMAND_STR
const char *commands[] = 
{
    COMMANDS
    NULL
};

#undef COMMAND
#define COMMAND COMMAND_HANDLER
const cmd_handler_t handlers[] =
{
    COMMANDS
    NULL
};

void commands_init(void)
{
}

static char * read_line()
{
    static char buf[COMMAND_BUF_SIZE];
    static int pos =0;
    int16_t read;
    
    read = uart_read_byte(false);
    switch(read)
    {
        //ending chars
        case '\0':
        case '\n':
            buf[pos] = '\0';
            if(pos == COMMAND_BUF_SIZE -1 || pos == 0) //if string too long return null
            {
                pos = 0;
                return NULL;
            }
            pos = 0;
            return buf;
        //ignored chars
        case '\r':
            break;
        //no data
        case -1:
            break; //didn't read anything
        //supported chars
        default:
            if(pos < COMMAND_BUF_SIZE -1)
                buf[pos++] = (char)read;
            break;
        
    }
    return NULL;
}
static command_type_t search_cmd(char * line)
{
    command_type_t i;
    int j;
    for(i=(command_type_t)0; commands[i]!=NULL; i++)
    {
        for(j=0; commands[i][j]!='\0' && line[j]!='\0'; j++)
        {
            if(commands[i][j] != line[j])
                break;
        }
        if(commands[i][j]=='\0' && (line[j]=='\0' || line[j]!=' '))
            break;
    }
    return i;
}
static command_type_t read_command(char **out_param)
{
    char * line;
    int i;
    *out_param = NULL;
    line = read_line();
    if(line == NULL)
        return COMMAND_NONE;
    for(i=0; line[i]!='\0'; i++)
        if(line[i] == ' ')
        {
            line[i++]='\0';
            *out_param = &line[i];
            break;
        }
    return search_cmd(line);
}

void commands_process(void)
{
    char *param;
    command_type_t command = read_command(&param);
    if(handlers[command] != NULL)
    {
        if(handlers[command](param))
            uart_poll_send(OK_STR, sizeof(OK_STR));
        else
            uart_poll_send(FAIL_STR, sizeof(FAIL_STR));
        
    }
}
