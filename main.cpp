#include <iostream>
#include <vector>
#include <algorithm>
#include <unistd.h>


using namespace std;

// function below clears the board
void ResetBoard(int board[][9]);

void DisplayBoard(int board[][9]);

void SetupBoard(int board[][9],vector<pair<int,vector<int>>> &emptyCells);

int calculateIndex(int r, int c);

//	.first (row)		.second (column)
pair<int,int> getBoxStart(int index);
pair<int,int> getPosition(int index);

void addToVector(int value, vector<pair<int,vector<int>>> &emptyCell, int ind);
void removeFromVector(int value, vector<pair<int,vector<int>>> &emptyCell, int ind);

void pushElement(int row, int col, int value, int board[][9], vector<pair<int,vector<int>>> &emptyCell);
void popElement(int row, int col, int board[][9], vector<pair<int,vector<int>>> &emptyCell);

bool solved(int board[][9],vector<pair<int,vector<int>>> &emptyCells, int ind);

//for debugging only
void showPossibilities(vector<pair<int,vector<int>>> emptyCell);


void solve(){
	
	int board [9][9];
	
	//		.first is the index corresponding to the empty cell
	//		.second is a vector of all possible values in that cell
	vector<pair<int,vector<int>>>emptyCells;
	
	ResetBoard(board);
	DisplayBoard(board);
	
	SetupBoard(board,emptyCells);
	DisplayBoard(board);
	int ind = 0;
	
	
	if( emptyCells.empty() || solved(board, emptyCells, ind) ){
		DisplayBoard(board);
	}else{
		cout << "\nUNSOLVABLE\n";
	}
	
	
	//return 0;
}

// function below clears the board
void ResetBoard(int board[][9]){
	for(int r=0; r<9; r++){
		for(int c=0; c<9; c++){
			board[r][c] = 0;
		}
	}
}

void DisplayBoard(int board[][9]){
	//system("clear");
	cout << "\t ______ ______ ______ ______ ______ ______ ______ ______ ______ \n";
	for(int r=0; r<9; r++){
		cout << "\t|      |      |      |      |      |      |      |      |      |\n";
		cout << "\t";
		for(int c=0; c<9; c++){
			cout << "|   ";
			if(board[r][c] == 0){
				cout << " ";
			}else{
				cout << board[r][c];
			}
			cout << "  ";
		}
		cout << "|\n";
		cout << "\t|______|______|______|______|______|______|______|______|______|\n";
	}
}

void SetupBoard(int board[][9],vector<pair<int,vector<int>>> &emptyCells){
	cout << "\nEnter each cell seperated by space.\n( for empty cells enter '0' )\n\n";
	
	for(int r=0; r<9; r++){
		for(int c=0; c<9; c++){
			cin >> board[r][c];
			if(board[r][c] == 0){
				int index = calculateIndex(r,c);
				vector<int>temp = {1,2,3,4,5,6,7,8,9};
				emptyCells.push_back(make_pair(index,temp));
			}
		}
	}
	for(int r=0; r<9; r++){
		for(int c=0; c<9; c++){
			if(board[r][c] != 0){
				pushElement(r, c, board[r][c], board, emptyCells);
			}
		}
	}
}

int calculateIndex(int r, int c){
	return (r*9)+c;
}

pair<int,int> getBoxStart(int index){
	int row;
	row = (index/27)*3;
	int col;
	col = ((index%9)/3)*3;
	return make_pair(row,col);
}

pair<int,int> getPosition(int index){
	return make_pair(index/9,index%9);
}

void removeFromVector(int value, vector<pair<int,vector<int>>> &emptyCell, int ind){
	vector<int>temp;
	for(auto e: emptyCell[ind].second){
		if(e != value){
			temp.push_back(e);
		}
	}
	emptyCell[ind].second = temp;

}

void addToVector(int value, vector<pair<int,vector<int>>> &emptyCell, int ind){
	vector<int>temp;
	int f = 1;
	int s = emptyCell[ind].second.size();		//mafhosh 8
	for(int i=0; i<emptyCell[ind].second.size();i++){
		if(emptyCell[ind].second[i] > value && f){
			temp.push_back(value);
			f = 0;
		}
		temp.push_back(emptyCell[ind].second[i]);
	}
	if(s <= temp.size() && f){
		temp.push_back(value);
	}
	emptyCell[ind].second = temp;
}

