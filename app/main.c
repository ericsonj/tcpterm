
#include <termios.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include "TCPClient.h"
#include "cmsis_os2.h"

#define IP "127.0.0.1"
#define PORT 8022

Connection* conn;

char get_char(void) {
    struct termios oldt, newt;
    int            ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void sock2stdout(void* args) {
    for (;;) {
    	memset(conn->buff, 0, TCP_CONN_BUFF_SIZE);
        int16_t read = TCPClient_read(conn, 5000);
        if (read > 0) {
        	write(1, conn->buff, read);
        }
    }
}

void stdin2sock(void* args) {
	TCPClient_write(conn, "\n", sizeof(char));
    for (;;) {
        char c = get_char();
        TCPClient_write(conn, &c, sizeof(char));
    }
}

int main(int argc, char* argv[]) {
    osKernelInitialize();

    conn = TCPClient_connect(IP, PORT);

    if (conn != NULL) {

        osThreadAttr_t thattr;
        thattr.name = "in2sock";
        osThreadNew(stdin2sock, NULL, &thattr);

        thattr.name = "sock2out";
        osThreadNew(sock2stdout, NULL, &thattr);

    }

    osKernelStart();
}
