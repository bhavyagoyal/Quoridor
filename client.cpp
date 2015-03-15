#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <bits/stdc++.h>

using namespace std;
/* Complete the function below to print 1 integer which will be your next move 
   */
#define FOR(i,n) for(int i=0;i<n;i++)

struct pos{
    int x;
    int y;
    pos(){};
    pos(int a, int b) : x(a), y(b){};
    bool operator<( const pos & d ) const {
        if(x==d.x){
            return y<d.y;
        }
        return  x< d.x;//this is for max-heap, reverse this for min heap
    }

};


struct gamestate{
    pos self;
    pos opp;
    // set< pos > wallsv;
    // set< pos > wallsh;
    gamestate(){};
    gamestate(const gamestate& b) : self(b.self), opp(b.opp){};
};

int N,M,K, time_left, player;
gamestate current;
int **walls;

int minVal(gamestate a,int alpha,int beta, int depth);

int maxVal(gamestate a,int alpha,int beta,int depth){
    // cout<<"get"<<endl;
    if(a.self.x==N){
        return (a.opp.x-1)+(a.self.x-N);
    }
    if(depth>=2){
        return (a.opp.x-1)+(a.self.x-N);
    }
    int child;
    int maxchild=INT_MIN;
    if(a.self.x<N && (a.opp.x!=(a.self.x+1) || a.opp.y!=a.self.y) && (walls[a.self.x+1][a.self.y]!=2) && (walls[a.self.x+1][a.self.y+1]!=2)){
        // cout <<"Here"<<endl;
        gamestate b = gamestate(a);
        b.self.x=a.self.x+1;
        child = minVal(b,alpha,beta,depth+1);
        alpha = max(alpha,child);
        if(alpha>=beta){
            return child;
        }
        maxchild = max(maxchild,child);
    }
    if(a.self.x>1 && (a.opp.x!=(a.self.x-1) || a.opp.y!=a.self.y) && (walls[a.self.x][a.self.y]!=2) && (walls[a.self.x][a.self.y+1]!=2)){
        gamestate b = gamestate(a);
        b.self.x=a.self.x-1;
        child = minVal(b,alpha,beta,depth+1);
        alpha = max(alpha,child);
        if(alpha>=beta){
            return child;
        }
        maxchild = max(maxchild,child);
    }
    if(a.self.y<M && (a.opp.y!=(a.self.y+1) || a.opp.x!=a.self.x) && (walls[a.self.x][a.self.y+1]!=1) && (walls[a.self.x+1][a.self.y+1]!=1)){
        gamestate b = gamestate(a);
        b.self.y=a.self.y+1;
        child = minVal(b,alpha,beta,depth+1);
        alpha = max(alpha,child);
        if(alpha>=beta){
            return child;
        }
        maxchild = max(maxchild,child);
    }
    if(a.self.y>1 && (a.opp.y!=(a.self.y-1) || a.opp.x!=a.self.x) && (walls[a.self.x][a.self.y]!=1) && (walls[a.self.x+1][a.self.y]!=1)){
        gamestate b = gamestate(a);
        b.self.y=a.self.y-1;
        child = minVal(b,alpha,beta,depth+1);
        alpha = max(alpha,child);
        if(alpha>=beta){
            return child;
        }
        maxchild = max(maxchild,child);
    }
    if(a.self.x<N-1 && (a.opp.x==(a.self.x+1) || a.opp.y==a.self.y) && (walls[a.self.x+1][a.self.y]!=2) && (walls[a.self.x+1][a.self.y+1]!=2) && (walls[a.self.x+2][a.self.y]!=2) && (walls[a.self.x+2][a.self.y+1]!=2)){
        gamestate b = gamestate(a);
        b.self.x=a.self.x+2;
        child = minVal(b,alpha,beta,depth+1);
        alpha = max(alpha,child);
        if(alpha>=beta){
            return child;
        }
        maxchild = max(maxchild,child);
    }
    if(a.self.x>2 && (a.opp.x==(a.self.x-1) || a.opp.y==a.self.y) && (walls[a.self.x][a.self.y]!=2) && (walls[a.self.x][a.self.y+1]!=2) && (walls[a.self.x-1][a.self.y]!=2) && (walls[a.self.x-1][a.self.y+1]!=2)){
        gamestate b = gamestate(a);
        b.self.x=a.self.x-2;
        child = minVal(b,alpha,beta,depth+1);
        alpha = max(alpha,child);
        if(alpha>=beta){
            return child;
        }
        maxchild = max(maxchild,child);
    }
    if(a.self.y<M-1 && (a.opp.y==(a.self.y+1) || a.opp.x==a.self.x) && (walls[a.self.x][a.self.y+1]!=1) && (walls[a.self.x+1][a.self.y+1]!=1) && (walls[a.self.x][a.self.y+2]!=1) && (walls[a.self.x+1][a.self.y+2]!=1)){
        gamestate b = gamestate(a);
        b.self.y=a.self.y+2;
        child = minVal(b,alpha,beta,depth+1);
        alpha = max(alpha,child);
        if(alpha>=beta){
            return child;
        }
        maxchild = max(maxchild,child);
    }
    if(a.self.y>2 && (a.opp.y==(a.self.y-1) || a.opp.x==a.self.x) && (walls[a.self.x][a.self.y]!=1) && (walls[a.self.x+1][a.self.y]!=1) && (walls[a.self.x][a.self.y-1]!=1) && (walls[a.self.x+1][a.self.y-1]!=1)){
        gamestate b = gamestate(a);
        b.self.y=a.self.y-2;
        child = minVal(b,alpha,beta,depth+1);
        alpha = max(alpha,child);
        if(alpha>=beta){
            return child;
        }
        maxchild = max(maxchild,child);
    }
    if(a.self.x<N && (a.opp.x==(a.self.x+1) || a.opp.y==a.self.y) && (walls[a.self.x+1][a.self.y]!=2) && (walls[a.self.x+1][a.self.y+1]!=2) && ((walls[a.self.x+2][a.self.y]!=2) || (walls[a.self.x+2][a.self.y+1]!=2))){
        if( (a.self.y<M) && (walls[a.self.x+1][a.self.y+1]!=1) && (walls[a.self.x+2][a.self.y+1]!=1)){
            gamestate b = gamestate(a);
            b.self.x=a.self.x+1;
            b.self.y=a.self.y+1;
            child = minVal(b,alpha,beta,depth+1);
            alpha = max(alpha,child);
            if(alpha>=beta){
                return child;
            }
            maxchild = max(maxchild,child);
        }
        if( a.self.y>1 && (walls[a.self.x+1][a.self.y]!=1) && (walls[a.self.x+2][a.self.y]!=1)){
            gamestate b = gamestate(a);
            b.self.x=a.self.x+1;
            b.self.y=a.self.y-1;            
            child = minVal(b,alpha,beta,depth+1);
            alpha = max(alpha,child);
            if(alpha>=beta){
                return child;
            }
            maxchild = max(maxchild,child);
        }
    }

    //jump condition
    //wall block jump
    //add wall //can reduce the depth for the case of walls
    for(int i=2;i<=M;i++){
        for(int j=2;j<=N;j++){
            if(walls[i+1][j]==0 && walls[i-1][j]==0 && walls[i][j]==0){
                walls[i][j]=1;
                gamestate b = gamestate(a);
                child = minVal(b,alpha,beta,depth+1);
                alpha = max(alpha,child);
                if(alpha>=beta){
                    return child;
                }
                maxchild = max(maxchild,child);
                walls[i][j]=0;
            }
            if(walls[i][j+1]==0 && walls[i][j-1]==0 && walls[i][j]==0){
                walls[i][j]=2;
                gamestate b = gamestate(a);
                child = minVal(b,alpha,beta,depth+1);
                alpha = max(alpha,child);
                if(alpha>=beta){
                    return child;
                }
                maxchild = max(maxchild,child);
                walls[i][j]=0;
            }
        }
    }
    cout<<"M "<<maxchild<<endl;
    return maxchild;
}