bool isSafe(int value, int index, int board[][9]){
	//is safe for same column
	pair<int,int> pos = getPosition(index);
	for(int r=0; r<9; r++){
		if(r!=pos.first){
			if(board[r][pos.second] == value){
				return false;
			}
		}
	}
	//is safe for same row
	for(int c=0; c<9; c++){
		if(c!=pos.second){
			if(board[pos.first][c] == value){
				return false;
			}
		}
	}
	int st_r;
	int st_c;
	pair<int,int> st = getBoxStart(index);
	st_r = st.first;
	st_c = st.second;
	//is safe for same box
	for(int r=st_r; r<st_r+3; r++){
		for(int c=st_c; c<st_c+3; c++){
			if(r != pos.first || c != pos.second){
				if(board[r][c] == value){
					return false;
				}
			}
		}
	}
	return true;
}

void pushElement(int row, int col, int value, int board[][9], vector<pair<int,vector<int>>> &emptyCell){
	board[row][col] = value;
	
	int index = calculateIndex(row,col);
	pair<int,int> boxRange = getBoxStart(index);
	int a = 0;
	for(auto v: emptyCell){
		//clear col			|
		if(v.first != index && v.first%9 == index%9){
			removeFromVector(value,emptyCell,a);
		}
		//clear row		   ---
		if(v.first != index && v.first/9 == index/9){
			removeFromVector(value,emptyCell,a);
		}
		//clear box		[#]
		pair<int,int> cellRange = getBoxStart(v.first);
		if(v.first != index && cellRange == boxRange){
			removeFromVector(value,emptyCell,a);
		}
		a++;
	}
}

void popElement(int row, int col, int board[][9], vector<pair<int,vector<int>>> &emptyCell){
	int value = board[row][col];
	
	board[row][col] = 0;
	
	int index = calculateIndex(row,col);
	pair<int,int> boxRange = getBoxStart(index);
	int a = 0;
	for(auto v: emptyCell){
		if(index != v.first){
			pair<int,int> cellRange = getBoxStart(v.first);
			if(v.first%9 == index%9){				//append col			|
				if(isSafe(value,v.first,board)){
					addToVector(value,emptyCell,a);
				}
			}else if(v.first/9 == index/9){			//append row		   ---
				if(isSafe(value,v.first,board)){
					addToVector(value,emptyCell,a);
				}
			}else if(cellRange == boxRange){			//append box		[#]
				if(isSafe(value,v.first,board)){
					addToVector(value,emptyCell,a);
				}
			}
		}
		a++;
	}
}

bool solved(int board[][9],vector<pair<int,vector<int>>> &emptyCells, int ind){
	
//base case
	int last_index = emptyCells.back().first;
	if(board[getPosition(last_index).first][getPosition(last_index).second] != 0){
		return true;
		//solved
	}
	
	if(emptyCells[ind].second.empty()){
		return false;
		//stuck
	}
	
	int index = emptyCells[ind].first;
	pair<int,int> position = getPosition(index);
	int r = position.first;	int c = position.second;
	bool isCorrectPath;
	
	for(int i=0; i<emptyCells[ind].second.size(); i++){
		pushElement(r, c, emptyCells[ind].second[i], board, emptyCells);
		isCorrectPath = solved(board, emptyCells, ind+1);
		if(!isCorrectPath){
			popElement(r, c, board, emptyCells);
		}else{
			return true;
		}
	}
	if(!isCorrectPath){
		return false;
	}
	return true;
}





//for debugging only
void showPossibilities(vector<pair<int,vector<int>>> emptyCell){
	for(auto v: emptyCell){
		cout << "Index->" << v.first << " / ";
		for(auto e: v.second){
			cout << e << " ";
		}
		cout << "\n";
	}
	
}

int main(){
	int t;
	cin >> t;
	while(t--){
		solve();
	}
	return 0;
}