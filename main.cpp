#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;

int RemainingCells = 81;

void deleteValueFromVector(vector<int> &vec, int value);

// function below clears the board
void ResetBoard(int board[][9], vector<int>pValues[][9]);

void DisplayBoard(int board[][9]);

void SetupBoard(int board[][9], vector<int>pValues[][9]);

bool isFilled(int x);

void applyAlgorithm(int r, int c, int value, vector<int>pValues[][9], int board[][9]);

void EliminateValueInRowAndCol(int row, int col, int value, vector<int>pValues[][9], int board[][9]);

void EliminateValueInBox(int row, int col, int value, vector<int>pValues[][9], int board[][9]);

void display(vector<int>vec[][9]){
	for(int r=0; r<9; r++){
		for(int c=0; c<9; c++){
			cout << "R" << r << "C" << c << "\n";
			for(auto e: vec[r][c]){
				cout << e << " - ";
			}
			cout << "\n";
		}
		cout << "\n";
	}
}

bool isPossibilityContradiction(int value, int row, int column, vector<int>pValues[][9]);
bool isSafeCell(int r, int c, const int board[][9]);
bool tryForcing(int r, int c, int board[][9], vector<int>pValues[][9], vector<pair<int,pair<int,int>>>remainingPosibilities);


bool leastPossibility(pair<int,pair<int,int>>x, pair<int,pair<int,int>>y){
	return x.first < y.first;
};


int main(){
	
	vector<int>pValues[9][9];		//possible values for each cell
	int board [9][9];
	ResetBoard(board,pValues);
	DisplayBoard(board);
	SetupBoard(board, pValues);
	DisplayBoard(board);
	
	//scanning for filled cells
	for(int r=0; r<9; r++){
		for(int c=0; c<9; c++){
			//cout << isFilled(board[r][c]);
			if(isFilled(board[r][c])){
				applyAlgorithm(r,c,board[r][c],pValues,board);
			}
		}
		//cout << "\n";
	}
	
	//DisplayBoard(board);
	
	vector<pair<int,pair<int,int>>>remainingPosibilities;
	remainingPosibilities.clear();
		for(int r=0; r<9; r++){
			for(int c=0; c<9; c++){
				if(pValues[r][c].size()!=0){
					remainingPosibilities.push_back(make_pair(pValues[r][c].size(),make_pair(r,c)));
				}
			}
		}
		sort(remainingPosibilities.begin(),remainingPosibilities.end(),leastPossibility);
	
	tryForcing(0,0,board,pValues,remainingPosibilities);
	
	for(int r=0; r<9; r++){
		for(int c=0; c<9; c++){
			for(auto e: pValues[r][c]){
				if(!isPossibilityContradiction(e,r,c,pValues)){
					board[r][c] = e;
					pValues[r][c].clear();
					applyAlgorithm(r,c,board[r][c],pValues,board);
				}
			}
		}
	}
	
	//find least possible values in the board
//	vector<pair<int,pair<int,int>>>remainingPosibilities;
//	while(true){
//		remainingPosibilities.clear();
//		for(int r=0; r<9; r++){
//			for(int c=0; c<9; c++){
//				if(pValues[r][c].size()!=0){
//					remainingPosibilities.push_back(make_pair(pValues[r][c].size(),make_pair(r,c)));
//				}
//			}
//		}
//		if(remainingPosibilities.size()==0){
//			break;
//		}
//		sort(remainingPosibilities.begin(),remainingPosibilities.end(),leastPossibility);
//		
//		//try closest possible cell
//		int try_r = remainingPosibilities[0].second.first;
//		int try_c = remainingPosibilities[0].second.second;
//		board[try_r][try_c] = pValues[try_r][try_c][0];
//		pValues[try_r][try_c].clear();
//		applyAlgorithm(try_r,try_c,board[try_r][try_c],pValues,board);
//	}
	
//	for(auto e: remainingPosibilities){
//		cout << e.first << " ";
//	}
	
	
	
	//DisplayBoard(board);
	//display(pValues);
	
	return 0;
}


void deleteValueFromVector(vector<int> &vec, int value){
	vector<int>temp;
	for(auto e: vec){
		if(e!=value){
			temp.push_back(e);
		}
	}
	temp.swap(vec);
}

