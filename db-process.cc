#include "sock.hpp"
// #define MSG_NOSIGNAL 524288
using namespace std;

DB_inst::DB_inst(const char *sock_path, int timeout){
    struct sockaddr_un addr;
    int fd, rc;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, sock_path);

    fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(fd<0){
        perror("socket error");
        exit(1);
    }
    this->sock_fd = fd;

    do{
        rc = connect(this->sock_fd, (struct sockaddr *)&addr, sizeof(addr));
        if(rc==0){
            break;
        } else {
            sleep(1);
        }
    } while(timeout-- > 0);

    if(rc != 0){
        perror("connect error");
        close(this->sock_fd);
        exit(1);
    }

    // sending the stdout file descriptor
    int err = this->send_stdout();
    if(err<0){
        perror("couldn't share stdout with the daemon");
        close(this->sock_fd);
        exit(1);
    }
}

DB_inst::~DB_inst(){
    close(this->sock_fd);
}

int DB_inst::send_stdout(){
    union {
        char   buf[CMSG_SPACE(sizeof(int))];
                        /* Space large enough to hold an 'int' */
        struct cmsghdr align;
    } controlMsg;

    
    struct msghdr msgh;
    msgh.msg_name = NULL;
    msgh.msg_namelen = 0;

    /* On Linux, we must transmit at least 1 Byte of real data in
       order to send ancillary data */

    struct iovec iov;
    int data = 12345;

    msgh.msg_iov = &iov;
    msgh.msg_iovlen = 1;
    iov.iov_base = &data;
    iov.iov_len = sizeof(data);
    // printf("Sending data = %d\n", data);

    /* Set 'msgh' fields to describe the ancillary data buffer */

    msgh.msg_control = controlMsg.buf;
    msgh.msg_controllen = sizeof(controlMsg.buf);

    /* The control message buffer must be zero-initialized in order
       for the CMSG_NXTHDR() macro to work correctly. Although we
       don't need to use CMSG_NXTHDR() in this example (because
       there is only one block of ancillary data), we show this
       step to demonstrate best practice */

    memset(controlMsg.buf, 0, sizeof(controlMsg.buf));

    /* Set message header to describe the ancillary data that
       we want to send */

    struct cmsghdr *cmsgp = CMSG_FIRSTHDR(&msgh);
    cmsgp->cmsg_len = CMSG_LEN(sizeof(int));
    cmsgp->cmsg_level = SOL_SOCKET;
    cmsgp->cmsg_type = SCM_RIGHTS;
    int fd = fileno(stdout);

    memcpy(CMSG_DATA(cmsgp), &fd, sizeof(int));

    ssize_t ns = sendmsg(this->sock_fd, &msgh, 0);
    if(ns == -1)
        return -1;
    
    return 0;
}

int DB_inst::send_query(const char *query){
    int q_len, bytes;
    q_len = strlen(query);

    bytes = send(this->sock_fd, query, q_len, 0);
    if(bytes != q_len){
        return -1;
    }

    char recv_string[3];
    bytes = recv(this->sock_fd, recv_string, 2, 0);
    if(bytes <= 0){
        perror("process: error ack recv");
    }

    return 0;
}


int main(){
    DB_inst *db = new DB_inst(SOCK_PATH, 10);
    char buf[MAX_BUFFLEN];
    char query[MAX_BUFFLEN];

    int i, last = 0, lq = 0;
    int rc;
    sleep(1);
    while(1){
        printf("$ ");
        cin.getline(buf, MAX_BUFFLEN-1); // 1 char always available for \0
        int len = strlen(buf);
        for(i=0; i<len; i++) {
            if(buf[i] == ';'){
                if(lq) {
                    query[lq] = ' ';
                    lq++;
                }
                strncpy(query+lq, buf+last, i-last+1);
                query[lq+i-last+1] = '\0';
                rc = db->send_query(query);
                if(rc < 0){
                    perror("connection error");
                    exit(1);
                }
                last = i+1;
                lq = 0;
            }
        }
        if(last != len){
            if(lq){
                query[lq] = ' ';
                lq++;
            }
            strncpy(query+lq, buf+last, len-last);
            lq += len - last;
        }
        last = 0;
    }
}