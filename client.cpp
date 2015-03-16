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
    int wallself;
    int wallopp;
    // set< pos > wallsv;
    // set< pos > wallsh;
    gamestate(){};
    gamestate(const gamestate& b) : self(b.self), opp(b.opp), wallself(b.wallself), wallopp(b.wallopp){};
};

struct node{
    pos p;
    int cost;
    bool operator<( const node& d ) const{
        return cost<d.cost;
    }
};

int N, M, K;
int time_left, player;
gamestate current;
int **walls;
vector< node > adjList;
vector<node> goal,goal2;
int global_min = INT_MAX;
vector< int > path;
int adjMatrix[82][82];
int tempadjMatrix[82][82];

bool inGoal(pos p, int self){
    return (self==1)?(p.x == N):(p.x == 1);
}

void search_init(){
    memcpy(tempadjMatrix, adjMatrix, sizeof (int) * (M*N+1)*(M*N+1));
    for(int i=1; i <= N; i++){
        for(int j=1;j<= M;j++){
            if(walls[i][j]==1){ 
                tempadjMatrix[(i-1)*M+j][(i-1)*M+j-1]=0;
                tempadjMatrix[(i-1)*M+j-1][(i-1)*M+j]=0;
                tempadjMatrix[(i-2)*M+j][(i-2)*M+j-1]=0;
                tempadjMatrix[(i-2)*M+j-1][(i-2)*M+j]=0;
            }
            if(walls[i][j]==2){
                    tempadjMatrix[(i-1)*M+j][(i-2)*M+j]=0;
                    tempadjMatrix[(i-2)*M+j][(i-1)*M+j]=0;
                    tempadjMatrix[(i-1)*M+j-1][(i-2)*M+j-1]=0;
                    tempadjMatrix[(i-2)*M+j-1][(i-1)*M+j-1]=0;
            }
        }
    }

}


void search(vector<int> explored, int path_length ,pos start, int self){
    // start.visited =true;
    // cout<<"pos= "<<(start.p.y-1)*m+start.p.x-1<<endl;
    explored[(start.x-1)*M+start.y-1]=1;
    if(inGoal(start,self)){
        // cout<<"Goal"<<endl;
        if(global_min> path_length){
            global_min = path_length;
        }
        // path.push_back(path_length);        
    }
    else{
        // cout<<"Not Goal "<<start.adj.size()<<endl;

            priority_queue<node , vector<node> > child_heap ;
            for(int i=1;i<=M*N;i++){
                if(tempadjMatrix[(start.x-1)*M+start.y][i]!=0){

                    pos p(((i-1)/M) + 1,((i-1)%M)+1);
                    if(path_length+1+abs((N-1)*self+1-p.x)<global_min)
                    {
                        node child;
                        child.p = p;
                        child.cost = path_length+1+abs((N-1)*self+1-p.x);
                        child_heap.push(child);
                    }
                }
                
            }
            while(!child_heap.empty())
            {
                node child = child_heap.top();
                child_heap.pop();
                pos p=child.p;
                // cout<<"new Pos= "<<(p.y-1)*m+p.x-1<<endl;
                if(explored[(p.x-1)*M+p.y-1]!=1){
                    if(global_min> path_length+1 + abs((N-1)*self+1-p.x)){
                    // cout<<"new Pos searched= "<<(p.y-1)*m+p.x-1<<endl;
                        search(explored,path_length+1,p,self);
                    }
                }
            }
        
    }
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
    // cout << "Maxval "<<maxVal(a,INT_MIN,INT_MAX,0)<<endl;
    // cout<<"\n";
}

int utility(gamestate a){
    //printgs(a);
    search_init();
    global_min = INT_MAX;
    vector<int> explored(M*N,0);
    search(explored,0,a.self,1);
    // return global_min;
    int minpathself = global_min;
    global_min = INT_MAX;
    std::fill(explored.begin(), explored.end(), 0);
    search(explored,0,a.opp,0);
    //cout<<"Utitlity "<<global_min<<" "<<minpathself<<endl;
    return global_min-minpathself;
}


