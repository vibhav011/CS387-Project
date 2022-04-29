#ifndef SOCK_H
#define SOCK_H

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/un.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <thread>
#include <vector>
#include "./receiver/query.h"
#include "ast.h"
#include "parser.tab.h"
#include "lex.yy.h"

#define MAX_BUFFLEN 1024
#define SOCK_PATH "/tmp/server.sock"

class DB_inst{
    int sock_fd;

public:
    DB_inst(const char *, int);
    ~DB_inst();

    int send_query(const char *);
    int send_stdout();
};

/*--------------------DAEMON RELATED DECLS----------------
----------------------------------------------------------*/
#define MAX_BACKLOG 10
/* Max number of db-processes connected at a time */
#define MAX_PROCESSES 10

struct Conn{
    static int id;
    int inuse;
    int worker_id;
    int client_fd;
    int stdout_fd;
    yyscan_t scanner;
    char fname[20];
    FILE *f;
    std::thread *conn_thread;

    Conn();
};

typedef int (*request_handler_t) (std::string, Conn*);

class Daemon{
    int sock_fd;
    std::vector<Conn*> conn;
    request_handler_t query_handler;

public:
    Daemon(const char *, request_handler_t);
    ~Daemon();
    
    // starts listening for connections
    int accept_conn();
    void thread_func(Conn *);
    int recv_stdout(Conn *);
};

#endif 