int minVal(gamestate a,int alpha, int beta,int depth){
    // cout<<"just "<<endl;
    if(a.opp.x==1){
        return (a.opp.x-1)+(a.self.x-N);
    }
    if(depth>=2){
        return (a.opp.x-1)+(a.self.x-N);
    }
    int child;
    int minchild=INT_MAX;
    if(a.opp.x<N && ((a.self.x!=(a.opp.x+1)) || (a.self.y!=a.opp.y)) && (walls[a.opp.x+1][a.opp.y]!=2) && (walls[a.opp.x+1][a.opp.y+1]!=2)){
        // cout<<"entered"<<endl;
        gamestate b = gamestate(a);
        b.opp.x=a.opp.x+1;
        child = maxVal(b,alpha,beta,depth+1);
        beta = min(beta,child);
        if(alpha>=beta){
            return child;
        }
        minchild = min(minchild,child);
    }
    // cout<<"min till now1"<<minchild<<endl;
    if(a.opp.x>1 && (a.self.x!=(a.opp.x-1) || a.self.y!=a.opp.y) && (walls[a.opp.x][a.opp.y]!=2) && (walls[a.opp.x][a.opp.y+1]!=2)){
        gamestate b = gamestate(a);
        b.opp.x=a.opp.x-1;
        child = maxVal(b,alpha,beta,depth+1);
        beta = min(beta,child);
        if(alpha>=beta){
            return child;
        }
        minchild = min(minchild,child);
    }
    // cout<<"min till now2"<<minchild<<endl;
    if(a.opp.y<M && (a.self.y!=(a.opp.y+1) || a.self.x!=a.opp.x) && (walls[a.opp.x][a.opp.y+1]!=1) && (walls[a.opp.x+1][a.opp.y+1]!=1)){
        gamestate b = gamestate(a);
        b.opp.y=a.opp.y+1;
        child = maxVal(b,alpha,beta,depth+1);
        beta = min(beta,child);
        if(alpha>=beta){
            return child;
        }
        minchild = min(minchild,child);
    }
    // cout<<"min till now3"<<minchild<<endl;
    if(a.opp.y>1 && (a.self.y!=(a.opp.y-1) || a.self.x!=a.opp.x) && (walls[a.opp.x][a.opp.y]!=1) && (walls[a.opp.x+1][a.opp.y]!=1)){
        gamestate b = gamestate(a);
        b.opp.y=a.opp.y-1;
        child = maxVal(b,alpha,beta,depth+1);
        beta = min(beta,child);
        if(alpha>=beta){
            return child;
        }
        minchild = min(minchild,child);
    }
    // cout<<"min till now4"<<minchild<<endl;
    //jump condition
    //wall block jump
    //add wall
    for(int i=2;i<=M;i++){
        for(int j=2;j<=N;j++){
            if(walls[i+1][j]==0 && walls[i-1][j]==0 && walls[i][j]==0){
                walls[i][j]=1;
                gamestate b = gamestate(a);
                child = maxVal(b,alpha,beta,depth+1);
                beta = min(beta,child);
                if(alpha>=beta){
                    return child;
                }
                minchild = min(minchild,child);
                walls[i][j]=0;
            }
            if(walls[i][j+1]==0 && walls[i][j-1]==0 && walls[i][j]==0){
                walls[i][j]=2;
                gamestate b = gamestate(a);
                child = maxVal(b,alpha,beta,depth+1);
                beta = min(beta,child);
                if(alpha>=beta){
                    return child;
                }
                minchild = min(minchild,child);
                walls[i][j]=0;
            }
//            if()
        }
    }
    cout<<"N "<<minchild<<endl;
    return minchild;
}