// int minpathself(gamestate a){
//     // search_init();
//     global_min = INT_MAX;
//     vector<int> explored(M*N,0);
//     search(explored,0,a.self,1);
//     return global_min;
// }

// int minpathopp(gamestate a){
//     // search_init();
//     global_min = INT_MAX;
//     vector<int> explored(M*N,0);
//     search(explored,0,a.self,1);
//     return global_min;
// }



int minVal(gamestate a,int alpha,int beta, int depth);

int maxVal(gamestate a,int alpha,int beta,int depth){
    // search_init();
    // cout<<"get"<<endl;
    if(a.self.x==N){
        return utility(a);
    }
    if(depth>=2){
        return utility(a);
    }
    int child;
    int maxchild=INT_MIN;
    gamestate temp;
    if(a.self.x<N && (a.opp.x!=(a.self.x+1) || a.opp.y!=a.self.y) && (walls[a.self.x+1][a.self.y]!=2) && (walls[a.self.x+1][a.self.y+1]!=2)){
        cout <<"Here"<<endl;
        gamestate b = gamestate(a);
        b.self.x=a.self.x+1;
        child = minVal(b,alpha,beta,depth+1);
        alpha = max(alpha,child);
        cout<<"INside "<<child<<endl;
        if(maxchild<child){
            maxchild=child;
            temp=b;
        }
        if(alpha>=beta){
            return child;
        }
        // maxchild = max(maxchild,child);
    }
    if(a.self.x>1 && (a.opp.x!=(a.self.x-1) || a.opp.y!=a.self.y) && (walls[a.self.x][a.self.y]!=2) && (walls[a.self.x][a.self.y+1]!=2)){
        gamestate b = gamestate(a);
        b.self.x=a.self.x-1;
        child = minVal(b,alpha,beta,depth+1);
        alpha = max(alpha,child);
        if(alpha>=beta){
            return child;
        }
        if(maxchild<child){
            maxchild=child;
            temp=b;
        }
    }
    if(a.self.y<M && (a.opp.y!=(a.self.y+1) || a.opp.x!=a.self.x) && (walls[a.self.x][a.self.y+1]!=1) && (walls[a.self.x+1][a.self.y+1]!=1)){
        gamestate b = gamestate(a);
        b.self.y=a.self.y+1;
        child = minVal(b,alpha,beta,depth+1);
        alpha = max(alpha,child);
        if(alpha>=beta){
            return child;
        }
        if(maxchild<child){
            maxchild=child;
            temp=b;
        }
    }
    if(a.self.y>1 && (a.opp.y!=(a.self.y-1) || a.opp.x!=a.self.x) && (walls[a.self.x][a.self.y]!=1) && (walls[a.self.x+1][a.self.y]!=1)){
        gamestate b = gamestate(a);
        b.self.y=a.self.y-1;
        child = minVal(b,alpha,beta,depth+1);
        alpha = max(alpha,child);
        if(alpha>=beta){
            return child;
        }
        if(maxchild<child){
            maxchild=child;
            temp=b;
        }
    }
    if(a.self.x<N-1 && (a.opp.x==(a.self.x+1) && a.opp.y==a.self.y) && (walls[a.self.x+1][a.self.y]!=2) && (walls[a.self.x+1][a.self.y+1]!=2) && (walls[a.self.x+2][a.self.y]!=2) && (walls[a.self.x+2][a.self.y+1]!=2)){
        gamestate b = gamestate(a);
        b.self.x=a.self.x+2;
        child = minVal(b,alpha,beta,depth+1);
        alpha = max(alpha,child);
        if(alpha>=beta){
            return child;
        }
        if(maxchild<child){
            maxchild=child;
            temp=b;
        }
    }
    if(a.self.x>2 && (a.opp.x==(a.self.x-1) && a.opp.y==a.self.y) && (walls[a.self.x][a.self.y]!=2) && (walls[a.self.x][a.self.y+1]!=2) && (walls[a.self.x-1][a.self.y]!=2) && (walls[a.self.x-1][a.self.y+1]!=2)){
        gamestate b = gamestate(a);
        b.self.x=a.self.x-2;
        child = minVal(b,alpha,beta,depth+1);
        alpha = max(alpha,child);
        if(alpha>=beta){
            return child;
        }
        if(maxchild<child){
            maxchild=child;
            temp=b;
        }
    }
    if(a.self.y<M-1 && (a.opp.y==(a.self.y+1) && a.opp.x==a.self.x) && (walls[a.self.x][a.self.y+1]!=1) && (walls[a.self.x+1][a.self.y+1]!=1) && (walls[a.self.x][a.self.y+2]!=1) && (walls[a.self.x+1][a.self.y+2]!=1)){
        gamestate b = gamestate(a);
        b.self.y=a.self.y+2;
        child = minVal(b,alpha,beta,depth+1);
        alpha = max(alpha,child);
        if(alpha>=beta){
            return child;
        }
        if(maxchild<child){
            maxchild=child;
            temp=b;
        }
    }
    if(a.self.y>2 && (a.opp.y==(a.self.y-1) && a.opp.x==a.self.x) && (walls[a.self.x][a.self.y]!=1) && (walls[a.self.x+1][a.self.y]!=1) && (walls[a.self.x][a.self.y-1]!=1) && (walls[a.self.x+1][a.self.y-1]!=1)){
        gamestate b = gamestate(a);
        b.self.y=a.self.y-2;
        child = minVal(b,alpha,beta,depth+1);
        alpha = max(alpha,child);
        if(alpha>=beta){
            return child;
        }
        if(maxchild<child){
            maxchild=child;
            temp=b;
        }
    }
    if(a.self.x<N && (a.opp.x==(a.self.x+1) && a.opp.y==a.self.y) && (walls[a.self.x+1][a.self.y]!=2) && (walls[a.self.x+1][a.self.y+1]!=2) && ((walls[a.self.x+2][a.self.y]!=2) || (walls[a.self.x+2][a.self.y+1]!=2))){
        if( (a.self.y<M) && (walls[a.self.x+1][a.self.y+1]!=1) && (walls[a.self.x+2][a.self.y+1]!=1)){
            gamestate b = gamestate(a);
            b.self.x=a.self.x+1;
            b.self.y=a.self.y+1;
            child = minVal(b,alpha,beta,depth+1);
            alpha = max(alpha,child);
            if(alpha>=beta){
                return child;
            }
            if(maxchild<child){
                maxchild=child;
                temp=b;
            }
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
            if(maxchild<child){
                maxchild=child;
                temp=b;
            }
        }
    }
    bool bestwall=false;
    pos wallpos;
    bool vert;
    //jump condition
    //wall block jump
    //add wall //can reduce the depth for the case of walls
    for(int i=2;i<=N;i++){
        for(int j=2;j<=M;j++){
            cout<< i<< " "<<j<<endl;
            if(walls[i][j]==0 && walls[i-1][j]!=1 && walls[i+1][j]!=1){
                walls[i][j]=1;
                gamestate b = gamestate(a);
                child = minVal(b,alpha,beta,depth+1);
                alpha = max(alpha,child);
                walls[i][j]=0;
                if(alpha>=beta){
                    return child;
                }
                if(maxchild<child){
                    maxchild=child;
                    temp=b;
                    wallpos = pos(i,j);
                    bestwall=true;
                    vert=true;
                }
            }
            if(walls[i][j]==0 && walls[i][j-1]!=2 && walls[i][j+1]!=2){
                walls[i][j]=2;
                gamestate b = gamestate(a);
                child = minVal(b,alpha,beta,depth+1);
                alpha = max(alpha,child);
                walls[i][j]=0;
                if(alpha>=beta){
                    return child;
                }
                if(maxchild<child){
                    maxchild=child;
                    temp=b;
                    wallpos = pos(i,j);
                    bestwall=true;
                    vert=false;
                }
            }
        }
    }
    if(depth==0){
        cout<<"Best wall "<<bestwall<<endl;
        printgs(temp);
        cout<<"wall "<<wallpos.x<<" "<<wallpos.y<<endl;
        cout<<"Dir "<<vert<<endl;
        cout<<"M "<<maxchild<<endl;
    }
    return maxchild;
}

