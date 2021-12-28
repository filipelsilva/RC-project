#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <csignal>
#include <iostream>
#include <fstream>
extern "C" {
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <dirent.h>
}


bool isNumber(std::string str){
	for(int i = 0; i < str.length(); i++)
      	if(! (str[i] >= '0' && str[i] <= '9') ) 
      		return false;

	return true;
}

bool isAlNum(std::string str){
	int i;

	for (i=0; i<8; i++)
		if (!isalnum(str[i]))
			return false;
	return true;
}

bool validPass(std::string pass){
	if (pass.length()==8 && isAlNum(pass))
		return true;
	return false;
}

bool validUID(std::string UID){
	if (isNumber(UID) && UID.length()==5)
		return true;
	return false;
}

/*Falta enviar os códigos, aka dar returns*/
void reg(std::string UID, std::string pass){
	std::string path="../USERS";
	std::string passFile_name = "_pass.txt";
	DIR *dir;
	struct dirent *diread;

	/*Provavelmente pode tornanr-se num && uma vez que a exceção é a mesma nos dois casos*/
	if (validUID(UID)){
		dir = opendir(path.c_str());

		path.append("/"); path.append(UID);

		/*verifica se NÃO existe uma diretoria com o mesmso UID, FALTA DAR RETURN*/
		while((diread = readdir(dir)) != nullptr){
			if(diread->d_name == UID)
				std::cout << "UID used already" << std::endl;
		}

		/*Cria nova diretoria*/
		mkdir(path.c_str(), 0777);
		path.append("/");

		/*Cria o ficheiro com a passe*/ 
		if(validPass(pass)){


			passFile_name.insert(0, UID);
			std::ofstream passFile(path.append(passFile_name));
			passFile << pass << std::endl;

			passFile.close();
			
		}
		else 
			std::cout << "Invalid password" << std::endl;	
	}

	else
		std::cout << "Invalid UID" << std::endl;
}



int main(void){
	reg ("12346", "abcd1234");

	exit(0);
}