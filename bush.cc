/*Made by Jonathon Lefler and Peter Maurer/
//Start date: October 23, 2017            /
//This program emulates bash.             /
//Functionality includes:                 /
//---------------------------------------*/

#include <iostream> //cout
#include <unistd.h> //getcwd
#include <cstring> //strtok, strlen
#include <string> //insert
#include <stdio.h> //printf

int countElements(char delim, char * buf){
	int count = 0;
        for(int i = 0; i < strlen(buf); ++i)
                if(buf[i] == delim)
                        ++count; //count is the size of cwPath
	return count;
}

int loadCwPath(std::string * &cwPath){ //uses getcwd() to load cwPath
	char buf[100];
	char * charPathElement;
	getcwd(buf, 100); //gets current working directory

	int count = countElements('/', buf); //count is the size of cwPath
	std::cout << count << std::endl;
	cwPath = new std::string[24]; //24 represents the depth of our path
	charPathElement = strtok(buf, "/");
	int i = 0;
	while(charPathElement != NULL){
		std::string stringPathElement = charPathElement;
		stringPathElement.insert(0, "/");
		cwPath[i] = stringPathElement;
		++i;
		charPathElement = strtok(NULL, "/");
	}
	return count;
}

void printPrompt(int cwPathLength, std::string * cwPath){
        for(int i = 0; i < cwPathLength; ++i)
                std::cout << "\033[1;34m" << cwPath[i] << "\033[0m";
	std::cout << " bush $>";
}

int main(int argc, char * argv[]){
	std::string line = ""; /*line will hold the entire command array. */
	std::string * cwPath; /*cwPath will hold the current working path */
	int cwPathLength = loadCwPath(cwPath); /*cwPathLength will hold the
						 current working path array size*/
	/*Stack overflow link number1
	  This prints the environment */
	extern char **environ;
	int pathIndex = 0;
	while(environ[pathIndex]) {
		if(std::string(environ[pathIndex]).find("PATH=") == 0){
			std::cout << environ[pathIndex] << std::endl;
			break; /* pathIndex now holds our index for PATH */
		}
		++pathIndex;
	}

	while(true){
		std::string * cmdArray; /*Holds all of the commands */
		char buf[1000];
		char * charCmdElement; /* Used to strtok() input to load cmdArray */

		printPrompt(cwPathLength, cwPath);
		std::getline(std::cin, line);
		std::cout << "Command:" << line << std::endl;

		std::strcpy(buf, line.c_str());
		int numCmdElements = countElements(' ', buf);
		cmdArray = new std::string[numCmdElements + 1];

		int i = 0;
		charCmdElement = strtok(buf, " ");
		while(charCmdElement != NULL){
			std::string stringCmdElement = charCmdElement;
			cmdArray[i] = stringCmdElement;
			++i;
			charCmdElement = strtok(NULL, " ");
		}
		if(line.compare("quit") == 0){ /*compare() is in std::string */
			std::cout << "quiting" << std::endl;
			break;
		}
		if(line.substr(0, 2) == "cd")
			std::cout << "You're cd'ing" << std::endl;
	}
}