void printgs(gamestate a){
    cout<<"Self "<<a.self.x<<" "<<a.self.y<<endl;
    cout<<"Opp "<<a.opp.x<<" "<<a.opp.y<<endl;
    cout<<"Walls ";
    FOR(i,N+2){
        FOR(j,M+2){
            cout<<walls[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"done"<<endl;
    cout << "Maxval "<<maxVal(a,INT_MIN,INT_MAX,0)<<endl;
    // cout<<"\n";
}



// std::vector< gamestate > successor(gamestate a){
//     std::vector<gamestate> v;
//     std::set<pos>::iterator wall1; 
//     std::set<pos>::iterator wall2; 
//     if(a.self.x<N && (a.opp.x!=(a.self.x+1) || a.opp.y!=a.self.y) ){
//         wall1 = a.wallsh.find(pos(a.self.x,a.self.y+1));
//         wall2 = a.wallsh.find(pos(a.self.x+1,a.self.y+1));
//         if(wall1==a.wallsh.end() && wall2==a.wallsh.end()){
//             gamestate b = gamestate(a);
//             b.wallsh = a.wallsh;
//             b.wallsv = a.wallsv;
//             b.self.x=a.self.x+1;
//             v.push_back(b);
//         }
//     }
//     if(a.self.x>1 && (a.opp.x!=(a.self.x-1) || a.opp.y!=a.self.y) ){
//         wall1 = a.wallsh.find(pos(a.self.x,a.self.y));
//         wall2 = a.wallsh.find(pos(a.self.x+1,a.self.y));
//         if(wall1==a.wallsh.end() && wall2==a.wallsh.end()){
//             gamestate b = gamestate(a);
//             b.wallsh = a.wallsh;
//             b.wallsv = a.wallsv;
//             b.self.x=a.self.x-1;
//             v.push_back(b);
//         }
//     }
//     if(a.self.y>1 && ((a.opp.y!=(a.self.y-1)) || (a.opp.x!=a.self.x)) ){
//         wall1 = a.wallsv.find(pos(a.self.x,a.self.y));
//         wall2 = a.wallsv.find(pos(a.self.x+1,a.self.y));
//         if(wall1==a.wallsv.end() && wall2==a.wallsv.end()){
//             gamestate b = gamestate(a);
//             b.wallsv = a.wallsv;
//             b.wallsh = a.wallsh;
//             b.self.y = a.self.y-1;
//             v.push_back(b);
//         }
//     }
//     if(a.self.y<M && (a.opp.y!=(a.self.y+1) || a.opp.x!=a.self.x) ){
//         wall1 = a.wallsv.find(pos(a.self.x,a.self.y+1));
//         wall2 = a.wallsv.find(pos(a.self.x+1,a.self.y+1));
//         if(wall1==a.wallsv.end() && wall2==a.wallsv.end()){
//             gamestate b = gamestate(a);
//             b.wallsv = a.wallsv;
//             b.wallsh = a.wallsh;
//             b.self.y = a.self.y+1;
//             v.push_back(b);
//         }
//     }
//     return v;
// }


int main(int argc, char *argv[])
{
    srand (time(NULL));
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    char sendBuff[1025];
    struct sockaddr_in serv_addr; 

    if(argc != 3)
    {
        printf("\n Usage: %s <ip of server> <port no> \n",argv[0]);
        return 1;
    } 

    
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2])); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

    cout<<"Quoridor will start..."<<endl;

    memset(recvBuff, '0',sizeof(recvBuff));
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    recvBuff[n] = 0;
    sscanf(recvBuff, "%d %d %d %d %d", &player, &N, &M, &K, &time_left);


    cout<<"Player "<<player<<endl;
    cout<<"Time "<<time_left<<endl;
    cout<<"Board size "<<N<<"x"<<M<<" :"<<K<<endl;
    float TL;
    int om,oro,oc;
    int m,r,c;
    int d=3;
    char s[100];
    int x=1;
    walls = (int **)malloc(sizeof(int *)*(N+2));
    for(int i=0;i<N+2;i++)
    {   
        walls[i]=(int *)malloc(sizeof(int)*(M+2));
    }

    if(player == 1)
    {
        current.opp = pos(N,(M+1)/2);
        current.self = pos(1,(M+1)/2);
        // current->self = pos(1,5);
        // current->opp = pos(9,5);                
        memset(sendBuff, '0', sizeof(sendBuff)); 
        string temp;
    // cin>>m>>r>>c;
        m=0; r=2;c=5;
        current.self = pos(r,c);
        // printgs(*current);
        snprintf(sendBuff, sizeof(sendBuff), "%d %d %d", m, r , c);
        write(sockfd, sendBuff, strlen(sendBuff));

        memset(recvBuff, '0',sizeof(recvBuff));
        n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
        recvBuff[n] = 0;
        sscanf(recvBuff, "%f %d", &TL, &d);
        cout<<TL<<" "<<d<<endl;
        if(d==1)
        {
            cout<<"You win!! Yayee!! :D ";
            x=0;
        }
        else if(d==2)
        {
            cout<<"Loser :P ";
            x=0;
        }
    }
    printgs(current);
    while(x)
    {
        memset(recvBuff, '0',sizeof(recvBuff));
        n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
        recvBuff[n] = 0;
        sscanf(recvBuff, "%d %d %d %d", &om,&oro,&oc,&d);
        cout << om<<" "<<oro<<" "<<oc << " "<<d<<endl;
        if(d==1)
        {
            cout<<"You win!! Yayee!! :D ";
            break;
        }
        else if(d==2)
        {
            cout<<"Loser :P ";
            break;
        }
        if(om==0){
            current.opp = pos(oro,oc);
        }
        else if(om==1){
            walls[oro][oc]=2;
        }
        else{
            walls[oro][oc]=1;
        }
        printgs(current);

            memset(sendBuff, '0', sizeof(sendBuff)); 
            string temp;
            // std::vector<gamestate> v = successor(*current);
            // cout<<"successors "<<endl;
            // cout<<v.size()<<endl;
            // for(std::vector<gamestate>::iterator it = v.begin(); it != v.end(); ++it) {
            //     printgs(*it);
            // }
            // cout<<"done"<<endl;
            cin>>m>>r>>c;
            snprintf(sendBuff, sizeof(sendBuff), "%d %d %d", m, r , c);
            write(sockfd, sendBuff, strlen(sendBuff));

            memset(recvBuff, '0',sizeof(recvBuff));
            n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
            recvBuff[n] = 0;
            sscanf(recvBuff, "%f %d", &TL, &d);//d=3 indicates game continues.. d=2 indicates lost game, d=1 means game won.
            cout<<TL<<" "<<d<<endl;
            if(d==1)
            {
                cout<<"You win!! Yayee!! :D ";
                break;
            }
            else if(d==2)
            {
                cout<<"Loser :P ";
                break;
            }
            if(m==0){
                cout<<"h1"<<endl;
                current.self = pos(r,c);
            }
            else if(m==1){
                walls[r][c] = 2;
                cout<<"h2"<<endl;
            }
            else{
                cout<<"h3"<<endl;
                walls[r][c] = 1;
            }
            printgs(current);
    }
    cout<<endl<<"The End"<<endl;
    return 0;
}
















