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

typedef int (*request_handler_t) (std::string,int);

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
    int inuse;
    int client_fd;
    int stdout_fd;
    std::thread *conn_thread;
};

class Daemon{
    int sock_fd;
    Conn conn[MAX_PROCESSES];
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