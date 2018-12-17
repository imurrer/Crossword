//cross.cpp
//Isobel Murrer
//11/13/18
//The implementation of the Cross class

using namespace std;

#include "cross.h"

Cross::Cross() {
	for(int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			answerboard[i][j] = '.';		//initialize the answer and puzzle boards
			puzzleboard[i][j] = '#';
		}
	}
}

Cross::~Cross() {  }

//method to get words from user input
bool Cross::getWord() {			
	bool wordsOk = true;
	bool end = false;
	int wordcount = 1;
	string word;
	cout << "Enter a list of words followed by a period:" << endl;
	getline(cin, word);
	while(!end && (wordcount < 20)){
		BEGIN:{}
		if(word[0] == '.'){			//assume user doesn't enter a space before the terminal period, stops reading in words
			end = true;
			if(wordVec.size() == 1 || wordVec.size()==0){		//quits program if only one word or no valid words were entered
				cout << "Error: Not enough inputs" << endl;
				wordsOk = false;
				goto END;
			}
			break;
		}
		else if(word.size() > 15 || word.size() < 2){					//ignores a word if it is longer than 15 or fewer than 2 letters
			cout << "Error - Word must be between 2 to 15 letters long" << endl;
			getline(cin, word);
			goto BEGIN;
		}
	  for(int i=0; i<word.size(); i++){
		  if(!isalpha(word[i])){		
			  cout << "Error - Enter only alphabetical characters for input" << endl;		//ignores words with non-alphabetical characters
				getline(cin, word);
				goto BEGIN;
		  }
			else			
				word[i] = toupper(word[i]);
		}
		wordVec.push_back(word);	//only adds word to vector if it doesn't produce an error	
		if(!end){									//gets a new word if the user isn't done entering words
			getline(cin,word);
			wordcount++;
		}
  }

	sortWords(wordsOk);		//calls sorting method
	
	for(int i=0; i<wordVec.size(); i++){	//creates a second vector of the words
		wordVec2.push_back(wordVec[i]);
	}
	END: return wordsOk;
}

//method to read words from a file
bool Cross::getWord2(string word) {		
	bool wordsOk = true;
	if(word[0] == '.'){
		if(wordVec.size()==1 || wordVec.size() == 0){ 	//quits program if only one word or no valid words were entered
			cout << "Error: Not enough inputs" << endl;
			wordsOk = false;
			goto END;
		}
		else{												//creates second word vector if the first one is valid and done being read in
			for(int i=0; i<wordVec.size(); i++){
				wordVec2.push_back(wordVec[i]);
			}
		}
		goto END;
	}
	if(wordVec.size() < 20){						//runs unless 20 words have been added to the vector
		if(word.size() > 15 || word.size() < 2){	
			cout << "'" << word << "': " << "Error - Word must be between 2 to 15 letters long" << endl;
			wordsOk = false;
			goto END;
		}
	  for(int i=0; i<word.size(); i++) {
		  if(!isalpha(word[i])){
			  cout << "'" << word << "': " <<  "Error - Enter only alphabetical characters for input" << endl;
				wordsOk = false;
			  goto END;	
		  }
			else
				word[i] = toupper(word[i]);
		}
	}
	wordVec.push_back(word);
	
	if(wordVec.size()>1)		//calls sort method if there are more than 1 valid inputs
		sortWords(wordsOk);
	
	END: return wordsOk;
}

//sort method with indexing
void Cross::sortWords(bool wordsOk){
		if(wordsOk){
		int change = 1;
		bool needChange=true;
		while(change>0 || needChange){
			needChange = false;
			CHECK:{}
	  	for(int i=0; i < wordVec.size()-1; i++){
				change = 0;
		  	if((wordVec[i]).size() < ((wordVec[i+1]).size())){	
			  	string first = wordVec[i];
					wordVec[i] = wordVec[i+1];
					wordVec[i+1] = first;
			  	change++;
					goto CHECK;						//continues to check through the words until the word is larger than all following words
				}
			}
			for(int j=1; j<wordVec.size()-1; j++){			//makes sure the first word is larger than the rest
				if(wordVec[0].size()<wordVec[j].size()){
					needChange = true;
					break;
				}
			}
		}
	}
}

