/*Made by Jonathon Lefler and Peter Maurer/
//Start date: October 23, 2017            /
//This program emulates bash.             /
//Functionality includes:                 /
//---------------------------------------*/

#include <iostream> /* cout */
#include <unistd.h> /* getcwd, chdir */
#include <cstring> /* strtok, strlen */
#include <string> /* insert */
#include <stdio.h> /* printf */
#include <cstdlib> /* getenv */
#include <errno.h> /* errono */
#include <string.h> /* strdup */
#include <pwd.h> /* Used to access /etc/passwd */

int countElements(char delim, char * buf){
	int count = 0;
        for(int i = 0; i < strlen(buf); ++i)
                if(buf[i] == delim)
                        ++count; /* count is the size of cwPath */
	return count;
}

void errorCheck(){
	if(errno != 0)
		std::cout << errno << std::endl;
	perror("Error");
	exit(0);
}

int loadCwPath(std::string * &cwPath, char * cwd2){ /* Loads cwPath in main */
	char * charPathElement;
	int count;
	char cwd[strlen(cwd2)];
	strcpy(cwd, cwd2);

	count = countElements('/', cwd); /* Count is the size of cwPath */
	charPathElement = strtok(cwd, "/");

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

void printPrompt(int cwPathLength, std::string * cwPath, char * user, char * manager){
	std::cout << "\033[1;32m" << user << "@" << manager << "\033[0m  ";
        for(int i = 0; i < cwPathLength; ++i){
			if(i == 0){
				std::string home = cwPath[i];
				home += cwPath[i + 1];
				if(home.compare(getenv("HOME")) == 0){
					std::cout << "\033[1;34m~" << "\033[0m";
					i = 1;
					continue;
				}
			}
			std::cout << "\033[1;34m" << cwPath[i] << "\033[0m";
		}
	std::cout << " g-shell >";
}

int changeCwDir(std::string * &cwPath, int cwPathLength){
	/* When changing dir, should change "PWD" in env using setenv */
	std::string tempString = "";
	for(int i = 0; i < cwPathLength; ++i)
		tempString += cwPath[i];
	if(chdir(tempString.c_str()) != 0)
		errorCheck();
	char * temp2 = strdup((tempString).c_str());
	if(setenv("PWD", temp2, 1) != 0)
		errorCheck();
	delete(temp2);
}

void printEnv(std::string envVar){
	extern char ** environ;
	int i = 0;
	if(envVar == "")
		while(environ[i] != NULL)
			std::cout << environ[i++] << std::endl;
	else
		while(environ[i] != NULL){
			if(std::string(environ[i]).find(envVar) == 0){
				std::cout << environ[i] << std::endl;
				break;
			}
			++i;
		}	
}

int main(int argc, char * argv[]){
	std::string line = ""; /*line will hold the entire command array. */
	std::string * validCmdArray; /* valid commands to check against */
	std::string * cwPath; /*cwPath will hold the current working path */
	char cwPathBuf[256]; /* used to load cwPath */
	char * user, host[256]; /*user is the current user, host is the host */
	struct passwd * pass; /* struct to get user */
	char * path, * cwd; /*path, cwd holds current working directory */
	int cwPathLength; /*cwPathLength will hold the
			            current working path array size*/
	/* Getting path, *CANNOT* use getenv */
	path = getenv("PATH");

	/* Getting CWD(current working directory), and loading it into cwPath */
	cwPath = new std::string[24]; /* 24 represents the depth of our path */
	/* cwd = getenv("PWD"); This is not realiable */
	cwPathLength = loadCwPath(cwPath, getcwd(cwPathBuf, 256));

	/* Getting user and host */
	if(gethostname(host, 256) != 0)
		errorCheck();
	pass = getpwuid(getuid()); /* Citation 2 */
	user = pass->pw_name;

	while(true){
		std::string * cmdArray; /*Holds all of the commands */
		char buf[1000];
		char * charCmdElement; /* Used to strtok() input to load cmdArray */

		printPrompt(cwPathLength, cwPath, user, host);
		std::getline(std::cin, line);
		std::cout << "Command:" << line << std::endl;

		std::strcpy(buf, line.c_str());
		int numCmdElements = countElements(' ', buf);
		//if(numCmdElements == 0)
			++numCmdElements;
		cmdArray = new std::string[numCmdElements];

		int i = 0;
		charCmdElement = strtok(buf, " ");
		while(charCmdElement != NULL){
			std::string stringCmdElement = charCmdElement;
			cmdArray[i] = stringCmdElement;
			++i;
			charCmdElement = strtok(NULL, " ");
		} /* compare is in std::string */
		if(cmdArray->compare("quit") == 0){ /* Start quit command */
			std::cout << "quiting" << std::endl;
			//delete(cmdArray);
			//delete(cwPath);
			exit(0);
		} /* End quit command */
		else if(cmdArray->compare("cd") == 0){ /* Start cd command */
			if(numCmdElements == 1 || cmdArray[1].compare("~") == 0){ /* just "cd", which means the same thing as "cd .." */
				cwPathLength = loadCwPath(cwPath, getenv("HOME"));
				changeCwDir(cwPath, cwPathLength);
				std::cout << "cd command 1" << std::endl;
			}
			else if(cmdArray[1].compare("..") == 0){
				cwPath[cwPathLength] = "";
				--cwPathLength;
				changeCwDir(cwPath, cwPathLength);
				std::cout << "cd command 2" << std::endl;
			}
			else{
				++cwPathLength;
				cmdArray[1].insert(0, "/");
				cwPath[cwPathLength] = cmdArray[1];
				changeCwDir(cwPath, cwPathLength);
				std::cout << "cd command 3" << std::endl;
			}
		} /* End cd command */
		else if(cmdArray->compare("env") == 0){ /* Start env command */
			if(numCmdElements == 2){
				printEnv(std::string(cmdArray[1]));
			}
			else
				printEnv("");
		} /* End env command  */
	}
}