int minVal(gamestate a,int alpha, int beta,int depth){
    // cout<<"just "<<endl;
    if(a.opp.x==1){
        return utility(a);
    }
    if(depth>=2){
        // cout<<"Utility "<<minpathop<<" "<<minpathsel<<endl;
        return utility(a);
    }
    //     int minpathop = minpathopp(a);
    //     int minpathsel = minpathself(a);
    //     cout<<"Utility "<<minpathop<<" "<<minpathsel<<endl;
    //     return minpathop-minpathsel;
    // }
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
    for(int i=2;i<=N;i++){
        for(int j=2;j<=M;j++){
            if(walls[i][j]==0 && walls[i-1][j]!=1 && walls[i+1][j]!=1){
                walls[i][j]=1;
                gamestate b = gamestate(a);
                child = maxVal(b,alpha,beta,depth+1);
                beta = min(beta,child);
                walls[i][j]=0;
                if(alpha>=beta){
                    return child;
                }
                minchild = min(minchild,child);
            }
            if(walls[i][j]==0 && walls[i][j-1]!=2 && walls[i][j+1]!=2){
                walls[i][j]=2;
                gamestate b = gamestate(a);
                child = maxVal(b,alpha,beta,depth+1);
                beta = min(beta,child);
                walls[i][j]=0;
                if(alpha>=beta){
                    return child;
                }
                minchild = min(minchild,child);
                //walls[i][j]=0;
            }
//            if()
        }
    }
    // cout<<"N "<<minchild<<endl;
    return minchild;
}


