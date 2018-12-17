//cross.h
//Isobel Murrer
//11/13/18
//Interface

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <fstream>
const int SIZE = 15;

class Cross {
		friend ostream& operator<< (ostream &, Cross &);
  public:
	  Cross();
		Cross(vector<string>);
		~Cross();
		bool getWord();
		bool getWord2(string);
		void sortWords(bool);
		void placeFirst();
		bool placeRest();
		bool nextLetterMatch(string, int);
		bool checkHorizontal();
		bool verticalRoom(string);
		bool horizontalRoom(string);
		bool vertEmptySpace(string);
		bool horEmptySpace(string);
		void vertPlaceLetters(string);
		void horPlaceLetters(string);
	private:
		char answerboard[SIZE][SIZE];
		char puzzleboard[SIZE][SIZE];
		vector<string> wordVec;
		vector<string> wordVec2;
		vector<int> clueVec;
		int same;
		int x;
		int y;
		int x1;
		int y1;
};

