#include "potato.h"
using namespace std;

int main(int argc, char *argv[]){
    if (argc < 3) {
        cout << "Syntax: player <machine_name> <port_num>\n" << endl;
        return EXIT_FAILURE;
    }

    //Begin connection part
    const char *hostname = argv[1];
    const char *port = argv[2];
    char self_hostname[512];
    if(gethostname(self_hostname, sizeof(self_hostname))==-1){
        cerr<<"Can not get host name correctly!\n";
        return EXIT_FAILURE;
    }
    // cout<<self_hostname<<endl;
    int player_id;
    int num_player;
    vector<int> select_ports;
    Player_info left_player;
    int listener,left_fd, right_fd;
    int master_fd = request_connection(hostname,port);
    // cout<<"Connect success!\n";
    select_ports.push_back(master_fd);
    unsigned short int listen_port;
    try_recv(master_fd, &player_id, sizeof(player_id), MSG_WAITALL);
    try_recv(master_fd, &num_player, sizeof(num_player), MSG_WAITALL);
    cout<<"Connected as player "<< player_id  << " out of "<< num_player<<" total players\n";
    if(player_id==0){
        listener=build_listen_socket("");
        listen_port = get_port(listener);
        send(master_fd,&self_hostname,sizeof(self_hostname),0);
        send(master_fd,&listen_port,sizeof(listen_port),0);
        right_fd=player_accept(listener);
        select_ports.push_back(right_fd);
        if(num_player>2){
            try_recv(master_fd,&left_player,sizeof(left_player),MSG_WAITALL);
            char left_port[10];
            sprintf(left_port,"%hu",left_player.port);
            left_fd=request_connection(left_player.hostname,left_port);
            select_ports.push_back(left_fd);
        } 
        else{
          left_fd=  right_fd;
        }
    }
    else{
        try_recv(master_fd,&left_player,sizeof(left_player),MSG_WAITALL);
        char left_port[10];
        sprintf(left_port,"%hu",left_player.port);
        left_fd=request_connection(left_player.hostname,left_port);
        select_ports.push_back(left_fd);
        if(num_player>2){
            listener=build_listen_socket("");
            listen_port = get_port(listener);
            send(master_fd,&self_hostname,sizeof(self_hostname),0);
            send(master_fd,&listen_port,sizeof(listen_port),0);
            right_fd=player_accept(listener);
            select_ports.push_back(right_fd);
        }
        else{
            right_fd=left_fd;
        }
    }

    //Begin select part
    srand((unsigned int)time(NULL)+player_id);
    Potato potato;
    memset(&potato, 0, sizeof(potato));
    fd_set potato_fds;    
    int max_num = -1;    
    int status,is_close;
    is_close=-1;
    // cout<<select_ports.size()<<endl;
    while(true){
        // memset(&potato, 0, sizeof(potato));
        FD_ZERO(&potato_fds);
        for(int i=0;i<select_ports.size();i++){
            max_num=max_num>select_ports[i] ? max_num : select_ports[i];
            FD_SET(select_ports[i], &potato_fds);
        }        
        status=select(max_num+1,&potato_fds,NULL,NULL,NULL);
        if(status<=0){
            cerr << "Error: select error occurs"<< endl;
            exit(EXIT_FAILURE);
        }
        for(int i=0;i<select_ports.size();i++){
            if (FD_ISSET(select_ports[i], &potato_fds)){
                is_close=try_recv(select_ports[i],&potato,sizeof(potato),MSG_WAITALL);
                break;
            }
        }
        if(is_close==0){
            break;
        }
        // cout<<"num: "<<potato.curr_num<<endl;
        // cout<<"hop: "<<potato.num_hops<<endl;
        if(potato.num_hops<=0){
            cout<<"invalid hop num!\n";
            break;
        }        
        else if(potato.num_hops==1){
            cout<<"I'm it\n";
            potato.num_hops--;
            potato.trace[potato.curr_num]=player_id;
            potato.curr_num++;
            send(master_fd,&potato,sizeof(potato),0);
            // break;
        }
        else{
            potato.num_hops--;
            potato.trace[potato.curr_num]=player_id;
            potato.curr_num++;
            // int random = 1;
            int random = rand() % 2;
            if(random==0){//Pass to the left
                cout<<"Sending potato to "<< (player_id-1+num_player)%num_player <<endl;
                send(left_fd,&potato,sizeof(potato),0); 
            }
            else{//Pass to the right
                cout<<"Sending potato to "<< (player_id+1)%num_player <<endl;
                send(right_fd,&potato,sizeof(potato),0);
            }
        }       
    }

    //Begian close part
    for(int i=0;i<select_ports.size();i++){
        close(select_ports[i]);
    }
    if(player_id<num_player-1||num_player>2){
        close(listener);
    }
    
    return EXIT_SUCCESS;
}