//method to place the first word
void Cross::placeFirst() {
	string word = wordVec[0];
	int startCol = (SIZE-word.size())/2;		//calculates proper starting position for word
	int startRow = SIZE/2;
	char firstX = startRow;
	char firstY = startCol;
	clueVec.push_back(firstY);							//stores the word's starting position for the clues
	clueVec.push_back(firstX);
	clueVec.push_back('A');									//first word is always across
	for(int i=0; i<word.size(); i++){								//place word
		answerboard[startRow][startCol] = word[i];
		puzzleboard[startRow][startCol] = ' ';
		startCol++;
	}
}

//Find where to place the rest of the words
bool Cross::placeRest() {
	bool allGood = false;		
	bool canPlace = false;	
	bool match;
	bool horizontal;
	bool room;
	bool empty;
	int addition = 1;
	while(addition>0){			//runs while an addition is made to the board
		addition = 0;
		for(int i=1; i<wordVec.size(); i++){		//runs through all words in vector
			x=0;																	//starts checking board at 0,0
			y=0;
			for(int j=0; j<wordVec[i].size(); j++){			//runs for the number of letters in word
				while(!(x==14 && y==14)&& !canPlace){
					match = nextLetterMatch(wordVec[i], j);
					if(match){															//check for a letter match
						horizontal = checkHorizontal();
						if(horizontal){													//check if the word match is horizontal
							room = verticalRoom(wordVec[i]);
							if(room){																//check if there's room to place vertically
								empty = vertEmptySpace(wordVec[i]);	
								if(empty){															//check if spots around are empty
									vertPlaceLetters(wordVec[i]);					//places word
									canPlace = true;
									allGood = true;
									addition++;
									wordVec[i].erase();										//erase word from vector
									char firstX = x1;											//store coordinates of the first letter of the word
									char firstY = y1;
									clueVec.push_back(firstY);
									clueVec.push_back(firstX);
									clueVec.push_back('D');								//puts first letter coordinates and "down" in clue vector
								}
								else{ goto AFTER;}
							}
							else{ goto AFTER;}
						}
						else{
							VERTICAL:
							room = horizontalRoom(wordVec[i]);
							if(room){																//check if there's room to place horizontally
								empty = horEmptySpace(wordVec[i]);
								if(empty){														//check if spots around are empty
									horPlaceLetters(wordVec[i]);			
									canPlace = true;
									allGood = true;
									addition++;
									wordVec[i].erase();
									char firstX = x1;
									char firstY = y1;
									clueVec.push_back(firstY);
									clueVec.push_back(firstX);
									clueVec.push_back('A');
								}
							}
						}
						AFTER:{}
					}
				}
				if(canPlace){				//resets canplace for the next word or letter so while loop will run		
					canPlace = false;
					break;
				}
				if(j<wordVec[i].size()-1){	//if it hasn't checked all letters in a word, reset x and y
					x=0;
					y=0;
				}
				else if(j==wordVec[i].size()-1){	//if it is done checking a word, erases it from the wordVec
					cout << "'" << wordVec[i] << "' could not be placed" << endl;
					wordVec.erase(wordVec.begin()+i);
					wordVec2.erase(wordVec2.begin()+i);
					x=0;
					y=0;
					i--;
					break;
				}
			}
		}
	}
	return allGood;
}

//find letter matches
bool Cross::nextLetterMatch(string word, int a){		
	bool match = false;
		for(int i=x; i<SIZE; i++){						//starts at stored x value and next y value to go through board and look for a letter match
			if(i==x){
				for(int j=y+1; j<SIZE; j++){
					if(answerboard[i][j] == word[a]){
						same = a;
						x = i;								//stores letter match as x and y
						y = j;
						match = true;
						goto END;
					}
				}
			}
			else{
				for(int j=0; j<SIZE; j++){
					if(answerboard[i][j] == word[a]){
						same = a;
						x = i;
						y = j;
						match = true;
						goto END;
					}
				}
			}
		}
	if(!match){					//if no match is found, set x and y so that while loop will quit in word-placing method
		x=14;
		y=14;
		goto END;
	}
	END: return match;
}

//check if the matched word is horizontal
bool Cross::checkHorizontal(){		
	bool horizontal = false;
	char left = answerboard[x][y-1];
	char right = answerboard[x][y+1];
	if(isalpha(left) || isalpha(right))
		horizontal = true;

	return horizontal;
}

//check if there's room for the word to be placed vetically
bool Cross::verticalRoom(string word){
	bool room = false;
	int above = same;
	int below = word.size() - (same+1);
	int roomA = x;
	int roomB = SIZE - (x+1);
	if((above<=roomA) && (below<=roomB)){
		room = true;
		x1 = x - same;
		y1 = y;
	}
	return room;
}

