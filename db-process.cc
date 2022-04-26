#include "sock.hpp"
using namespace std;

DB_inst::DB_inst(const char *sock_path, int timeout){
    struct sockaddr_un addr;
    int fd, rc;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, sock_path);

    fd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
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
}

DB_inst::~DB_inst(){
    close(this->sock_fd);
}

int DB_inst::send_query(const char *query){
    int q_len, bytes;
    q_len = strlen(query);

    bytes = send(this->sock_fd, query, q_len, MSG_NOSIGNAL);
    if(bytes != q_len){
        return -1;
    }

    return 0;
}


int main(){
    DB_inst *db = new DB_inst(SOCK_PATH, 10);
    char buf[MAX_BUFFLEN];
    char query[MAX_BUFFLEN];

    int i, last = 0, lq = 0;
    int rc;
    while(1){
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
                // cout << query << endl;
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