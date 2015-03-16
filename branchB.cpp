#include <bits/stdc++.h>

using namespace std;




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


struct node{
    pos p;
    int cost;
    bool visited;
    vector<node> adj;
};

struct node_cmp
{
	bool operator()( const node a, const node b) const
	{
		return a.cost > b.cost ;
	}
};

vector< node > adjList;

vector<node> goal;

int global_min = 1000000;

bool inGoal(node p){
	for(int i=0;i<goal.size();i++){
		if(goal.at(i).p.x == p.p.x && goal.at(i).p.y == p.p.y ){
			return true;
		}
	}
	return false;
}

vector< int > path;

void search(vector<int> explored, int path_length ,node start, int n,int m){

	start.visited =true;
	cout<<"pos= "<<(start.p.y-1)*m+start.p.x-1<<endl;
	explored[(start.p.y-1)*m+start.p.x-1]=1;

	if(inGoal(start)){
		cout<<"Goal"<<endl;
		if(global_min> path_length){
			global_min = path_length;
		}
		path.push_back(path_length);
		
	}
	else{
		cout<<"Not Goal "<<start.adj.size()<<endl;
		if(start.adj.size()!=0){
			priority_queue<node,vector<node>,node_cmp> child_heap ;
			for(int i=0;i<start.adj.size();i++)
			{
				pos p=start.adj[i].p;

				if(path_length+1+(n-p.y)<global_min)
				{
					node child=adjList[(p.y-1)*m+p.x-1];
					child.cost = path_length+1+(n-p.y);
					child_heap.push(child);
				}
				

			}
			while(!child_heap.empty())
			{
				node child = child_heap.top();
				child_heap.pop();
				pos p=child.p;
				cout<<"new Pos= "<<(p.y-1)*m+p.x-1<<endl;

				if(explored[(p.y-1)*m+p.x-1]!=1){
					if(global_min> path_length+1 + (n-p.y)){
					cout<<"new Pos searched= "<<(p.y-1)*m+p.x-1<<endl;

						search(explored,path_length+1,adjList[(p.y-1)*m+p.x-1],n,m);
					}
				}
			}

			// for(int i=0;i<start.adj.size();i++)
			// {	
			// 	pos p=start.adj[i].p;
			// 	// cout<<"new Pos= "<<(p.y-1)*m+p.x-1<<endl;
				
			// 	if(explored[(p.y-1)*m+p.x-1]!=1){
			// 		if(global_min> path_length+1 + (n-p.y)){
			// 			search(explored,path_length+1,adjList[(p.y-1)*m+p.x-1],m);
			// 		}
			// 	}
				
			// }
			// cout<<"closed "<<(start.p.y-1)*m+start.p.x-1<<endl;
			// adjList[(start.p.y-1)*m+start.p.x-1].visited = true;

		}
		else{
			// cout<<"closed "<<(start.p.y-1)*m+start.p.x-1<<endl;
			// adjList[(start.p.y-1)*m+start.p.x-1].visited = true;
		}

	}	


}




int main(){

	int n = 9;
	int m = 9;
	int k =  10;






int walls[n+1][m+1];

for(int i=1;i<n+1;i++){
	for(int j=1;j<m+1;j++){
		walls[i][j]=0;
	}
}
walls[2][7]=2;
walls[4][7]=2;
walls[5][6]=1;
walls[5][4]=1;
walls[4][3]=2;



// cout<<walls[3][3]<<"asdvasd"<<endl;
pos opp(2,3);





for(int i=1; i <= n; i++){
	for(int j=1;j<=m;j++){
		pos p(j,i);
		node item;
		item.p  = p;
		
		if(p.y+1<=m){
			if(walls[p.x][p.y+1]==2 || walls[p.x+1][p.y+1]==2){
			}
			else{
				pos newP(p.x,p.y+1);
				node element;
				element.p = newP;
				
				item.adj.push_back(element);
			}
			
		}
		if(p.x-1>0){
			
			if(walls[p.x][p.y]==1 || walls[p.x][p.y+1]==1){
			}
			else{
				pos newP(p.x-1,p.y);
				node element;
				element.p = newP;
				item.adj.push_back(element);
				
			}
		}
		if(p.x+1 <=n){
			
			if(walls[p.x+1][p.y]==1 || walls[p.x+1][p.y+1]==1){}
			else{
				pos newP(p.x+1,p.y);
				node element;
				element.p = newP;
				item.adj.push_back(element);
			}
		}
		if(p.y-1>0){
			
			if(walls[p.x][p.y]==2 || walls[p.x+1][p.y]==2){}
			else{	
				pos newP (p.x,p.y-1);
				node element;
				element.p = newP;
				item.adj.push_back(element);
			}
		}
		adjList.push_back(item);
		
	}
}

for(int i=0; i <  adjList.size(); i++){
// 	cout<<"Pos= "<<adjList[i].p.x << ","<<adjList[i].p.y<<endl;
// 	for(int j=0;j<adjList[i].adj.size();j++){
// 		cout<<"Pos"<<j<<"= "<<adjList[i].adj[j].p.x << ","<<adjList[i].adj[j].p.y <<endl;
// 	}
// 	cout<<endl;
// }

for(int i=1;i<=m;i++){
	node a;
	pos s(i,n);
	a.p=s;
	goal.push_back(a);
}

vector<int> explored(m*n);
for(int i=0;i<m*n;i++){
	explored[i]=0;
}
node start;
search(explored,0,adjList[38],n,m);

for(int i=0;i<path.size();i++){
	cout<<path[i]<<endl;
}








return 0;
}