// function below clears the board
void ResetBoard(int board[][9], vector<int>pValues[][9]){
	for(int r=0; r<9; r++){
		for(int c=0; c<9; c++){
			board[r][c] = 0;
			//pValues[r][c].clear();
			//pushing all possible values for each cell
			for(int n=1; n<10; n++){
				pValues[r][c].push_back(n);
			}
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

void SetupBoard(int board[][9], vector<int>pValues[][9]){
	for(int r=0; r<9; r++){
		for(int c=0; c<9; c++){
			cin >> board[r][c];
			if(board[r][c] != 0){
				RemainingCells--;
				pValues[r][c].clear();
			}
		}
	}
}

bool isFilled(int x){
	return x != 0;
}


void EliminateValueInRowAndCol(int row, int col, int value, vector<int>pValues[][9], int board[][9]){
	for(int r=0; r<9; r++){
		if(r!=row){
			deleteValueFromVector(pValues[r][col],value);
			if(pValues[r][col].size()==1){
				//fill cell with the remaining value
				board[r][col] = pValues[r][col][0];
				RemainingCells--;
				pValues[r][col].clear();
				EliminateValueInRowAndCol(r,col,board[r][col],pValues,board);
				EliminateValueInBox(r,col,board[r][col],pValues,board);
			}
		}
	}
	for(int c=0; c<9; c++){
		if(c!=col){
			deleteValueFromVector(pValues[row][c],value);
			if(pValues[row][c].size()==1){
				//fill cell with the remaining value
				board[row][c] = pValues[row][c][0];
				RemainingCells--;
				pValues[row][c].clear();
				EliminateValueInRowAndCol(row,c,board[row][c],pValues,board);
				EliminateValueInBox(row,c,board[row][c],pValues,board);
			}
		}
	}
}

void EliminateValueInBox(int row, int col, int value, vector<int>pValues[][9], int board[][9]){
	if(row>5){
		//Row 3
		// 6,7,8
		int r = 6;
		if(col>5){
			//R3 C3
			int c = 6;
			for(r=6;r<9; r++){
				for(c=6;c<9; c++){
					if(!isFilled(board[r][c])){
						deleteValueFromVector(pValues[r][c],value);
						if(pValues[r][c].size()==1){
							//fill cell with the remaining value
							board[r][c] = pValues[r][c][0];
							RemainingCells--;
							pValues[r][c].clear();
							applyAlgorithm(r,c,board[r][c],pValues,board);
						}
					}
				}
			}
		}else if(col>2){
			//R3 C2
			int c = 3;
			for(r=6;r<9; r++){
				for(c=3;c<6; c++){
					if(!isFilled(board[r][c])){
						deleteValueFromVector(pValues[r][c],value);
						if(pValues[r][c].size()==1){
							//fill cell with the remaining value
							board[r][c] = pValues[r][c][0];
							RemainingCells--;
							pValues[r][c].clear();
							applyAlgorithm(r,c,board[r][c],pValues,board);
						}
					}
				}
			}
			
		}else{
			//R3 C1
			int c = 0;
			for(r=6;r<9; r++){
				for(c=0;c<3; c++){
					if(!isFilled(board[r][c])){
						deleteValueFromVector(pValues[r][c],value);
						if(pValues[r][c].size()==1){
							//fill cell with the remaining value
							board[r][c] = pValues[r][c][0];
							RemainingCells--;
							pValues[r][c].clear();
							applyAlgorithm(r,c,board[r][c],pValues,board);
						}
					}
				}
			}
		}
	}else if(row>2){
		//Row 2
		int r = 3;
		if(col>5){
			//R2 C3
			int c = 6;
			for(r=3;r<6; r++){
				for(c=6;c<9; c++){
					if(!isFilled(board[r][c])){
						deleteValueFromVector(pValues[r][c],value);
						if(pValues[r][c].size()==1){
							//fill cell with the remaining value
							board[r][c] = pValues[r][c][0];
							RemainingCells--;
							pValues[r][c].clear();
							applyAlgorithm(r,c,board[r][c],pValues,board);
						}
					}
				}
			}
			
		}else if(col>2){
			//R2 C2
			int c = 3;
			for(r=3;r<6; r++){
				for(c=3;c<6; c++){
					if(!isFilled(board[r][c])){
						deleteValueFromVector(pValues[r][c],value);
						if(pValues[r][c].size()==1){
							//fill cell with the remaining value
							board[r][c] = pValues[r][c][0];
							RemainingCells--;
							pValues[r][c].clear();
							applyAlgorithm(r,c,board[r][c],pValues,board);
						}
					}
				}
			}
			
		}else{
			//R2 C1
			int c = 0;
			for(r=3;r<6; r++){
				for(c=0;c<3; c++){
					if(!isFilled(board[r][c])){
						deleteValueFromVector(pValues[r][c],value);
						if(pValues[r][c].size()==1){
							//fill cell with the remaining value
							board[r][c] = pValues[r][c][0];
							RemainingCells--;
							pValues[r][c].clear();
							applyAlgorithm(r,c,board[r][c],pValues,board);
						}
					}
				}
			}
			
		}
	}else{
		//Row 1
		int r = 0;
		if(col>5){
			//R1 C3
			int c = 6;
			for(r = 0;r<3; r++){
				for(c = 6;c<9; c++){
					if(!isFilled(board[r][c])){
						deleteValueFromVector(pValues[r][c],value);
						if(pValues[r][c].size()==1){
							//fill cell with the remaining value
							board[r][c] = pValues[r][c][0];
							RemainingCells--;
							pValues[r][c].clear();
							applyAlgorithm(r,c,board[r][c],pValues,board);
						}
					}
				}
			}
		}else if(col>2){
			//R1 C2
			int c = 3;
			for(r = 0;r<3; r++){
				for(c = 3;c<6; c++){
					if(!isFilled(board[r][c])){
						deleteValueFromVector(pValues[r][c],value);
						if(pValues[r][c].size()==1){
							//fill cell with the remaining value
							board[r][c] = pValues[r][c][0];
							RemainingCells--;
							pValues[r][c].clear();
							applyAlgorithm(r,c,board[r][c],pValues,board);
						}
					}
				}
			}
		}else{
			//R1 C1
			int c = 0;
			for(r = 0 ;r<3; r++){
				for(c = 0 ;c<3; c++){
					if(!isFilled(board[r][c])){
						deleteValueFromVector(pValues[r][c],value);
						if(pValues[r][c].size()==1){
							//fill cell with the remaining value
							board[r][c] = pValues[r][c][0];
							RemainingCells--;
							pValues[r][c].clear();
							applyAlgorithm(r,c,board[r][c],pValues,board);
						}
					}
				}
			}
		}
	}
}

void applyAlgorithm(int r, int c, int value, vector<int>pValues[][9], int board[][9]){
	EliminateValueInRowAndCol(r,c,board[r][c],pValues,board);
	EliminateValueInBox(r,c,board[r][c],pValues,board);
}

bool isPossibilityContradiction(int value, int row, int column, vector<int>pValues[][9]){
	for(int i=0; i<9; i++){
		if(i!=column){
			for(auto e: pValues[row][i]){
				if(e == value){
					return true;
				}
			}
		}
	}
	for(int i=0; i<9; i++){
		if(i!=row){
			for(auto e: pValues[i][column]){
				if(e == value){
					return true;
				}
			}
		}
	}
	return false;
}

bool isSafeCell(int r, int c, const int board[][9]){
	for(int i=0; i<9; i++){
		if(i!=c){
			if(board[r][c] == board[r][i]){
				return false;
			}
		}
	}
	for(int i=0; i<9; i++){
		if(i!=r){
			if(board[r][c] == board[i][c]){
				return false;
			}
		}
	}
	return true;
}

bool tryForcing(int r, int c, int board[][9], vector<int>pValues[][9], vector<pair<int,pair<int,int>>>remainingPosibilities){
	//base case
	DisplayBoard(board);
	if(RemainingCells>0 && remainingPosibilities.size()==0){
		return false;
	}
	for(int r=0; r<9; r++){
		for(int c=0; c<9; c++){
			for(auto e: pValues[r][c]){
				if(!isPossibilityContradiction(e,r,c,pValues)){
					board[r][c] = e;
					pValues[r][c].clear();
					applyAlgorithm(r,c,board[r][c],pValues,board);
				}
			}
		}
	}
	if(remainingPosibilities[0].first == 0 && RemainingCells==0){
		return true;
	}
	int currentR, currentC;
	currentR = remainingPosibilities[0].second.first;
	currentC = remainingPosibilities[0].second.second;
	for(auto e: pValues[currentR][currentC]){
		board[currentR][currentC] = e;
		vector<int>temp = pValues[currentR][currentC];
		pValues[currentR][currentC].clear();
		remainingPosibilities.clear();
		for(int i=0; i<9; i++){
			for(int j=0; j<9; j++){
				if(pValues[i][j].size()!=0){
					remainingPosibilities.push_back(make_pair(pValues[i][j].size(),make_pair(i,j)));
				}
			}
		}
		sort(remainingPosibilities.begin(),remainingPosibilities.end(),leastPossibility);
		applyAlgorithm(currentR,currentC,e,pValues,board);
		if(tryForcing(currentR,currentC,board,pValues,remainingPosibilities)){
			return true;
		}else{
			board[currentR][currentC] = 0;
			pValues[currentR][currentC] = temp;
			remainingPosibilities.push_back(make_pair(pValues[currentR][currentC].size(),make_pair(currentR,currentC)));
		}
	}
}