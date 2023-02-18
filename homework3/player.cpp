#include "potato.h"
using namespace std;

int main(int argc, char *argv[]){
    if (argc < 3) {
        cout << "Syntax: player <machine_name> <port_num>\n" << endl;
        return EXIT_FAILURE;
    }
    const char *hostname = argv[1];
    const char *port = argv[2];
    int player_id;
    int num_player;
    vector<int> select_ports;
    Player_info left_player;
    int listener,left_fd, right_fd;
    int master_fd = request_connection(hostname,port);
    select_ports.push_back(master_fd);
    unsigned short int listen_port;
    try_recv(master_fd, &player_id, sizeof(player_id), 0);
    try_recv(master_fd, &num_player, sizeof(num_player), 0);
    cout<<"Connected as player "<< player_id  << " out of "<< num_player<<" total players\n";
    if(player_id==0&&num_player>1){
        listener=build_listen_socket("");
        listen_port = get_port(listener);
        send(master_fd,&listen_port,sizeof(listen_port),0);
        right_fd=player_accept(listener);
        select_ports.push_back(right_fd);
        if(num_player>2){
            try_recv(master_fd,&left_player,sizeof(left_player),0);
            char left_port[10];
            sprintf(left_port,"%u",left_player.port);
            left_fd=request_connection(left_player.hostname,left_port);
            select_ports.push_back(left_fd);
        } 
    }
    else if(player_id>0){
        try_recv(master_fd,&left_player,sizeof(left_player),0);
        char left_port[10];
        sprintf(left_port,"%u",left_player.port);
        left_fd=request_connection(left_player.hostname,left_port);
        select_ports.push_back(left_fd);
        if(num_player>2){
            listener=build_listen_socket("");
            listen_port = get_port(listener);
            send(master_fd,&listen_port,sizeof(listen_port),0);
            right_fd=player_accept(listener);
            select_ports.push_back(right_fd);
        }
    }
    
    for(int i=0;i<select_ports.size();i++){
        close(select_ports[i]);
    }
    if(player_id<num_player-1||num_player>2){
        close(listener);
    }
    
    return EXIT_SUCCESS;
}