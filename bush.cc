//Made by Jonathon Lefler and Peter Maurer

#include <iostream> //cout
#include <unistd.h> //getcwd
#include <cstring> //strtok
#include <string> //insert

int loadCwPath(std::string * &cwPath){ //uses getcwd() to load cwPath
	char buf[100];
	char * charPathElement;
	getcwd(buf, 100); //gets current working directory

	int count = 0;
	for(int i = 0; i < sizeof(buf); ++i)
		if(buf[i] == '/')
			++count; //count is the size of cwPath
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
	std::string line = ""; //line will hold the entire command(cmd).
	std::string * cwPath; //cwPath will hold the current working path
	int cwPathLength = loadCwPath(cwPath);
	//Will start from current working directory, can be got from loadCwPath

	while(true){
		printPrompt(cwPathLength, cwPath);
		std::getline(std::cin, line);
		std::cout << "Command:" << line << std::endl;
		if(line.compare("quit") == 0){ //comapre() is in std::string
			std::cout << "quiting" << std::endl;
			break;
		}
		if(line.substr(0, 2) == "cd")
			std::cout << "You're cd'ing" << std::endl;
	}
}
