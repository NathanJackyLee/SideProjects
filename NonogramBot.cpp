#include<bits/stdc++.h>
using namespace std;
#define pb push_back

// board is 1-15 x 1-15
// row is 1-15
// col is 1-15

int bs; // board size

// assuming maximum board size of 100
char board[102][102];
vector<int> row[101];
vector<int> col[101];
int target;

void printBoard(){
//	for(int i=1;i<=bs;i++) cout <<"__"; cout <<"_\n";
//	cout <<"_______________________________\n";
	for(int i=1;i<=bs;i++){
		cout <<"|";
		for(int j=1;j<=bs;j++){
			if(board[i][j]=='o') cout <<board[i][j];
			else if(board[i][j]=='x') cout <<" ";
			cout <<"|";
		}
		cout <<endl;
//		for(int i=1;i<=bs;i++) cout <<"--"; cout <<"-\n";
//		cout <<"\n-------------------------------\n";
	}
}
void init(){
	cout <<"What size board is this? ";
	cin >>bs; getchar();
	for(int i=0;i<bs+2;i++) for(int j=0;j<bs+2;j++) ((i==0)||(j==0)||(i==bs+1)||(j==bs+1))? board[i][j]='x':board[i][j]=' ';
	int n,get;
	target = 0;
	char st[50];
	for(int i=1;i<=bs;i++){
		cout <<"Input row " <<i <<" from left to right: ";
		scanf("%[^\n]",st); getchar();
		stringstream stream(st);
		while(stream >> get){
			target += get;
			row[i].pb(get);
		}
		strcpy(st,"");
	}
	for(int i=1;i<=bs;i++){
		cout <<"Input col " <<i <<" from top to bottom: ";
		scanf("%[^\n]",st); getchar();
		stringstream stream(st);
		while(stream >> get){
			target += get;
			col[i].pb(get);
		}
		strcpy(st,""); 
	}
}

struct pellet {string code; int di;};
struct pellet pelt(string code,int di){
	struct pellet bruh;
	bruh.code = code;
	bruh.di = di;
	return bruh;
}
string codify(int a,int b){
	char A = 'a'+a-1;
	char B = 'a'+b-1;
	string bruh = "";
	bruh = bruh + A;
	bruh = bruh + B;
	return bruh;
}

bool includesAlphaBlocksR(string code,int rowi){
	bool sol[bs+2]; memset(sol,false,sizeof(sol));
	for(int i=0;i<code.length();i+=2){
		int a = code[i]-'a'+1;
		int b = code[i+1]-'a'+1;
		for(int j=a;j<=b;j++) sol[j]++;
	}
	for(int j=1;j<=bs;j++) if((board[rowi][j]=='o')&&(sol[j]==false)) return false;
	return true;
}
bool includesAlphaBlocksC(string code,int colj){
	bool sol[bs+2]; memset(sol,false,sizeof(sol));
	for(int i=0;i<code.length();i+=2){
		int a = code[i]-'a'+1;
		int b = code[i+1]-'a'+1;
		for(int j=a;j<=b;j++) sol[j]++;
	}
	for(int i=1;i<=bs;i++) if((board[i][colj]=='o')&&(sol[i]==false)) return false;
	return true;
}

bool validPlaceR(int dj,int re,int rowi){
	bool yes = true;
	for(int j=dj;j<dj+re;j++) if(board[rowi][j]=='x'){yes = false; break;}
	if(board[rowi][dj-1]=='o') yes=false;
	if(board[rowi][dj+re]=='o') yes=false;
	return yes;
}
bool validPlaceC(int di,int ce,int colj){
	bool yes = true;
	for(int i=di;i<di+ce;i++) if(board[i][colj]=='x'){yes = false; break;}
	if(board[di-1][colj]=='o') yes=false;
	if(board[di+ce][colj]=='o') yes=false;
	return yes;
}

void solverow(int rowi){
	int deck[bs+2]; memset(deck,0,sizeof(deck));
	int NumOfSol = 0;
	queue<struct pellet> que; 
	struct pellet cur;
	que.push(pelt("",2)); // first skip
	if(validPlaceR(1,row[rowi][0],rowi)) que.push(pelt(codify(1,row[rowi][0]),row[rowi][0]+2)); // first block place
	while(!que.empty()){
		cur = que.front(); que.pop();
		int rei = cur.code.length()/2;
		if(rei == row[rowi].size()){
			if(includesAlphaBlocksR(cur.code,rowi)){
				NumOfSol++;
				for(int i=0;i<cur.code.length();i+=2){
					int a = cur.code[i]-'a'+1;
					int b = cur.code[i+1]-'a'+1;
					for(int j=a;j<=b;j++) deck[j]++;
				}
			}
			continue;
		}
		int clearance_needed = 0;
		for(int j=rei;j<row[rowi].size();j++){clearance_needed += row[rowi][j]; clearance_needed++;} clearance_needed--;
		if((bs+1) - cur.di >= clearance_needed){
			if(validPlaceR(cur.di,row[rowi][rei],rowi)){
				que.push(pelt(cur.code + codify(cur.di,cur.di+row[rowi][rei]-1), cur.di+row[rowi][rei]+1));
			}
		}
		if((bs+1) - cur.di > clearance_needed) que.push(pelt(cur.code,cur.di+1));
	}
	for(int j=1;j<=bs;j++){
		if(deck[j] == NumOfSol) {board[rowi][j]='o'; target--;}
		if((deck[j] == 0)&&(board[rowi][j]!='o')) board[rowi][j]='x';
	}
}

void solvecol(int colj){
	int deck[bs+2]; memset(deck,0,sizeof(deck));
	int NumOfSol = 0;
	queue<struct pellet> que; 
	struct pellet cur;
	que.push(pelt("",2)); // first skip
	if(validPlaceC(1,col[colj][0],colj)) que.push(pelt(codify(1,col[colj][0]),col[colj][0]+2)); // first block place
	while(!que.empty()){
		cur = que.front(); que.pop();
		int cei = cur.code.length()/2;
		if(cei == col[colj].size()){
			if(includesAlphaBlocksC(cur.code,colj)){
				NumOfSol++;
				for(int i=0;i<cur.code.length();i+=2){
					int a = cur.code[i]-'a'+1;
					int b = cur.code[i+1]-'a'+1;
					for(int j=a;j<=b;j++) deck[j]++;
				}
			}
			continue;
		}
		int clearance_needed = 0;
		for(int i=cei;i<col[colj].size();i++){clearance_needed += col[colj][i]; clearance_needed++;} clearance_needed--;
		if((bs+1) - cur.di >= clearance_needed){
			if(validPlaceC(cur.di,col[colj][cei],colj)){
				que.push(pelt(cur.code + codify(cur.di,cur.di+col[colj][cei]-1), cur.di+col[colj][cei]+1));
			}
		}
		if((bs+1) - cur.di > clearance_needed) que.push(pelt(cur.code,cur.di+1));
	}
	for(int i=1;i<=bs;i++){
		if(deck[i] == NumOfSol) {board[i][colj]='o'; target--;}
		if((deck[i] == 0)&&(board[i][colj]!='o')) board[i][colj]='x';
	}
}

int main(){
	init();
	// this is just looping it 50 times :p
	// when I tried to auto-stop it with counting, for some reason it stops early
	// and in the grand scheme of things, what's a couple of extra rounds anyway :p
	for(int z=0;z<50;z++){
		for(int i=1;i<=bs;i++) solverow(i);
		for(int j=1;j<=bs;j++) solvecol(j);
	}
	printBoard();
	return 0;
}
