#include <stdio.h>
#include <iostream.h>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <limits>

//this grid is made to put the final answers in it
std::vector<std::vector<int> > sudoku;
//this grid is used to put the remaining possible correct answers in 0 and 1 bits
std::vector<std::vector<int> > sudokuAnswers;
//this is only a loops counter indication
int turn = 0;

//weight calculation (number of 0 in the sudokuAnswers case)
int weight(int x) {
	int tmp=0;
	while (x>0) {
		if(x&1L) tmp++;
		x=x>>1;
	}//while
	return 9-tmp;
}//weight

//number 0000010000 is related to 2^4
int power (int n, int m) {
	//because I need 2^0 to give 0 not to alter my answers possibilites
	if (m==0) return 0;
	//else :
	int c=1, result=n;
	while (c<m){
		c++;
		result*=n;
	}//while
	return result;
}//power

//to fill the grid with the file given
std::vector<std::vector<int> > firstFillGrid (const char *text){
	std::vector<std::vector<int> > mat;
	//This is the C++ allocation function for class vector
	mat.resize(9);
	for(int i=0 ; i < mat.size() ; i++)
		mat[i].resize(9);
	FILE *file;
	file = fopen(text, "r");
	for(int i=0 ; i < mat.size() ; i++)
		for (int j=0; j<9; j++) {
			fscanf(file, "%d", &mat[i][j]);
		}//for
	fclose(file);
	return mat;
}//firstFillGrid

//to display the grid in a "sudoku style"
void displayData (std::vector<std::vector<int> >  tab, int N, int M){
	for (int i = 0; i<N; i++) {
		for (int j = 0; j<M; j++) {
			cout <<  tab[i][j] << "   ";
			if (j%3==2) cout << "|  ";
		}//for j
	cout << endl;
	if (i%3==2) cout << "____________________________________________\n";
	}//for i
}//displayData

//control functions
//useLine get bits away from a given sudokuAnswers case corresponding to the items already present on the line
int useLine (std::vector<std::vector<int> > sudoku, int sudokuAnswers, int i, int j){
	for (int k=0; k<9; k++) 
		sudokuAnswers = (sudokuAnswers | power(2, sudoku[i][k]));
	return sudokuAnswers;
}//useLine
int useColumn (std::vector<std::vector<int> > sudoku, int sudokuAnswers,int i, int j){
	for (int k=0; k<9; k++)
		sudokuAnswers = (sudokuAnswers | power(2, sudoku[k][j]));
	return sudokuAnswers;
}//useColumn
int useSquare (std::vector<std::vector<int> > sudoku, int sudokuAnswers, int i, int j){
	for (int k=((i/3)*3); k<(((i/3)*3)+3); k++)
		for (int l=((j/3)*3); l<(((j/3)*3)+3); l++)
			sudokuAnswers = (sudokuAnswers | power(2, sudoku[k][l]));
	return sudokuAnswers;
}//useSquare
int checkCaseDone (int bin){
	switch (bin) {
//good answers look like 1111110110
//(the last bit is useless)
		case 1020 : return 1;
		case 1018 : return 2;
		case 1014 : return 3;
		case 1006 : return 4;
		case 990 : return 5;
		case 958 : return 6;
		case 894 : return 7;
		case 766 : return 8;
		case 510 : return 9;
		//there are still several possibilities for the case
		default : return 0;
	}
}

