#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>
using namespace std;
struct Potato_t{
int num_hops;
int trace[512];
int curr_num;
};
typedef struct Potato_t Potato;

struct Player_info_t{
    int socket_fd;
    char hostname[16];
    unsigned short int port;
};
typedef struct Player_info_t Player_info;
int build_listen_socket(const char *port){
    int status;
    int socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    const char *hostname = NULL;
    //make sure it is empty
    memset(&host_info, 0, sizeof(host_info));
    memset(&host_info_list, 0, sizeof(host_info_list));
    host_info.ai_family   = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    host_info.ai_flags    = AI_PASSIVE;
    
    //get information in host_info and host_info_list
    status = getaddrinfo(hostname, port, &host_info, &host_info_list);
    if (status != 0) {
        cerr << "Error: cannot get address info for host" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        exit(EXIT_FAILURE);
    }
    if(strlen(port)<1){//indicate random port
        ((struct sockaddr_in*) host_info_list->ai_addr)->sin_port=0;
    }
    //Setup listen socket
    socket_fd = socket(host_info_list->ai_family, 
                host_info_list->ai_socktype, 
                host_info_list->ai_protocol);
    if (socket_fd == -1) {
        cerr << "Error: cannot create socket" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        exit(EXIT_FAILURE);
    }    
    //Set various options for a socket
    //Allow other socket to bind to the port unless these is an active listening stock
    int yes = 1;
    status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (status == -1) {
        cerr << "Error: Address is already in use" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        exit(EXIT_FAILURE);
    } 
    status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1) {
        cerr << "Error: cannot bind socket" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        exit(EXIT_FAILURE);
    }
    status = listen(socket_fd, 100);
    if (status == -1) {
        cerr << "Error: cannot listen on socket" << endl; 
        cerr << "  (" << hostname << "," << port << ")" << endl;
        exit(EXIT_FAILURE);
    }
    // cout<<ntohs(((struct sockaddr_in*) host_info_list->ai_addr)->sin_port)<<endl;
    freeaddrinfo(host_info_list);
    return socket_fd;
}

int request_connection(const char *hostname, const char *port ){
    int status;
    int socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family   = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    //Get host infomation
    status = getaddrinfo(hostname, port, &host_info, &host_info_list);
    if (status != 0) {
        cerr << "Error: cannot get address info for host" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        exit(EXIT_FAILURE);
    }
    //Setup socket
    socket_fd = socket(host_info_list->ai_family, 
		     host_info_list->ai_socktype, 
		     host_info_list->ai_protocol);
    if (socket_fd == -1) {
        cerr << "Error: cannot create socket" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        exit(EXIT_FAILURE);
    } 
    //Build connection
    status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1) {
        cerr << "Error: cannot connect to socket" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        exit(EXIT_FAILURE);
    } 
    freeaddrinfo(host_info_list);
    return socket_fd;
}

Player_info master_accpet(int socket_fd){
    Player_info player;
    memset(&player, 0, sizeof(player));
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    int player_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
    if (player_fd == -1) {
    cerr << "Error: cannot accept connection on master listening socket" << endl;
    exit(EXIT_FAILURE);
  }
  sprintf(player.hostname, "%s", inet_ntoa(((struct sockaddr_in*)&socket_addr)->sin_addr));
//   player.hostname= inet_ntoa(((struct sockaddr_in*)&socket_addr)->sin_addr);
  player.socket_fd=player_fd;
  return player;
}

int player_accept(int socket_fd){
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    int player_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
    if (player_fd == -1) {
        cerr << "Error: cannot accept connection on master listening socket" << endl;
        exit(EXIT_FAILURE);
  }
  return player_fd;
}

int try_recv(int sockfd, void *buf, int len, int flags){
    int status = recv(sockfd, buf, len, flags);
    if(status==-1){
        cerr << "Error: error occurs in receiving message" << endl;
    }
    return status;
}
unsigned short int get_port(int sockfd){
    struct sockaddr_storage socket_add;
    // struct sockaddr  socket_add;
    socklen_t add_len = sizeof(socket_add);
    int status = getsockname(sockfd,(struct sockaddr*)&socket_add,&add_len);
    if(status==-1){
        cerr << "Error: cannot get port corretly" << endl;
        exit(EXIT_FAILURE);
    }
    return ntohs(((struct sockaddr_in*)&socket_add)->sin_port);
}

