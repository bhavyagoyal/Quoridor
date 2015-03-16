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

typedef pair<int,int> ii;

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
    set< ii* > wallsv;
    set< ii* > wallsh;
    // gamestate(){};
    // gamestate(const gamestate& b) : self(b.self), opp(b.opp){};
};

int N,M,K, time_left, player;
gamestate current;


// void printgs(gamestate a){
//     cout<<"self "<<a.self.x<<" "<<a.self.y<<endl;
//     cout<<"Opp "<<a.opp.x<<" "<<a.opp.y<<endl;
//     cout<<"Wallsh ";
//     cout<<a.wallsh.size()<<" Size "<<endl;
//     for(std::set<pos>::iterator it = a.wallsh.begin(); it != a.wallsh.end(); ++it) {
//         cout<<it->x<<" "<<it->y<<" ";
//     }
//     cout<<"\nWallsv ";
//     for(std::set<pos>::iterator it = a.wallsv.begin(); it != a.wallsv.end(); ++it) {
//         cout<<it->x<<" "<<it->y<<" ";
//     }
//     cout<<"\n";
// }



int main(int argc, char *argv[])
{
    // current->self = pos(2,3);
    // current->opp = pos(4,5);
    ii* p = make_pair(5,6);
    current.wallsv.insert(p);
    cout<<current.wallsv.size()<<endl;
    // for(std::set<ii>::iterator it = current.wallsh.begin(); it != current.wallsh.end(); ++it) {
    //     cout<<(*it)->first<<" "<<(*it)->second<<" ";
    // }
    // printgs(current);
    return 0;
}
