//check if there's room for the word to be placed horizontally
bool Cross::horizontalRoom(string word){
	bool room = false;
	int left = same;
	int right = word.size() - (same+1);
	int roomL = y;
	int roomR = SIZE - (y+1);
	if((left<=roomL) && (right<=roomR)){
		room = true;
		x1 = x;
		y1 = y - same;

	}
	return room;
}

//check if spaces are empty around and where the word should be placed vertically
bool Cross::vertEmptySpace(string word){
	bool empty = true;
	if(x1 != 0){
		if(isalpha(answerboard[x1-1][y])){
			empty = false;
			goto LEAVE;
		}
	}
	for(int i=0; i<word.size(); i++){
		if((x1+i)!=x){
			if(y!=0){
				if(isalpha(answerboard[x1+i][y1-1]) || isalpha(answerboard[x1+i][y])){
					empty = false;
					goto LEAVE;
				}
			}
			if(y<15){
				if(isalpha(answerboard[x1+i][y1+1]) || isalpha(answerboard[x1+i][y])){
					empty = false;
					goto LEAVE;
				}
			}
		}
	}
	if((x1+word.size()-1) < 15){
		if(isalpha(answerboard[x1+word.size()][y])){
			empty = false;
			goto LEAVE;
		}
	}
	LEAVE: return empty;
}

//check if spaces are empty around and where the word should be placed horizontally
bool Cross::horEmptySpace(string word){
	bool empty = true;
	if(y1 != 0){
		if(isalpha(answerboard[x][y1-1])){
			empty = false;
			goto LEAVE;
		}
	}
	for(int i=0; i<word.size(); i++){
		if((y1+i)!=y){
			if(x!=0){
				if(isalpha(answerboard[x1-1][y1+i]) || isalpha(answerboard[x][y1+i])){
					empty = false;
					goto LEAVE;
				}
			}
			if(x<15){
				if(isalpha(answerboard[x1+1][y1+i]) || isalpha(answerboard[x][y1+i])){
					empty = false;
					goto LEAVE;
				}
			}
		}
	}
	if((y1+word.size()-1) < 15){
		if(isalpha(answerboard[x][y1+word.size()])){
			empty = false;
			goto LEAVE;
		}
	}
	LEAVE: return empty;
}

//places a word vertically
void Cross::vertPlaceLetters(string word){
	for(int i=0; i<word.size(); i++){
		answerboard[x1+i][y1] = word[i];
		puzzleboard[x1+i][y1] = ' ';
	}
}

//places a word horizontally
void Cross::horPlaceLetters(string word){
		for(int i=0; i<word.size(); i++){
		answerboard[x1][y1+i] = word[i];
		puzzleboard[x1][y1+i] = ' ';
	}
}

//overload output operator
ostream& operator<< (ostream & os, Cross & b1 ){
	os << endl;
	os << "Answer: " << endl;						//displays answer board
	for(int i=0; i<SIZE; i++) {
		for(int j=0; j<SIZE; j++) {
			os << b1.answerboard[i][j];
		}
		os << endl;
	}
	os << endl;
	os << "Puzzle: " << endl;						//displays puzzle board
	for(int i=0; i<SIZE; i++) {
		for(int j=0; j<SIZE; j++) {
			os << b1.puzzleboard[i][j];
		}
		os << endl;
	}
	os << endl;
	
	int column = 0;	
	os << "Clues:" << endl;						//displays clues
	os << endl;
	for(int i=0; i<b1.wordVec2.size(); i++){
		random_shuffle((b1.wordVec2[i]).begin(), b1.wordVec2[i].end());		//won't shuffle certain words sometimes
	}
	int i=0;
	int Col=0;
	for(int j=0; j<b1.clueVec.size(); j++){								//displays starting letter coordinates for each word,
		if((j!=0) && (column!=0) && column%3 == 0){					//orientation, and anagram
			os << "  " << b1.wordVec2[i] << endl;
			i++;
		}
		if(b1.clueVec[j]==65){
			os << setw(8) << "Across";
			column++;
		}
		else if(b1.clueVec[j]==68){
			os << setw(8) << "Down";
			column++;
		}
		else{	
			if(Col==0){
				os << setw(3) << b1.clueVec[j] << ",";
				Col++;
			}
			else{
				os << setw(2) << b1.clueVec[j]; 
				Col = 0;
			}
			column++;
		}
	}
	os << "  " << b1.wordVec2[i];
	os << endl;

	return os;
}
