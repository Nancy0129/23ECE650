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
    //Build up connections
    int listener=build_listen_socket(port);
    vector<Player_info> player_list;
    if(num_players>0){
        int ind=0;
        Player_info player0 = master_accpet(listener);
        send(player0.socket_fd, &ind, sizeof(ind), 0);
        send(player0.socket_fd, &num_players, sizeof(num_players), 0);
        try_recv(player0.socket_fd, &player0.port, sizeof(player0.port), MSG_WAITALL);
        player_list.push_back(player0);    
    
        for (int i=1;i<num_players;i++){
            Player_info player = master_accpet(listener);
            send(player.socket_fd, &i, sizeof(i), 0);
            send(player.socket_fd, &num_players, sizeof(num_players), 0);
            send(player.socket_fd,&(player_list[i-1]), sizeof(player_list[i-1]),0);    
            if(num_players>2){
                try_recv(player.socket_fd, &player.port, sizeof(player.port), MSG_WAITALL);
            }     
            if(i>1){                
                cout<<"Player "<<i-1<<" is ready to play\n";
            }
            player_list.push_back(player);   
        }
        if(num_players>2){//connect head and tail
            send(player_list[0].socket_fd, &(player_list[num_players-1]), sizeof(player_list[num_players-1]), 0);
            cout<<"Player "<<0<<" is ready to play\n";
            cout<<"Player "<<num_players-1<<" is ready to play\n";
        }
        else{
            cout<<"Player "<<0<<" is ready to play\n";
            cout<<"Player "<<1<<" is ready to play\n";
        }
    }
    //Make a potato
    Potato potato;
    memset(&potato, 0, sizeof(potato));
    potato.curr_num=0;
    potato.num_hops=num_hops;
    // int random = 0;
    int random = rand() % num_players;

    //Pass the potato
    if(num_hops>0){        
        cout<<"Ready to start the game, sending potato to player "<< random <<endl;
        send(player_list[random].socket_fd, &potato, sizeof(potato),0);
        //Wait for potato
        fd_set player_fds;
        FD_ZERO(&player_fds);
        int max_num = -1;
        for(int i=0;i<player_list.size();i++){
            max_num=max_num>player_list[i].socket_fd ? max_num : player_list[i].socket_fd;
            FD_SET(player_list[i].socket_fd, &player_fds);
        }
        int status = select(max_num+1,&player_fds,NULL,NULL,NULL);
        if(status<=0){
            cerr<<"Error: select error occurs\n";
            exit(EXIT_FAILURE);
        }
        for(int i=0;i<player_list.size();i++){
            if(FD_ISSET(player_list[i].socket_fd, &player_fds)){
                // memset(&potato, 0, sizeof(potato));
                int is_close = try_recv(player_list[i].socket_fd,&potato,sizeof(potato),MSG_WAITALL);
                if(is_close==0){
                    cerr<<"Error: player "<< i <<" lose connection\n";
                    exit(EXIT_FAILURE);
                }
            }
        }
        //Make sure the number match expectation
        assert(potato.curr_num==num_hops);
        assert(potato.num_hops==0);

        //Show trace
        cout<<"Trace of potato:\n";
        for(int i=0;i<num_hops;i++){
            cout<<potato.trace[i];
            if(i<num_hops-1){
                cout<<",";
            }
            else{
                cout<<"\n";
            }
        }
    }
    
    //Close part
    for(int i=0;i<player_list.size();i++){
        close(player_list[i].socket_fd);
    }
    close(listener);
    return EXIT_SUCCESS;
}