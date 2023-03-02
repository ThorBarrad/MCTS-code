#include<iostream>
#include<cstdlib>
#include <time.h>
#include<math.h>
int maxsearch=9999;
int c=2;
using namespace std;
class chessboard{
	private:
		int board[9][9];
		int turn;	
		bool visited[9][9];
	public:
		chessboard();
		bool gameover();
		bool drop(int x,int y);
		void printboard();
		bool hasair(int x,int y,int val);
		void refresh();
		int getturn(){
			return turn;
		}
};
chessboard::chessboard(){
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
			board[i][j]=0;
		}
	}
	turn=1;
}
void chessboard::refresh(){
	for(int x=0;x<9;x++){
		for(int y=0;y<9;y++){
			visited[x][y]=false;
		}
	}
}
bool chessboard::hasair(int x,int y,int val){
	if(x==-1||x==9||y==-1||y==9){
		return false;
	}
	if(visited[x][y]){
		return false;
	}
	if(board[x][y]==0){
		return true;
	}
	if(board[x][y]!=val){
		return false;
	}
	visited[x][y]=true;
	if(hasair(x-1,y,val)||hasair(x+1,y,val)||hasair(x,y+1,val)||hasair(x,y-1,val)){
		return true;
	}
	return false;
}
bool chessboard::gameover(){
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
			refresh();
			if(!hasair(i,j,board[i][j])){
				turn=3-turn;
				return true;
			}
		}
	}
	return false;
}
bool chessboard::drop(int x,int y){
	if(board[x][y]==0&&x>=0&&x<=8&&y>=0&&y<=8){
		board[x][y]=turn;
		turn=3-turn;
		return true;
	}
	else{
		return false;
	}
}
void chessboard::printboard(){
	cout<<"turn:"<<turn<<endl;
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
			cout<<board[i][j]<<" ";
		}
		cout<<endl;
	}
}

int randomdrop(chessboard c,int t){
	int x,y;
	srand((unsigned)time(NULL));
	x = rand() % 9;
	y = rand() % 9;
	while(!c.drop(x,y)){
		x = rand() % 9;
		y = rand() % 9;
	}
	if(c.gameover()){
		if(c.getturn()==t){
			return 0;
		}
		return 1;
	}
	return randomdrop(c,t);
}

typedef struct treenode{
	chessboard c;
	double wingames;
	double allgames;
	int newx,newy;
	struct treenode *parent,*firstchild,*nextbrother;
}treenode,*tree;
double caluct(tree t){
	if(t->allgames==0){
		return 9999;
	}
	double res;
	tree a;
	for(a=t;a->parent;a=a->parent){
	}
	res=t->wingames/t->allgames+sqrt(c*log(a->allgames)/t->allgames);
	//res=t->wingames/t->allgames+sqrt(c*log(t->parent->allgames)/t->allgames);
	return res;
}
void generatechild(tree &t){
	chessboard cb=t->c;
	for(int i=0;i<9;i++){
		for(int j=0;j<9;j++){
			if(cb.drop(i,j)){
				if(!cb.gameover()){
					treenode *tn=(treenode*)malloc(sizeof(treenode));
					tn->c=cb;
					tn->wingames=0;
					tn->allgames=0;
					tn->newx=i;
					tn->newy=j;
					tn->parent=t;
					tn->firstchild=NULL;
					tn->nextbrother=t->firstchild;
					t->firstchild=tn;
				}
			}
			cb=t->c;
		}
	}
}
void traceback(tree &t,int i){
	for(tree temp=t;temp;temp=temp->parent){
		temp->allgames++;
		temp->wingames+=i;
		i=1-i;
	}
}
treenode* findchild(treenode *t){
	treenode* res=t->firstchild;
	for(treenode* temp=t->firstchild;temp;temp=temp->nextbrother){
		if(caluct(temp)>caluct(res)){
			res=temp;
		}
	}
	return res;
}
void search(treenode *t){
	if(t->allgames==0){
		int i=randomdrop(t->c,t->c.getturn());
		traceback(t,i);
	}
	else{
		if(!t->firstchild){
			generatechild(t);
			
		}
		if(t->firstchild){
			search(findchild(t));
		}
		
	}
}

bool montecarlo(chessboard cb,int &x,int &y){
	treenode *t=(treenode*)malloc(sizeof(treenode));
	t->c=cb;
	t->wingames=0;
	t->allgames=0;
	t->newx=0;
	t->newy=0;
	t->parent=NULL;
	t->firstchild=NULL;
	t->nextbrother=NULL;
	for(int i=0;i<maxsearch;i++){
		search(t);
	}
	if(t->firstchild){
		x=findchild(t)->newx;
		y=findchild(t)->newy;
		return true;
	}
	return false;
}


int main(){
	int x,y;
	char entry;
	chessboard c=chessboard();
	while(!c.gameover()){
		system("cls");
		c.printboard();
		/* 
		srand((unsigned)time(NULL));
		x = rand() % 9;
		y = rand() % 9;
		while(!c.drop(x,y)){
			x = rand() % 9;
			y = rand() % 9;
		}
		*/
		/*
		cin>>x>>y;
		while(!c.drop(x,y)){
			cin>>x>>y;
		}
		*/
		
		if(!montecarlo(c,x,y)){
			cout<<"player 1 lose"<<endl;
			break;
		}
		cout<<x<<" "<<y<<endl;
		c.drop(x,y);
			
		if(c.gameover()){
			break;
		}
		
/**************************************/		
		c.printboard();
		
		if(!montecarlo(c,x,y)){
			cout<<"player 2 lose"<<endl;
			break;
		}
		cout<<x<<" "<<y<<endl;
		c.drop(x,y);
		
		/*
		cin>>x>>y;
		while(!c.drop(x,y)){
			cin>>x>>y;
		}
		*/
	}
	//cout<<"game over! player "<<c.getturn()<<" has stepped onto a wrong position!"<<endl;
	c.printboard();
	return 0;
}

