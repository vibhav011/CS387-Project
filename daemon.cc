#include <signal.h>
#include <string.h>
#include <mutex>
#include <filesystem>
#include "sock.hpp"
#include <map>
#include <string>
#include "utils.h"
#include "receiver/helper.h"

using namespace std;

int Conn::id = 0;
vector<Temp_Table*> results;


Conn::Conn(){
    this->worker_id = id++;
    sprintf(this->fname, "/tmp/toydb.%d", this->worker_id);

    yylex_init_extra(&this->worker_id, &this->scanner);
}

Daemon::Daemon(const char *sock_path, request_handler_t func){
    this->conn = vector<Conn*>(MAX_PROCESSES);

    for(int i=0; i<MAX_PROCESSES; i++){
        this->conn[i] = new Conn(); 
        this->conn[i]->conn_thread = NULL; 
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

    results = vector<Temp_Table*>(MAX_PROCESSES);
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
        if(!this->conn[i]->inuse) break;
    }

    if(i>=MAX_PROCESSES){
        perror("already too many db-processes open\n");
        close(cl);
        return -1;
    }
    // cout << "here" << endl;
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
extern mutex query_mutex;
extern int yyparse(int);

int myhandler(string query, Conn *conn){
    cout << query << endl;

    conn->f = fopen(conn->fname, "w");
    fprintf(conn->f, "%s", query.c_str());
    fclose(conn->f);

    conn->f = fopen(conn->fname, "r");
    yyset_in(conn->f, conn->scanner);
    
    cout << query << endl;

    yyparse(conn->scanner);
    cout << query << endl;
    fclose(conn->f);
    cout<<query<<endl;

    results[conn->worker_id]->prettyPrint();
    cout<<"ankit"<<endl;
    delete results[conn->worker_id];
    results[conn->worker_id] = NULL;
    
    write(conn->stdout_fd, "this is another output\n", 23);

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

// void recover_from_folder(string folder_name) {
//     map<string, ChangeLog> change_logs;
//     map<string, MappingLog> mapping_logs;
//     for (const auto& dirEntry : std::filesystem::directory_iterator(folder_name)) {
//         if (!dirEntry.is_directory()) {
//             string s = dirEntry.path().filename();
//             if (s.size() > 5) {
//                 if (s.substr(s.size() - 5) == ".clog") {
//                     string clog_path = dirEntry.path().string();
//                     string table_name = s.substr(0, s.size() - 5);
//                     read_log(change_logs[table_name], clog_path);
//                 }
//                 else if (s.substr(s.size() - 5) == ".mlog") {
//                     string mlog_path = dirEntry.path().string();
//                     string table_name = s.substr(0, s.size() - 5);
//                     // TODO: implement following functions and uncomment
//                     // read_mlog(mapping_logs[table_name], mlog_path);
//                 }
//             }
//         }
//     }
//     for (auto it = change_logs.begin(); it != change_logs.end(); it++) {
//         string table_name = it->first;
//         ChangeLog change_log = it->second;

//         if (mapping_logs.find(table_name) == mapping_logs.end())
//             continue;

//         MappingLog mapping_log = mapping_logs[table_name];
//         // TODO: implement following functions and uncomment
//         // execute_rollback_single(tables[table_name_to_id[table_name]], change_log, mapping_log);
//     }
// }

// void setup_and_recover() {
//     vector<int> log_folders;
//     for (const auto& dirEntry : std::filesystem::directory_iterator("./data")) {
//         if (dirEntry.is_directory()) {
//             string s = dirEntry.path().filename();
//             if (s.size() > 4) {
//                 if (s.substr(s.size() - 4) == ".log") {
//                     log_folders.push_back(s);
//                 }
//             }
//         }
//         else {
//             string s = dirEntry.path().filename();
//             if (s.size() > 4) {
//                 if (s.substr(s.size() - 4) == ".tbl") {
//                     string tbl_name = s.substr(0, s.size() - 4);
//                     Table *table = new Table();
//                     Table_Open(s.c_str(), schema, false, &table);
//                     tables.push_back(table);
//                     //TODO: populate other vecotrs/maps
//                 }
//             }
//         }
//     }

//     for (string s: log_folders) {
//         recover_from_folder(s);
//         std::filesystem::remove(dirEntry.path());
//     }

// }

int main(){
    // setup_and_recover();

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

