#include <signal.h>
#include <string.h>
#include "sock.hpp"
using namespace std;

Daemon::Daemon(const char *sock_path, request_handler_t func){
    for(int i=0; i<MAX_PROCESSES; i++){
        this->conn[i].conn_thread = NULL;
        this->conn[i].worker_id = i;
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

    this->sock_fd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
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
}

Daemon::~Daemon(){
    close(this->sock_fd);
    for(int i=0; i<MAX_PROCESSES; i++){
        if(this->conn[i].inuse || this->conn[i].conn_thread!=NULL) this->conn[i].conn_thread->join();
        delete this->conn[i].conn_thread;
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

    memcpy(&conn->stdout_fd, CMSG_DATA(cmsgp), sizeof(int));

    write(conn->stdout_fd, "database connected\n", 19);

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
        if(!this->conn[i].inuse) break;
    }

    if(i>=MAX_PROCESSES){
        perror("already too many db-processes open\n");
        close(cl);
        return -1;
    }
    // cout << "here" << endl;
    if(this->conn[i].conn_thread != NULL) this->conn[i].conn_thread->join();

    this->conn[i].inuse = 1;
    this->conn[i].client_fd = cl;
    int err = this->recv_stdout(&this->conn[i]);

    if(err < 0){
        perror("didn't recieve the stdout fd");
        close(cl);
        return -1;
    }

    this->conn[i].conn_thread = new std::thread(&Daemon::thread_func, this, &this->conn[i]);

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
        int resp = this->query_handler(string(buf), conn->stdout_fd, conn->worker_id);

        q_len = send(conn->client_fd, "OK", 2, MSG_NOSIGNAL);
        if(q_len != 2){
            perror("error ack");
        }
    }

    printf("joining back: %p\n", conn);
}


/* 
    this function is executed on each arriving query
    need to call yyparse() here
*/
extern string global_query;
int myhandler(string query, int fd, int worker_id){
    size_t pos_start = 0, pos_end;
    string token;

    while (pos_start < query.size() && (pos_end = query.find (";", pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + 1;
        token.push_back(';');

        query_mutex.lock();
        global_query = token;
        yyparse(worker_id);
        query_mutex.unlock();
    }
    cout << query << endl;
    write(fd, "this is another output\n", 23);

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

int main(){
    Daemon *d = new Daemon(SOCK_PATH, myhandler);
    install_sig_handler();

    while(loop_flag){
        // cout << "here" << endl;
        d->accept_conn();
    }
    delete d;
    /*
        Finally we wait for all the threads to join back
        in the destructor
    */
}