int aResolutionLoop (int numberOfFilledCases){
	cout << "loop number   : " << turn++ << endl;
	cout << " cases filled  :  " << numberOfFilledCases << endl;
//	if (turn > 60) return 0;
	//this int is usefull for the backtracking
	int numberOfFilledCasesBefore = numberOfFilledCases;
	/** reduce for each case the weight by adding the items already present on line, colulmn or square */
	for (int i=0; i<9; i++)
		for (int j=0; j<9; j++)
			if (sudoku [i][j] == 0) {
				sudokuAnswers [i][j] =  useLine (sudoku, sudokuAnswers[i][j], i, j);
				sudokuAnswers [i][j] =  useColumn (sudoku, sudokuAnswers[i][j], i, j);
				sudokuAnswers [i][j] =  useSquare (sudoku, sudokuAnswers[i][j], i, j);
				int answer = checkCaseDone (sudokuAnswers [i][j]);
				if (answer > 0) {
					sudoku [i][j] = answer;
					sudokuAnswers[i][j] = -1;
					numberOfFilledCases++;
					//if an answer is given an update has to be performed on the related line column and square bits
					for (int l=0; l<9; l++) { 
						sudokuAnswers[l][j]=(sudokuAnswers[l][j] | power(2, answer));
						sudokuAnswers[i][l]=(sudokuAnswers[i][l] | power(2, answer));
					}//for l
					for (int k=((i/3)*3); k<(((i/3)*3)+3); k++)
					for (int l=((j/3)*3); l<(((j/3)*3)+3); l++)
						sudokuAnswers[k][l]=(sudokuAnswers[k][l] | power(2, answer));
				}//if answer
			}//if the case ain't filled yet
	/**checkPresenceOnlyOnceOnLine : */
	//every item has to stand on every line
	for (int i=0; i<9; i++)	
	for (int k=1; k<10; k++) {
		//for every case on line i
		int once = 0;
		int tmp = -1;
		for (int j=0; j<9; j++) 
			if (sudoku[i][j] == 0) {
				if ((power(2, k) & sudokuAnswers[i][j]) != power(2, k)) {
					once++;
					tmp = j;
				}//if it's possible to add item k here
			}//if case ain't filled yet
		if (once == 1) {//number k is present only once : in case tmp
			sudoku [i][tmp] = k;
			sudokuAnswers[i][tmp] = -1;
			numberOfFilledCases++;
			for (int l=0; l<9; l++) { 
				sudokuAnswers[l][tmp]=(sudokuAnswers[l][tmp] | power(2, k));
				sudokuAnswers[i][l]=(sudokuAnswers[i][l] | power(2, k));
			}//for l
			for (int p=((i/3)*3); p<(((i/3)*3)+3); p++)
			for (int l=((tmp/3)*3); l<(((tmp/3)*3)+3); l++)
				sudokuAnswers[p][l]=(sudokuAnswers[p][l] | power(2, k));
		}//if there was only a possibility where to put the item
	}//for i k
	/**checkPresenceOnlyOnceOnColumn : */
	for (int i=0; i<9; i++)	
	for (int k=1; k<10; k++) {
		//for every case on column i
		int once = 0;
		int tmp = -1;
		for (int j=0; j<9; j++) 
			if (sudoku[j][i] == 0)
				if ((power(2, k) & sudokuAnswers[j][i]) != power(2, k)) {
					once++;
					tmp = j;
				}//if
		if (once == 1) {//number k is present only once : in case tmp
			sudoku [tmp][i] = k;
			sudokuAnswers[tmp][i] = -1;
			numberOfFilledCases++;
			for (int l=0; l<9; l++) { 
				sudokuAnswers[tmp][l]=(sudokuAnswers[tmp][l] | power(2, k));
				sudokuAnswers[l][i]=(sudokuAnswers[l][i] | power(2, k));
			}//for l
			for (int p=((tmp/3)*3); p<(((tmp/3)*3)+3); p++)
			for (int l=((i/3)*3); l<(((i/3)*3)+3); l++)
				sudokuAnswers[p][l]=(sudokuAnswers[p][l] | power(2, k));
		}//if
	}//for i k 
	/**checkPresenceOnlyOnceOnSquare : */
	for (int i=0; i<9; i+=3) for (int j=0; j<9; j+=3)
	for (int k=1; k<10; k++) {
		int once = 0;
		int tmp = -1, tmp2 = -1;
		//for every case on square
		for (int p=((i/3)*3); p<(((i/3)*3)+3); p++)
			for (int l=((j/3)*3); l<(((j/3)*3)+3); l++)
			if (sudoku[p][l] == 0)
				if ((power(2, k) & sudokuAnswers[p][l]) != power(2, k)) {
					once++;
					tmp = p;
					tmp2 = l;
				}//if
		if (once == 1) {//number k is present only once in case tmp
			sudoku [tmp][tmp2] = k;
			sudokuAnswers[tmp][tmp2] = -1;
			numberOfFilledCases++;
			for (int l=0; l<9; l++) { 
				sudokuAnswers[tmp][l]=(sudokuAnswers[tmp][l] | power(2, k));
				sudokuAnswers[l][tmp2]=(sudokuAnswers[l][tmp2] | power(2, k));
			}//for l
			for (int p=((tmp/3)*3); p<(((tmp/3)*3)+3); p++)
			for (int l=((tmp2/3)*3); l<(((tmp2/3)*3)+3); l++)
				sudokuAnswers[p][l]=(sudokuAnswers[p][l] | power(2, k));
		}//if once
	}//for i k
	//the last loop had no effect
	if (numberOfFilledCasesBefore == numberOfFilledCases){
	/**
	A backtracking method is bound
	*/
	cout << "I'm performing a backtracking\n";
	int ii, jj, w=9;
	//let's choose the case in which the least solutions are possible
	for (int i=0; i<9; i++) for (int j=0; j<9; j++) {
		if ((sudoku [i][j] == 0) && (weight(sudokuAnswers[i][j]) < w)) {
			w = weight(sudokuAnswers[i][j]); 
			//stop condition on backtracking
			if(w==0) {
				cout << "stop condition\n";
				return 0;
			}//if
			ii = i; 
			jj = j;
		}//if
	}//for 
	for(int m = 1; m < 10; m++) {
		//m bit (at 0) is in case [ii][jj]
		if((power(2,m) & ~(sudokuAnswers[ii][jj])) == power(2,m)) {
			cout << "I try Item  " << m << " in case : " << ii << " " << jj << endl;
			std::vector<std::vector<int> > sudokuMem;
			std::vector<std::vector<int> > sudokuAnswersMem;
			sudokuMem.resize(9);
			sudokuAnswersMem.resize(9);
			for (int i=0; i<9; i++){
				//safeguard
				sudokuAnswersMem[i].resize(9);
				sudokuMem[i].resize(9);
				for (int j=0; j<9; j++) {
					sudokuMem[i][j] = sudoku[i][j];
					sudokuAnswersMem[i][j] = sudokuAnswers[i][j];
				}//for j
			}//for i
			int numberOfFilledCasesMem = numberOfFilledCases;
			//get this bit off
			sudoku[ii][jj] = m;
			sudokuAnswers[ii][jj] = -1;
			numberOfFilledCases++;
			for (int l=0; l<9; l++) { 
				sudokuAnswers[ii][l]=(sudokuAnswers[ii][l] | power(2, m));
				sudokuAnswers[l][jj]=(sudokuAnswers[l][jj] | power(2, m));
			}//for l
			for (int p=((ii/3)*3); p<(((ii/3)*3)+3); p++)
			for (int l=((jj/3)*3); l<(((jj/3)*3)+3); l++)
				sudokuAnswers[p][l]=(sudokuAnswers[p][l] | power(2, m));
			numberOfFilledCases = aResolutionLoop (numberOfFilledCases);
			if(numberOfFilledCases < 81) {
				cout << "I'm goin reverse...\n";
				//old values have to be reused
				for (int i=0; i<9; i++)for (int j=0; j<9; j++) {
					sudoku[i][j] = sudokuMem[i][j];
					sudokuAnswers[i][j] = sudokuAnswersMem[i][j];
				}//for i j
				numberOfFilledCases = numberOfFilledCasesMem;
			}//if not finished
		}//if m is in case
	}//for m
	}//if backtracking
	//recursion
	if ((numberOfFilledCases < 81) && (numberOfFilledCasesBefore < numberOfFilledCases)) 
		numberOfFilledCases = aResolutionLoop(numberOfFilledCases);
	numberOfFilledCasesBefore = numberOfFilledCases;
	return numberOfFilledCases;
}

int main(int argc, char ** argv){

	if (argc<2) {cout << "execution : $./a.out [file containing sudoku grid to fill]\n"; return 0;}
/* 
	to find out what are the possible solutions in sudokuAnswers
	int sol = 0;
	for (int i=1; i<10; i++) sol+= power(2, i);
	//display possible solutions :
	cout << "sol  ==> " << sol << endl;
	for (int i=1; i<10; i++) 
	cout << "sol " << i << " : " << sol - power(2, i) << endl;
*/
	//the matrix is filled with the test items
	sudoku = firstFillGrid(argv[1]);
	//another matrix is created to fill with possible answers corresponding to every case 0 of sudoku
	int numberOfFilledCases = 0;
	sudokuAnswers.resize(9);
	for (int i=0; i<9; i++){
		sudokuAnswers[i].resize(9); 
		for (int j=0; j<9; j++) {
			if (sudoku [i][j] == 0) sudokuAnswers[i][j]=0;
			else {
				sudokuAnswers[i][j]=-1;
				numberOfFilledCases++;
			}//else
		}//for j
	}//for i
	numberOfFilledCases = aResolutionLoop(numberOfFilledCases);

	displayData (sudoku, 9, 9);
	return 1;
}//main

