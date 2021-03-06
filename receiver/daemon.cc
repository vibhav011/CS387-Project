#include <signal.h>
#include <string.h>
#include <mutex>
#include <filesystem>
#include <map>
#include <string>
#include "sock.hpp"
#include "utils.h"
#include "helper.h"
#include "commit.h"
// #define MSG_NOSIGNAL 524288

using namespace std;

int Conn::id = 0;
vector<Temp_Table*> results;
extern set<string> changed_tables[MAX_PROCESSES];

Conn::Conn(){
    this->worker_meta.id = id++;
    sprintf(this->fname, "/tmp/toydb.%d", this->worker_meta.id);
    this->inuse = 0;
    this->conn_thread = NULL;
}

Daemon::Daemon(const char *sock_path, request_handler_t func){
    this->conn = vector<Conn*>(MAX_PROCESSES);

    for(int i=0; i<MAX_PROCESSES; i++){
        this->conn[i] = new Conn(); 
    }

    struct sockaddr_un addr;
    int i, rc;

    if (func==NULL){
        printf("Error: invalid parameter\n");
        exit(1);
    }   
    this->query_handler = func;

    unlink(sock_path);

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, sock_path);

    this->sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(this->sock_fd < 0){
        perror("cannot open listen socket");
        exit(1);
    }

    rc = bind(this->sock_fd, (struct sockaddr *)&addr, sizeof(addr));
    if(rc!=0){
        perror("bind error");
        close(this->sock_fd);
        exit(1);
    }

    rc = listen(this->sock_fd, MAX_BACKLOG);
    if(rc != 0){
        perror("listen error");
        close(this->sock_fd);
        exit(1);
    }

    results = vector<Temp_Table*>(MAX_PROCESSES, NULL);
}

Daemon::~Daemon(){
    close(this->sock_fd);
    for(int i=0; i<MAX_PROCESSES; i++){
        if(this->conn[i]->inuse || this->conn[i]->conn_thread!=NULL) this->conn[i]->conn_thread->join();
        delete this->conn[i]->conn_thread;
    }
}

int Daemon::recv_stdout(Conn *conn){
    union {
        char buf[CMSG_SPACE(sizeof(int))];
        struct cmsghdr align;
    } controlMsg;

    struct msghdr msgh;
    msgh.msg_name = NULL;
    msgh.msg_namelen = 0;

    struct iovec iov;
    int data;

    msgh.msg_iov = &iov;
    msgh.msg_iovlen = 1;

    iov.iov_base = &data;
    iov.iov_len = sizeof(data);

    msgh.msg_control = controlMsg.buf;
    msgh.msg_controllen = sizeof(controlMsg.buf);

    /* recieve stdout as ancillary data */
    ssize_t nr = recvmsg(conn->client_fd, &msgh, 0);
    if(nr == -1){
        perror("couldn't share stdout");
        return -1;
    }

    struct cmsghdr *cmsgp = CMSG_FIRSTHDR(&msgh);

    /* Check the validity of the 'cmsghdr' */

    if (cmsgp == NULL || cmsgp->cmsg_len != CMSG_LEN(sizeof(int))){
        printf("bad cmsg header / message length");
        return -1;
    }
    if (cmsgp->cmsg_level != SOL_SOCKET){
        printf("cmsg_level != SOL_SOCKET");
        return -1;
    }
    if (cmsgp->cmsg_type != SCM_RIGHTS){
        printf("cmsg_type != SCM_RIGHTS");
        return -1;
    }
    int fd;
    memcpy(&fd, CMSG_DATA(cmsgp), sizeof(int));

    conn->worker_meta.out = fdopen(fd, "a+");
    fprintf(conn->worker_meta.out, "Database connected\n\n");
    // write(conn->stdout_fd, "database connected\n", 19);

    return 0;
}

int Daemon::accept_conn(){
    int i, cl;

    cl = accept(this->sock_fd, NULL, NULL);
    if(cl < 0){
        perror("accept error");
        return -1;
    }

    /* Find free slot in the connections array */
    for (i = 0; i < MAX_PROCESSES; i++){
        if(!this->conn[i]->inuse) break;
    }

    if(i>=MAX_PROCESSES){
        perror("already too many db-processes open\n");
        close(cl);
        return -1;
    }
    if(this->conn[i]->conn_thread != NULL) this->conn[i]->conn_thread->join();

    this->conn[i]->inuse = 1;
    this->conn[i]->client_fd = cl;
    int err = this->recv_stdout(this->conn[i]);

    if(err < 0){
        perror("didn't recieve the stdout fd");
        close(cl);
        return -1;
    }

    this->conn[i]->conn_thread = new std::thread(&Daemon::thread_func, this, this->conn[i]);

    return 0;
}

/* 
    Every thread initiates from this function, with connection 
    that particular action thread as argument    
*/
void Daemon::thread_func(Conn *conn){
    printf("entering thread addr: %p\n", conn);
    fflush(stdout);

    char buf[MAX_BUFFLEN];
    int q_len, resp;

    while(1){
        q_len = recv(conn->client_fd, buf, MAX_BUFFLEN-1, 0);
        buf[q_len>=0?q_len:0] = '\0';

        if(q_len <= 0) {
            close(conn->client_fd);
            conn->inuse = 0;
            break;
        }
        int resp = this->query_handler(string(buf), conn);

        q_len = send(conn->client_fd, "OK", 2, 0);
        if(q_len != 2){
            perror("error ack");
        }
    }
    execute_rollback(changed_tables[conn->worker_meta.id]);
    fclose(conn->worker_meta.out);
    printf("joining back: %p\n", conn);
}


/* 
    this function is executed on each arriving query
    need to call yyparse() here
*/

extern int yyparse(int);

int myhandler(string query, Conn *conn){
    conn->f = fopen(conn->fname, "w");
    fprintf(conn->f, "%s", query.c_str());
    fclose(conn->f);

    conn->f = fopen(conn->fname, "r");
    yylex_init_extra(&conn->worker_meta, &conn->scanner);
    yyset_in(conn->f, conn->scanner);
    
    yyparse(conn->scanner);
    fclose(conn->f);
    free(conn->scanner);

    // FILE *f = fdopen(conn->stdout_fd, "a+");

    if (results[conn->worker_meta.id] != NULL) {
        results[conn->worker_meta.id]->prettyPrint(conn->worker_meta.out);
        delete results[conn->worker_meta.id];
        results[conn->worker_meta.id] = NULL;
    }
    
    // write(conn->stdout_fd, "this is another output\n", 23);

    return 0;
}

volatile sig_atomic_t loop_flag = 1;

void handler_sigint(int sig){
    loop_flag = 0;
}

void install_sig_handler(){
    struct sigaction act;

    sigemptyset(&act.sa_mask);
    act.sa_handler = handler_sigint;
    act.sa_flags = 0;
    sigaction(SIGINT, &act, 0);
}

extern void setup_and_recover();

int main(){
    setup_and_recover();
    cout << "startup done" << endl;
    Daemon *d = new Daemon(SOCK_PATH, myhandler);
    install_sig_handler();

    while(loop_flag){
        d->accept_conn();
    }
    delete d;
    /*
        Finally we wait for all the threads to join back
        in the destructor
    */
}

