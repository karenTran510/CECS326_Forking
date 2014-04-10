//	Karen Tran
//	CECS 326, Section 1
//	Project 3: Fork and Wait
//	Project Description: The child processes will perform keyword searches. The user 
//	will enter a keyword, and the program will spawn a child using "fork()". The
//	child will do the search, and return the number of times the word occurs.
//	If the search renders unsuccessful, the child process will continuously loop through 
// 	the file, and prints out "." for each iteration of the file. The user must stop the 
//	endless loop manually in the terminal.

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <iostream>
#include <fstream> 
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std; 

//Converts 'string' into lowercase using std::tolower()
string toLowerCase(string &str) {
	int length = str.length(); 
	for(int i = 0; i < length; i++) {
		str[i] = tolower(str[i]);
	}
	return str; 	
}


int main() {

	//Declare document used for word search	
	ifstream file("chocolate.txt");
	string input; //Keyword to be searched
	string word; //Current word in stream
	int count=0; //Keeps track of the occurrences
	bool pass = true; //Used for while loop
	long childPID; //Stores child's PID

	//Run a keyword search repeatedly until the user enter "!wq"
	while(pass) {
		//Prompt user to enter a word to search
		cout << "\nEnter a word to search."<< endl;
		cin >> input;
		
		//If the user didn't want to quit, do a word search
		if(input != "!wq") {
			childPID = fork(); //Spawn a child process
			if(childPID == 0) { //Child will execute entire if-block
				while(file >> word) {
					word = toLowerCase(word);
					input = toLowerCase(input); 
					if(word == input){
						count++;
					}
				}
				//Print out the count
				cout << "Number of occurences: " << count << endl;
				
				//Injected bug. Will continuously run unless user interferes.
				if (count == 0) {
					for(int i = 0; i < 1; i++) {
						file.clear(); //Clear the buffer
						file.seekg(0,file.beg); //Set cursor to the beginning of file
						while(file >> word) {
							//Do nothing
						}
						cout << ".";
						i=-1;//Bug condition
					}
				}
			} 

			else if(childPID > 0) { //Parent process will execute entire else-if block
				wait(0); //Waits for the child process to finish
				pass = false; //Set the condition to have parent exit while-loop
				break; //Parent will break out of this block
			}
			//When child and parent is done, the file buffer and count will be resetted.
			count = 0;
			file.clear(); //Clear the buffer
			file.seekg(0,file.beg); //Set the cursor to beginning of file
		}
	
		else { //The user entered "!wq" and while-loop will end
			pass = false;
			break;
		}  
	}
	//Close the file and end program
	file.close();
	return 0;
}
