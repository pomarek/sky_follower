#ifndef _COMMANDS_H_
    #define _COMMANDS_H_
#include <stdbool.h>
#include <stdint.h>

typedef bool (*cmd_handler_t)(char *params);

bool set_speed_command(char * params);
bool start_command(char * params);
bool stop_command(char *params);
bool move_command(char *params);
bool home_command(char *params);
bool debug_level_command(char *params);

#define COMMANDS \
        COMMAND(SPEED, "SPEED", set_speed_command)\
        COMMAND(START, "START", start_command)\
        COMMAND(STOP, "STOP", stop_command)\
        COMMAND(MOVE, "MOVE", move_command)\
        COMMAND(HOME1, "HOME", home_command)\
        COMMAND(DEBUG_LEVEL, "DEBUG", debug_level_command)

void commands_init(void);
void commands_process(void);

uint16_t get_uint16(char *params, uint32_t index);
uint16_t get_param_start(char *params, uint32_t index);
#endif //_COMMANDS_H_
