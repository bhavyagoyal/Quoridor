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
        return  x< d.x;
    }
};

struct move{
    int mov;//This is opposite of what we have in code //0 for move //2 for wall vertical // 1 for wall horizontal
    pos posit;
    move(){};
    move(int a, pos b) : mov(a), posit(b){};
};

struct gamestate{
    pos self;
    pos opp;
    int wallself;
    int wallopp;
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
int global_min = INT_MAX;
bool *visited;
int gdepth=3;
int gameresult=3;
std::vector<int> ii;
std::vector<int> ij;
std::vector<int> ii2;
std::vector<int> ij2;
std::vector<int> ii3;
std::vector<int> ij3;

bool inGoal(pos p, int self){
    return ((player==1)?(self):(!self))?(p.x == N):(p.x == 1);
}

std::vector<pos> neighbour2(pos a, pos b){
    std::vector<pos> ans;
        if(a.x<N && (walls[a.x+1][a.y]!=2) && (walls[a.x+1][a.y+1]!=2)){
            ans.push_back(pos(a.x+1,a.y));
        }
        if(a.x>1 && (walls[a.x][a.y]!=2) && (walls[a.x][a.y+1]!=2)){
            ans.push_back(pos(a.x-1,a.y));
        }
        if(a.y<M && (walls[a.x][a.y+1]!=1) && (walls[a.x+1][a.y+1]!=1)){
            ans.push_back(pos(a.x,a.y+1));
        }
        if(a.y>1 && (walls[a.x][a.y]!=1) && (walls[a.x+1][a.y]!=1)){
            ans.push_back(pos(a.x,a.y-1));
        }
    return ans;
}


std::vector<pos> neighbour(pos a, pos b){
    std::vector<pos> ans;
    if(gameresult!=3){
        if(a.x<N && (walls[a.x+1][a.y]!=2) && (walls[a.x+1][a.y+1]!=2)){
            ans.push_back(pos(a.x+1,a.y));
        }
        if(a.x>1 && (walls[a.x][a.y]!=2) && (walls[a.x][a.y+1]!=2)){
            ans.push_back(pos(a.x-1,a.y));
        }
        if(a.y<M && (walls[a.x][a.y+1]!=1) && (walls[a.x+1][a.y+1]!=1)){
            ans.push_back(pos(a.x,a.y+1));
        }
        if(a.y>1 && (walls[a.x][a.y]!=1) && (walls[a.x+1][a.y]!=1)){
            ans.push_back(pos(a.x,a.y-1));
        }
    }
    else{
        if(a.x<N && (b.x!=(a.x+1) || b.y!=a.y) && (walls[a.x+1][a.y]!=2) && (walls[a.x+1][a.y+1]!=2)){
            ans.push_back(pos(a.x+1,a.y));
        }
        if(a.x>1 && (b.x!=(a.x-1) || b.y!=a.y) && (walls[a.x][a.y]!=2) && (walls[a.x][a.y+1]!=2)){
            ans.push_back(pos(a.x-1,a.y));
        }
        if(a.y<M && (b.y!=(a.y+1) || b.x!=a.x) && (walls[a.x][a.y+1]!=1) && (walls[a.x+1][a.y+1]!=1)){
            ans.push_back(pos(a.x,a.y+1));
        }
        if(a.y>1 && (b.y!=(a.y-1) || b.x!=a.x) && (walls[a.x][a.y]!=1) && (walls[a.x+1][a.y]!=1)){
            ans.push_back(pos(a.x,a.y-1));
        }
        if(a.x<N-1 && (b.x==(a.x+1) && b.y==a.y) && (walls[a.x+1][a.y]!=2) && (walls[a.x+1][a.y+1]!=2) && (walls[a.x+2][a.y]!=2) && (walls[a.x+2][a.y+1]!=2)){
            ans.push_back(pos(a.x+2,a.y));
        }
        if(a.x>2 && (b.x==(a.x-1) && b.y==a.y) && (walls[a.x][a.y]!=2) && (walls[a.x][a.y+1]!=2) && (walls[a.x-1][a.y]!=2) && (walls[a.x-1][a.y+1]!=2)){
            ans.push_back(pos(a.x-2,a.y));
        }
        if(a.y<M-1 && (b.y==(a.y+1) && b.x==a.x) && (walls[a.x][a.y+1]!=1) && (walls[a.x+1][a.y+1]!=1) && (walls[a.x][a.y+2]!=1) && (walls[a.x+1][a.y+2]!=1)){
            ans.push_back(pos(a.x,a.y+2));
        }
        if(a.y>2 && (b.y==(a.y-1) && b.x==a.x) && (walls[a.x][a.y]!=1) && (walls[a.x+1][a.y]!=1) && (walls[a.x][a.y-1]!=1) && (walls[a.x+1][a.y-1]!=1)){
            ans.push_back(pos(a.x,a.y-2));
        }
        if(a.x<N && (b.x==(a.x+1) && b.y==a.y) && (walls[a.x+1][a.y]!=2) && (walls[a.x+1][a.y+1]!=2) && ((walls[a.x+2][a.y]==2) || (walls[a.x+2][a.y+1]==2))){
            if( (a.y<M) && (walls[a.x+1][a.y+1]!=1) && (walls[a.x+2][a.y+1]!=1)){
                ans.push_back(pos(a.x+1,a.y+1));
            }
            if( a.y>1 && (walls[a.x+1][a.y]!=1) && (walls[a.x+2][a.y]!=1)){
                ans.push_back(pos(a.x+1,a.y-1));
            }
        }
        if(a.x>1 && (b.x==(a.x-1) && b.y==a.y) && (walls[a.x][a.y]!=2) && (walls[a.x][a.y+1]!=2) && ((walls[a.x-1][a.y]==2) || (walls[a.x-1][a.y+1]==2))){
            if( (a.y<M) && (walls[a.x][a.y+1]!=1) && (walls[a.x-1][a.y+1]!=1)){
                ans.push_back(pos(a.x-1,a.y+1));
            }
            if( a.y>1 && (walls[a.x][a.y]!=1) && (walls[a.x-1][a.y]!=1)){
                ans.push_back(pos(a.x-1,a.y-1));
            }
        }
        if(a.y<M && (b.y==(a.y+1) && b.x==a.x) && (walls[a.x][a.y+1]!=1) && (walls[a.x+1][a.y+1]!=1) && ((walls[a.x][a.y+2]==1) || (walls[a.x+1][a.y+2]==1))){
            if( (a.x<N) && (walls[a.x+1][a.y+1]!=2) && (walls[a.x+1][a.y+2]!=2)){
                ans.push_back(pos(a.x+1,a.y+1));
            }
            if( a.x>1 && (walls[a.x][a.y+1]!=2) && (walls[a.x][a.y+2]!=2)){
                ans.push_back(pos(a.x-1,a.y+1));
            }
        }
        if(a.y>1 && (b.y==(a.y-1) && b.x==a.x) && (walls[a.x][a.y]!=1) && (walls[a.x+1][a.y]!=1) && ((walls[a.x][a.y-1]==1) || (walls[a.x+1][a.y-1]==1))){
            if( (a.x<N) && (walls[a.x+1][a.y]!=2) && (walls[a.x+1][a.y-1]!=2)){
                ans.push_back(pos(a.x+1,a.y-1));
            }
            if( a.x>1 && (walls[a.x][a.y]!=2) && (walls[a.x][a.y-1]!=2)){
                ans.push_back(pos(a.x-1,a.y-1));
            }
        }
        if(a.x==(N-1) && (b.x==(a.x+1) && b.y==a.y) && (walls[a.x+1][a.y]!=2) && (walls[a.x+1][a.y+1]!=2) ){
            if( (a.y<M) && (walls[a.x+1][a.y+1]!=1) && (walls[a.x+2][a.y+1]!=1)){
                ans.push_back(pos(a.x+1,a.y+1));
            }
            if( a.y>1 && (walls[a.x+1][a.y]!=1) && (walls[a.x+2][a.y]!=1)){
                ans.push_back(pos(a.x+1,a.y-1));
            }
        }
        if(a.x==2 && (b.x==(a.x-1) && b.y==a.y) && (walls[a.x][a.y]!=2) && (walls[a.x][a.y+1]!=2)){
            if( (a.y<M) && (walls[a.x][a.y+1]!=1) && (walls[a.x-1][a.y+1]!=1)){
                ans.push_back(pos(a.x-1,a.y+1));
            }
            if( a.y>1 && (walls[a.x][a.y]!=1) && (walls[a.x-1][a.y]!=1)){
                ans.push_back(pos(a.x-1,a.y-1));
            }
        }
        if(a.y==M-1 && (b.y==(a.y+1) && b.x==a.x) && (walls[a.x][a.y+1]!=1) && (walls[a.x+1][a.y+1]!=1)){
            if( (a.x<N) && (walls[a.x+1][a.y+1]!=2) && (walls[a.x+1][a.y+2]!=2)){
                ans.push_back(pos(a.x+1,a.y+1));
            }
            if( a.x>1 && (walls[a.x][a.y+1]!=2) && (walls[a.x][a.y+2]!=2)){
                ans.push_back(pos(a.x-1,a.y+1));
            }
        }
        if(a.y==2 && (b.y==(a.y-1) && b.x==a.x) && (walls[a.x][a.y]!=1) && (walls[a.x+1][a.y]!=1)){
            if( (a.x<N) && (walls[a.x+1][a.y]!=2) && (walls[a.x+1][a.y-1]!=2)){
                ans.push_back(pos(a.x+1,a.y-1));
            }
            if( a.x>1 && (walls[a.x][a.y]!=2) && (walls[a.x][a.y-1]!=2)){
                ans.push_back(pos(a.x-1,a.y-1));
            }
        }
    }
    return ans;
}



int bfs(int start,int opp,int self){
    list<int> queue;
    memset(visited,false,sizeof(bool)*(M*N+1));
    visited[start] = true;
    queue.push_back(start);
    queue.push_back(-1);
    int level=0;
    // int count_debug=0;
    int i=0;
    while(!queue.empty()){
        // count_debug++;
        int root = queue.front();
        queue.pop_front();
        if(root==-1){
            // cout<<"level "<<level<<endl;
            level++;
            if(queue.empty()){
                break;
            }
            queue.push_back(-1);
            continue;
        }
        if(inGoal(pos(((root-1)/M) +1,(root-1)%M +1),self)){
                // cout<<"Dfs"<<count_debug;
                return level;
        }
        std::vector<pos> ans = neighbour2(pos(((root-1)/M) +1,(root-1)%M +1),pos(((opp-1)/M) +1,(opp-1)%M +1));
        for(std::vector<pos>::iterator it = ans.begin(); it != ans.end(); ++it) {
            i = (it->x-1)*M+it->y;
            // cout<<"i " <<i<<endl;
            if(!visited[i]){
                visited[i]=true;
                queue.push_back(i);
            }
        }
    }
    cout<<"Dfs"<<endl;
    return INT_MAX;
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
}

int utility(gamestate a){
    // printgs(a);
    // global_min = INT_MAX;
    global_min = bfs((a.self.x-1)*M+ a.self.y,(a.opp.x-1)*M+ a.opp.y,1);
    // printgs(a);
    if(global_min==INT_MAX){
        return global_min;
    }
    int minpathself = global_min;
    global_min = INT_MAX;
    global_min = bfs((a.opp.x-1)*M+ a.opp.y,(a.self.x-1)*M+ a.self.y,0);
    // cout<<"gmin2 "<<global_min<<endl;
    if(global_min==INT_MAX){
        return global_min;
    }
    // cout<<"Utitlity "<<global_min<<" "<<minpathself<<endl;
    return (global_min-minpathself);//+2*(a.wallself-a.wallopp);
}


int minVal(gamestate a,int alpha,int beta, int depth);

move maxValO(gamestate a,int alpha,int beta,int depth){
    // if(depth>=gdepth){
    //     return utility(a);
    // }
    int child;
    int maxchild=INT_MIN;
    gamestate temp;
    if(gameresult!=1){
        std::vector<pos> ans = neighbour(a.self,a.opp);
        random_shuffle(ans.begin(),ans.end());
        for(std::vector<pos>::iterator it = ans.begin(); it != ans.end(); ++it) {
            gamestate b = gamestate(a);
            b.self.x=it->x;
            b.self.y=it->y;
            child = minVal(b,alpha,beta,depth+1);
            cout<<"child " <<b.self.x<<" " <<b.self.y<<" " <<child<<endl;
            alpha = max(alpha,child);
            if(alpha>=beta){
                return move(0,b.self);
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
    //add wall //can reduce the depth for the case of walls

    random_shuffle(ii.begin(), ii.end());
    random_shuffle(ij.begin(), ij.end());
    int i;
    int j;
    if(gameresult!=2){
        if(a.wallself>0){
            for(std::vector<int>::iterator it = ii.begin(); it != ii.end(); ++it){
            // for(int i=2;i<=N;i++){
                for(std::vector<int>::iterator it2 = ij.begin(); it2 != ij.end(); ++it2){
            //     for(int j=2;j<=M;j++){
                    i=*it;
                    j=*it2;
                    cout<< i<< " "<<j<<endl;
                    if(walls[i][j]==0 && walls[i-1][j]!=1 && walls[i+1][j]!=1){
                        walls[i][j]=1;
                        gamestate b = gamestate(a);
                        b.wallself--;
                        if(utility(b)!=INT_MAX){
                            child = minVal(b,alpha,beta,depth+1);
                            alpha = max(alpha,child);
                            walls[i][j]=0;
                            if(alpha>=beta){
                                return move(2,pos(i,j));
                            }
                            if(maxchild<child){
                                maxchild=child;
                                temp=b;
                                wallpos = pos(i,j);
                                bestwall=true;
                                vert=true;
                            }
                        }
                        else{
                            walls[i][j]=0;
                        }
                    }
                    if(walls[i][j]==0 && walls[i][j-1]!=2 && walls[i][j+1]!=2){
                        walls[i][j]=2;
                        gamestate b = gamestate(a);
                        b.wallself--;
                        if(utility(b)!=INT_MAX){
                            child = minVal(b,alpha,beta,depth+1);
                            alpha = max(alpha,child);
                            walls[i][j]=0;
                            if(alpha>=beta){
                                return move(1,pos(i,j));
                            }
                            if(maxchild<child){
                                maxchild=child;
                                temp=b;
                                wallpos = pos(i,j);
                                bestwall=true;
                                vert=false;
                            }
                        }
                        else{
                            walls[i][j]=0;
                        }
                    }
                }
            }
        }
    }

        cout<<"Best wall "<<bestwall<<endl;
        printgs(temp);
        cout<<"wall "<<wallpos.x<<" "<<wallpos.y<<endl;
        cout<<"Dir "<<vert<<endl;
        cout<<"M "<<maxchild<<endl;
        if(bestwall && vert){
            return move(2,wallpos);
        }
        else if(bestwall && !vert){
            return move(1,wallpos);
        }
        else if(maxchild==INT_MIN){
            return move(0,pos(0,0));
        }
        return move(0,temp.self);
}



int maxVal(gamestate a,int alpha,int beta,int depth){
    if(depth>=gdepth){
        return utility(a);
    }
    int child;
    int maxchild=INT_MIN;
        std::vector<pos> ans = neighbour(a.self,a.opp);
        random_shuffle(ans.begin(),ans.end());
        for(std::vector<pos>::iterator it = ans.begin(); it != ans.end(); ++it) {
            gamestate b = gamestate(a);
            b.self.x=it->x;
            b.self.y=it->y;
            child = minVal(b,alpha,beta,depth+1);
            alpha = max(alpha,child);
            if(alpha>=beta){
                return child;
            }
            if(maxchild<child){
                maxchild=child;
            }
        }

    random_shuffle(ii2.begin(), ii2.end());
    random_shuffle(ij2.begin(), ij2.end());
    int i;
    int j;
    if(gameresult!=2){
        if(a.wallself>0){
            for(std::vector<int>::iterator it = ii2.begin(); it != ii2.end(); ++it){
                for(std::vector<int>::iterator it2 = ij2.begin(); it2 != ij2.end(); ++it2){
                        i=*it;
                        j=*it2;
                    // cout<< i<< " "<<j<<endl;
                    if(walls[i][j]==0 && walls[i-1][j]!=1 && walls[i+1][j]!=1){
                        walls[i][j]=1;
                        gamestate b = gamestate(a);
                        b.wallself--;
                        child = minVal(b,alpha,beta,depth+1);
                        alpha = max(alpha,child);
                        walls[i][j]=0;
                        if(alpha>=beta){
                            return child;
                        }
                        maxchild=max(maxchild,child);
                    }
                    if(walls[i][j]==0 && walls[i][j-1]!=2 && walls[i][j+1]!=2){
                        walls[i][j]=2;
                        gamestate b = gamestate(a);
                        b.wallself--;
                        child = minVal(b,alpha,beta,depth+1);
                        alpha = max(alpha,child);
                        walls[i][j]=0;
                        if(alpha>=beta){
                            return child;
                        }
                        maxchild=max(maxchild,child);
                    }
                }
            }
        }
    }
    if(maxchild==INT_MIN){
        return INT_MAX;
    }
    return maxchild;
}


int minVal(gamestate a,int alpha, int beta,int depth){
    if(depth>=gdepth){
        int ut=utility(a);
        if(ut==INT_MAX){
            return INT_MIN;
        }
        return ut;
    }
    int child;
    int minchild=INT_MAX;
        std::vector<pos> ans = neighbour(a.opp,a.self);
        random_shuffle(ans.begin(),ans.end());
        for(std::vector<pos>::iterator it = ans.begin(); it != ans.end(); ++it) {
            gamestate b = gamestate(a);
            b.opp.x=it->x;
            b.opp.y=it->y;
            child = maxVal(b,alpha,beta,depth+1);
            beta = min(beta,child);
            if(alpha>=beta){
                return child;
            }
            minchild = min(minchild,child);
        }

    random_shuffle(ii3.begin(), ii3.end());
    random_shuffle(ij3.begin(), ij3.end());

    int i;
    int j;
    if(gameresult!=1){
        if(a.wallopp>0){
            for(std::vector<int>::iterator it = ii3.begin(); it != ii3.end(); ++it){
                for(std::vector<int>::iterator it2 = ij3.begin(); it2 != ij3.end(); ++it2){
                    i=*it;
                    j=*it2;
                    if(walls[i][j]==0 && walls[i-1][j]!=1 && walls[i+1][j]!=1){
                        walls[i][j]=1;
                        gamestate b = gamestate(a);
                        b.wallself--;
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
                        b.wallself--;
                        child = maxVal(b,alpha,beta,depth+1);
                        beta = min(beta,child);
                        walls[i][j]=0;
                        if(alpha>=beta){
                            return child;
                        }
                        minchild = min(minchild,child);
                    }
                }
            }        
        }
    }

    if(minchild==INT_MAX){
        return INT_MIN;
    }
    // cout<<"N"<<endl;
    return minchild;
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
    move tobe;
    current.wallself=K;
    current.wallopp=K;
    walls = (int **)malloc(sizeof(int *)*(N+2));
    for(int i=0;i<N+2;i++)
    {   
        walls[i]=(int *)malloc(sizeof(int)*(M+2));
    }
    visited = (bool *)malloc(sizeof(bool)*(N*M+1));
    ii.resize(N-1);
    ij.resize(M-1);
    ii2.resize(N-1);
    ij2.resize(M-1);
    ii3.resize(N-1);
    ij3.resize(M-1);
    for(int i=2;i<=N;i++){
        ii[i-2]=i;
        ii2[i-2]=i;
        ii3[i-2]=i;
    }
    for(int i=2;i<=M;i++){
        ij[i-2]=i;
        ij2[i-2]=i;
        ij3[i-2]=i;
    }

    if(player == 1)
    {
        current.opp = pos(N,(M+1)/2);
        current.self = pos(1,(M+1)/2);
        memset(sendBuff, '0', sizeof(sendBuff)); 
        string temp;
        m=0; r=2;c=5;
        current.self = pos(r,c);
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
    else{
        current.opp = pos(1,(M+1)/2);
        current.self = pos(N,(M+1)/2);

    }
    printgs(current);
    while(x)
    {
        memset(recvBuff, '0',sizeof(recvBuff));
        n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
        recvBuff[n] = 0;
        sscanf(recvBuff, "%d %d %d %d", &om,&oro,&oc,&d);
        cout << om<<" "<<oro<<" "<<oc << " "<<d<<endl;
        if(om==0){
            current.opp = pos(oro,oc);
            // cout<<"H1"<<endl;
        }
        else if(om==1){
            walls[oro][oc]=2;
            current.wallopp--;
            // cout<<"H2"<<endl;
        }
        else{
            walls[oro][oc]=1;
            current.wallopp--;
            // cout<<"H3"<<endl;
        }
        if(d==(30+player))
        {
            gameresult=1;
            cout<<"You win!! Yayee!! :D ";
            // break;
        }
        else if(d==(33-player))
        {
            gameresult=2;
            cout<<"Loser :P ";
            // break;
        }
        else if(d==1){
            gameresult=2;
            break;
        }
        else if(d==2){
            gameresult=2;
            break;
        }

        tobe = maxValO(current,INT_MIN,INT_MAX,0);
            memset(sendBuff, '0', sizeof(sendBuff)); 
            string temp;
            m = tobe.mov;
            r = tobe.posit.x;
            c = tobe.posit.y;
            cout<< m<<" " <<r<< " "<<c<<"Move made";
            // cin>>m>>r>>c;
            snprintf(sendBuff, sizeof(sendBuff), "%d %d %d", m, r , c);
            write(sockfd, sendBuff, strlen(sendBuff));

            memset(recvBuff, '0',sizeof(recvBuff));
            n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
            recvBuff[n] = 0;
            sscanf(recvBuff, "%f %d", &TL, &d);//d=3 indicates game continues.. d=2 indicates lost game, d=1 means game won.
            cout<<TL<<" "<<d<<endl;
            if(2<TL<=10){
                gdepth=2;
            }
            if(TL<=2){
                gdepth=1;
            }
            if(m==0){
                // cout<<"h1"<<endl;
                if(r==0 && c==0){

                }
                else{
                    current.self = pos(r,c);
                }
            }
            else if(m==1){
                walls[r][c] = 2;
                current.wallself--;
                // cout<<"h2"<<endl;
            }
            else{
                // cout<<"h3"<<endl;
                current.wallself--;
                walls[r][c] = 1;
            }
            if(d==(30+player))
            {
                gameresult=1;
                cout<<"You win!! Yayee!! :D ";
                // break;
            }
            else if(d==(33-player))
            {
                gameresult=2;
                cout<<"Loser :P ";
                // break;
            }
            else if(d==1){
                gameresult=2;
                break;
            }
            else if(d==2){
                gameresult=2;
                break;
            }
            // maxVal(current,INT_MIN,INT_MAX,0);
            // printgs(current);
            // sleep(1);
    }
    cout<<endl<<"The End"<<endl;
    return 0;
}
