void globalsearchinit(){
    memset(adjMatrix, 0, sizeof(int) * (M*N+1)*(N*M+1));
    // cerr<<"Got "<<endl;
    for(int i=1; i <= N; i++){
        for(int j=1;j<= M;j++){
            // cerr<<i<<" "<<j<<endl;
            int first = (i-1)*M+j;         
            if(j+1<=M){
                        adjMatrix[first][(i-1)*M+j+1]=1;
                        adjMatrix[(i-1)*M+j+1][first]=1;                       
                }
            // cerr<<i<<" "<<j<<endl;
                if(i-1>0){
                        adjMatrix[first][(i-2)*M+j]=1;
                        adjMatrix[(i-2)*M+j][first]=1;
                }
            // cerr<<i<<" "<<j<<endl;
                if(i+1 <=N){
                         adjMatrix[first][(i)*M+j]=1;
                         adjMatrix[(i)*M+j][first]=1;
                }
            // cerr<<i<<" "<<j<<endl;
                if(j-1>0){
                         adjMatrix[first][(i-1)*M+j-1]=1;
                         adjMatrix[(i-1)*M+j-1][first]=1;
                }
            // cerr<<i<<" "<<j<<endl;
            }
        }
}

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

    // adjMatrix = (int **)malloc(sizeof(int *)*(M*N+1));
    // for(int i=0;i< (M*N+1);i++)
    // {   
    //     adjMatrix[i]=(int *)malloc(sizeof(int)*(M*N+1));
    // }

    // tempadjMatrix = (int **)malloc(sizeof(int *)*(M*N+1));
    // for(int i=0;i< (M*N+1);i++)
    // {   
    //     tempadjMatrix[i]=(int *)malloc(sizeof(int)*(M*N+1));
    // }
    cerr<<"H"<<endl;
    globalsearchinit();
    cerr<<"I"<<endl;
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
            // cout<<"H1"<<endl;
        }
        else if(om==1){
            walls[oro][oc]=2;
            // cout<<"H2"<<endl;
        }
        else{
            walls[oro][oc]=1;
            // cout<<"H3"<<endl;
        }
        // printgs(current);
        maxVal(current,INT_MIN,INT_MAX,0);
        // printgs(current);

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
            maxVal(current,INT_MIN,INT_MAX,0);
            // printgs(current);
    }
    cout<<endl<<"The End"<<endl;
    return 0;
}
















