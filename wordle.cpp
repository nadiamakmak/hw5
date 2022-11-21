// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
bool isInDictionary(const string& word, const set<string>& dict); //checks to see if the word is in the dictionary
set<string> createStrings(string& soFar, const string& in, const string& floating);
//void finalizeList(set<string>& all, const set<string>& dict, int pos);
 
// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    set<string> allStrings;
		string soFar = "";

		if(in==soFar){ //string is empty, return an empty set
			return allStrings; 
		}

		allStrings = createStrings(soFar, in, floating);

		/*cout << "All possible strings" << endl;
		for(set<string>::iterator it = allStrings.begin(); it != allStrings.end(); it++){
			string word = *it;
			if(word[0]=='r' && word[1]=='e'){
				cout << word << endl;
			}
		}
		cout << endl << endl;*/

		set<string> results;
    for(set<string>::iterator it = allStrings.begin(); it != allStrings.end(); it++){
        if(isInDictionary(*it, dict) == true){
          results.insert(*it);
        }
    }

		/*for(set<string>::iterator it = results.begin(); it != results.end(); it++){
			cout << *it << endl;
		}*/
  	//finalizeList(allStrings, dict, 0);
		
		return results;
}

// Define any helper functions here
set<string> createStrings(string& soFar, const string& in, const string& floating){ //rescursive function to generate all the possible strings
	set<string> results;
	
	//base case: one letter left to guess
	if((in.size()-soFar.size())==1){ 
		if(floating.size()==1){ //if we still have a floating at this point, it has to be the floating
			string tempword = soFar + floating;
			set<string> t = {tempword};
			return t;
		}
		else if(in[in.size()-1] != '-'){ //if the last character isn't a hyphen, then we already know what's in the last spot
			string tempword = soFar + in[in.size()-1];
			set<string> t = {tempword};
			return t;
		}
		else{ //if it's a hyphen, then it has to be something in the alphabet
			string alphabet = "abcdefghijklmnopqrstuvwxyz";
			set<string> newWords;
			for(int i = 0; i < 26; i++){
				string temp = soFar + alphabet[i];
				newWords.insert(temp);
			}
			return newWords;
		}
	}

	//not at the last letter, so we have to go through all possible options
	else{
		unsigned int x = soFar.size(); //starting from the position of the next letter we will be guessing
		unsigned int hyphens = 0;
		while(x!=in.size()){ //find out how many hyphens are left
			if(in[x]=='-'){
				hyphens++;
			}
			x++;
		}

		if(in[soFar.size()] != '-'){ //we know what should go here
			string tempword = soFar + in[soFar.size()];
			results = createStrings(tempword, in, floating);
		}

		else if(floating.size()==hyphens){ //there are as many floaters as there are letters to guess

			for(unsigned int i = 0; i < floating.size(); i++){ //for each of these floaters, make different combinations of them
				const string newFloating = floating.substr(0, i) + floating.substr(i+1);
				string tempWord = soFar + floating[i];
				set<string> tempResults = createStrings(tempWord, in, newFloating);
				results.insert(tempResults.begin(), tempResults.end());
			}

		}

		else{ //either a floating letter or a letter from the alphabet will go here
			string alphabet = "abcdefghijklmnopqrstuvwxyz";

			for(int i = 0; i < 26; i++){ //go through all letters in the alphabet

				if(floating.find(alphabet[i]) == string::npos){ //if the current letter is not in the list of floating letters add the letter from the alphabet and leave the floating
					string tempWord = soFar + alphabet[i]; 
					set<string> tempResults = createStrings(tempWord, in, floating);
					results.insert(tempResults.begin(), tempResults.end());
				}
				else{ //if the current letter is part of the floating letters, remove it from the floating letters to account for the fact that we're using it
					int index = floating.find(alphabet[i]);
					const string newFloating = floating.substr(0, index) + floating.substr(index+1);
					string tempWord = soFar + alphabet[i];
					set<string> tempResults = createStrings(tempWord, in, newFloating);
					results.insert(tempResults.begin(), tempResults.end());
				}

			}

		}
	}


	//the only level of the recursive call that will get here is the very first call (from the wordle() function)
	return results;
} 

/*void finalizeList(set<string>& all, const set<string>& dict, int pos){ //rescursively iterates removes all non-dictionary elements in the list
    if(pos == all.size()){
			return;
		}
		if(isInDictionary(all.at(pos), dict) == true){ //if its in the dictionary, advance the position and look at the rest of list
			pos++;
			finalizeList(all, dict, pos);
		}
		else{ //if it isnt, remove it from the list (don't advance the position (dynamic allocation))
			all.erase(all.at(pos));
			finalizeList(all, dict, pos);
		}
}*/

bool isInDictionary(const string& word, const set<string>& dict){ //checks to see if the word is in the dictionary
	if(dict.find(word) != dict.end()){
		//cout << "here";
		return true;
	}
	return false;
}
