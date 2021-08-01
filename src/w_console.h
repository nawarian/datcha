#ifndef w_console_h
#define w_console_h

typedef struct ConsoleState {
    bool typing;
    int cursor_pos;
    int log_pos;
    int scroll_pos;
    char buff[1024];
    char log[50][1024]; // last 50 messages
} ConsoleState;

void w_console_update(void);
void w_console_draw(void);

#endif

