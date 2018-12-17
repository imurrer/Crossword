//crossword.cpp
//Isobel Murrer
//11/13/18
//Main Driver

using namespace std;

#include "cross.h"

int main(int argc, char* argv[]) {
	Cross b1;
	bool wordsOk;
	bool allGood;
	if(argc == 1){								//user input mode
		wordsOk = b1.getWord();		
		if(wordsOk){							//checks if there are words to place
			b1.placeFirst();	
			allGood = b1.placeRest();
			if(allGood){								//displays output if there is a suitable crossword to create
				cout << b1;
			}
			else{
				cout << "Error: Can't place these words" << endl;
			}
		}
	}
	else if(argc == 2){						//uploads words from file
		ifstream ifs;
		string word;
		string filename = argv[1];	//assigns the second command line argument as the filename
		bool wordsOk;
		bool canPlace;
		ifs.open(filename);			//makes sure file is valid
		if(!ifs) {
			cout << "Error opening file " << filename << endl;
			return 1;
		}
		while(ifs.peek() != EOF){						//reads in words from file until it detects end of file
			getline(ifs, word);
			wordsOk = b1.getWord2(word);
			if((word[0]=='.') && (!wordsOk)){
				return 1;
			}
		}
		b1.placeFirst();
		canPlace = b1.placeRest();
		if(canPlace){
			cout << b1;
		}
		else{
			cout << "Error: Can't place these words" << endl;
		}
	}
	else if(argc == 3){						//uploads words from file and transfers output to new file
		ifstream ifs;	
		ofstream ofs;
		string word;
		string filename = argv[1];	//assigns the second command line argument as the filename
		string outfilename = argv[2];
		bool wordsOk;
		bool canPlace;
		ifs.open(filename);			//makes sure file is valid
		if(!ifs) {
			cout << "Error opening file " << filename << endl;
			return 1;
		}
		while(ifs.peek() != EOF){
			getline(ifs, word);
			wordsOk = b1.getWord2(word);
			if((word[0]=='.') && (!wordsOk)){
				return 1;
			}
		}
		b1.placeFirst();
		canPlace = b1.placeRest();
		if(canPlace){
			ofs.open(outfilename, ofstream::out);		//connect to output file
			if(!ofs) {														//make sure the file can be created
				cout << "error creating file" << outfilename << endl;
				return 2;
			}
			ofs << b1 << endl;	//save crossword info into file
			ofs.close();
		}
		else{
			cout << "Error: Can't place these words" << endl;
		}
	}
	else{
		cout << "Error: too many command line arguments" << endl;
	}
	return 0;
}
