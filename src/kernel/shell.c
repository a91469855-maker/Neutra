#include "shell.h"
#include "idt.h"
#include "vga.h"
#include "kernel.h"
#include "graphics.h"
#include "dce.h"

#define CMD_BUFFER_SIZE 256

static char cmd_buffer[CMD_BUFFER_SIZE];
static char current_dir[256] = "/";

extern char keyboard_getchar_poll(void);
extern FramebufferInfo fb_info;

int strncmp(const char* s1, const char* s2, int n) {
    for (int i = 0; i < n; i++) {
        if (s1[i] != s2[i]) return 1;
        if (s1[i] == '\0') return 0;
    }
    return 0;
}

void shell_init(void) {
    
    vga_clear();
    vga_write("Neutra OS Shell\n", MAKE_COLOR(COLOR_BLACK, COLOR_CYAN));
    
    vga_write("FB: has=", MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_YELLOW));
    vga_print_int(fb_info.has_framebuffer, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_YELLOW));
    vga_write(" w=", MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_YELLOW));
    vga_print_int(fb_info.framebuffer_width, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_YELLOW));
    vga_write(" h=", MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_YELLOW));
    vga_print_int(fb_info.framebuffer_height, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_YELLOW));
    vga_write("\n", MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_YELLOW));
    
    vga_write("Type 'help' for commands\n\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
}

void shell_prompt(void) {
    vga_write("neutra@kernel:", MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
    vga_write(current_dir, MAKE_COLOR(COLOR_BLACK, COLOR_CYAN));
    vga_write("$ ", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
}

void shell_read_line(char* buffer, int size) {
    int i = 0;
    while (i < size - 1) {
        char c = keyboard_getchar_poll();
        if (c == '\n') {
            buffer[i] = '\0';
            vga_putchar('\n', MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
            break;
        } else {
            buffer[i++] = c;
            vga_putchar(c, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
        }
    }
}

void cmd_whoami(void) {
    vga_write("root\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
}

void cmd_pwd(void) {
    vga_write(current_dir, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
}

void cmd_ls(void) {
    vga_write("kernel.elf  boot/  dev/  proc/  sys/  home/\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
}

void cmd_ls_la(void) {
    vga_write("drwxr-xr-x  root  root  4096  kernel\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("-rw-r--r--  root  root  262144  kernel.elf\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("drwxr-xr-x  root  root  4096  boot\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("drwxr-xr-x  root  root  4096  dev\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
}

void cmd_ls_lh(void) {
    vga_write("drwxr-xr-x  root  root  4.0K  kernel\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("-rw-r--r--  root  root  256K  kernel.elf\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("drwxr-xr-x  root  root  4.0K  boot\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("drwxr-xr-x  root  root  4.0K  dev\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
}

void cmd_echo(const char* text) {
    vga_write(text, MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
}

void cmd_uname(void) {
    vga_write("Neutra OS 1.0 neutra_attempt3\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
}

void cmd_execute(void) {
    extern void execute_cde_task(void);
    execute_cde_task();
}

void cmd_help(void) {
    vga_write("Available commands:\n", MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
    vga_write("  whoami   - Show current user\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("  pwd      - Print working directory\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("  ls       - List directory\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("  ls -la   - List with details\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("  ls -lh   - List human readable\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("  echo     - Print text\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("  uname    - System info\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("  execute  - Run CDE programs\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("  help     - This help\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
    vga_write("  exit     - Exit shell\n", MAKE_COLOR(COLOR_BLACK, COLOR_WHITE));
}

void shell_execute_command(const char* cmd) {
    if (strcmp(cmd, "") == 0) return;
    if (strcmp(cmd, "whoami") == 0) cmd_whoami();
    else if (strcmp(cmd, "pwd") == 0) cmd_pwd();
    else if (strcmp(cmd, "ls") == 0) cmd_ls();
    else if (strcmp(cmd, "ls -la") == 0) cmd_ls_la();
    else if (strcmp(cmd, "ls -lh") == 0) cmd_ls_lh();
    else if (strncmp(cmd, "echo ", 5) == 0) cmd_echo(cmd + 5);
    else if (strcmp(cmd, "uname") == 0) cmd_uname();
    else if (strcmp(cmd, "execute") == 0) cmd_execute();
    else if (strcmp(cmd, "help") == 0) cmd_help();
    else if (strcmp(cmd, "exit") == 0) return;
    else if (strcmp(cmd, "test fb") == 0) test_framebuffer();
    else {
        vga_write("command not found: ", MAKE_COLOR(COLOR_BLACK, COLOR_RED));
        vga_write(cmd, MAKE_COLOR(COLOR_BLACK, COLOR_RED));
        vga_write("\n", MAKE_COLOR(COLOR_BLACK, COLOR_RED));
    }
}

void shell_run(void) {
    shell_init();
    while (1) {
        shell_prompt();
        shell_read_line(cmd_buffer, CMD_BUFFER_SIZE);
        if (strcmp(cmd_buffer, "exit") == 0) break;
        shell_execute_command(cmd_buffer);
    }
}

