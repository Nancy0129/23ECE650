#include "potato.h"
using namespace std;
int main(int argc, char *argv[]){
    if (argc < 4) {
        cout << "Syntax: ringmaster <port_num> <num_players> <num_hops>" << endl;
        return EXIT_FAILURE;
    }
    const char *port=argv[1];
    int num_players= atoi(argv[2]);
    if(num_players<=1){
        cout<<"The number of players must larger than 1\n";
        return EXIT_FAILURE;
    }
    int num_hops=atoi(argv[3]);  
    if(num_hops>512){
        cout<<"The number of hops should not larger than 512\n";
        return EXIT_FAILURE;
    }
    cout<<"Potato Ringmaster\n";
    cout<<"Players = "<<num_players<<endl; 
    cout<<"Hops = " << num_hops <<endl;
    int listener=build_listen_socket(port);
    vector<Player_info> player_list;
    if(num_players>0){
        int ind=0;
        Player_info player0 = master_accpet(listener);
        send(player0.socket_fd, &ind, sizeof(ind), 0);
        send(player0.socket_fd, &num_players, sizeof(num_players), 0);
        try_recv(player0.socket_fd, &player0.port, sizeof(player0.port), 0);
        player_list.push_back(player0);    
    
        for (int i=1;i<num_players;i++){
            Player_info player = master_accpet(listener);
            send(player.socket_fd, &i, sizeof(i), 0);
            send(player.socket_fd, &num_players, sizeof(num_players), 0);
            send(player.socket_fd,&(player_list[i-1]), sizeof(player_list[i-1]),0);
            try_recv(player.socket_fd, &player.port, sizeof(player.port), 0);
            player_list.push_back(player);    
            if(i>1){
                cout<<"Player "<<i-1<<" is ready to play\n";
            }
        }
        if(num_players>2){//connect head and tail
            send(player_list[0].socket_fd, &(player_list[num_players-1]), sizeof(player_list[num_players-1]), 0);
            cout<<"Player "<<0<<" is ready to play\n";
            cout<<"Player "<<num_players-1<<" is ready to play\n";
        }
    }
    
    for(int i=0;i<player_list.size();i++){
        close(player_list[i].socket_fd);
    }
    close(listener);
    return EXIT_SUCCESS;
}