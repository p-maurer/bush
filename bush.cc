//Made by Jonathon Lefler and Peter Maurer

#include <iostream> //cout
//#include <string> //string

int main(int argc, char * argv[]){
	std::string line = ""; //line will hold the entire command(cmd).

	while(true){
		std::cout << "bush~$";
		std::getline(std::cin, line);
		std::cout << "Command:" << line << std::endl;
		if(line.compare("quit") == 0){
			std::cout << "quiting" << std::endl;
			break;
		}
	}
}
