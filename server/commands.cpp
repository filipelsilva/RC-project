#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <csignal>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
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

bool UID_free(std::string UID){
	std::string path = "../USERS";
	DIR *dir;
	struct dirent *diread;

	dir = opendir(path.c_str());

	while((diread = readdir(dir)) != nullptr){
		if(diread->d_name[0]=='.')
			continue;
		if(diread->d_name == UID)
			return false;
	}
	closedir(dir);

	return true;
}

void delete_files(std::string path){
	DIR *dir;
	struct dirent *diread;	
	dir = opendir(path.c_str());

	while((diread = readdir(dir)) != nullptr){
		if(diread->d_name[0]=='.')
			continue;
		std::string current_path = path;
		current_path.append("/"); current_path.append(diread->d_name);

		remove(current_path.c_str());
	}
}

bool correct_pass(std::string UID, std::string pass){
	std::string path="../USERS/";
	path.append(UID);

	std::string passFile_name = "_pass.txt";
	passFile_name.insert(0, UID);

	path.append("/"); path.append(passFile_name);

	std::ifstream passwordFile;
	std::string password;

	passwordFile.open(path.c_str());
	getline(passwordFile, password);

	if(password == pass)
		return true;
	return false;

}

int numberOfGroups(std::string path){
	int i = 0;
	DIR *dir;
	struct dirent *diread;

	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){
		if(diread->d_name[0]=='.')
			continue;
		if(strlen(diread->d_name)>2)
			continue;
		i++;
	}	
	closedir(dir);
	return i;
}

bool groupExists(std::string GID){
	std::string path = "../GROUPS";
	DIR *dir;
	struct dirent *diread;

	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){
		if(diread->d_name[0]=='.')
			continue;
		if(strlen(diread->d_name)>2)
			continue;
		if(diread->d_name == GID)
			return true;
	}
	return false;	
}

bool validGID(std::string GID){
	if(GID.length() == 2 && isNumber(GID))
		if(GID == "00" || groupExists(GID))
			return true;
	return false;
}

bool verifyGroupName(std::string GID, std::string gname){
	std::string path = "../GROUPS/";
	std::string gname_file = "_name.txt";

	path.append(GID); path.append("/");

	gname_file.insert(0,GID);

	path.append(gname_file);

	std::ifstream gNameFile;
	std::string name;

	gNameFile.open(path.c_str());
	getline(gNameFile, name);	

	if(gname == name)
		return true;
	return false;

}

/*REGISTER*/
/*Falta enviar os códigos, aka dar returns*/
void reg(std::string UID, std::string pass){
	std::string path="../USERS";
	std::string passFile_name = "_pass.txt";
	
	/*Provavelmente pode tornanr-se num && uma vez que a exceção é a mesma nos dois casos*/
	if (validUID(UID)){

		/*verifica se NÃO existe uma diretoria com o mesmso UID, FALTA DAR RETURN*/
		if(!UID_free(UID))
			std::cout << "UID used already" << std::endl;

		/*Cria o ficheiro com a passe*/ 
		if(validPass(pass)){

			path.append("/"); path.append(UID);

			/*Cria nova diretoria*/
			mkdir(path.c_str(), 0777);
			path.append("/");

			passFile_name.insert(0, UID);
			std::ofstream passFile(path.append(passFile_name));
			passFile << pass << std::endl;

			passFile.close();

			std::cout << "Registered successfully!" << std::endl;			
		}
		else 
			std::cout << "Invalid password" << std::endl;	
	}
	else
		std::cout << "Invalid UID" << std::endl;
}

/*UNREGISTER*/
void unr(std::string UID, std::string pass){
	std::string path="../USERS";

	if(validUID(UID) && validPass(pass)){
		if(!UID_free(UID)){
			path.append("/"); path.append(UID); 
			if(correct_pass(UID, pass)){

				delete_files(path);
				rmdir(path.c_str());

				std::cout << "Unregistered successfully!" << std::endl;
			}
			else
				std::cout << "Wrong password" << std::endl;
		}
		else
			std::cout << "No registration for that UID" << std::endl;
	}
	else
		std::cout << "Invalid UID or password" << std::endl;	
}

/*LOGIN*/
void login(std::string UID, std::string pass){
	std::string path="../USERS";
	std::string loginFile_name = "_login.txt";

	if(validUID(UID) &&  validPass(pass) 
		 && !UID_free(UID) && correct_pass(UID,pass)){

		path.append("/"); path.append(UID); path.append("/");
		loginFile_name.insert(0, UID);
		std::ofstream loginFile(path.append(loginFile_name));

		loginFile.close();

		std::cout << "Logged in successfully!" << std::endl;
	}
	else
		std::cout << "Incorrect UID or password" << std::endl;
}

/*LOGOUT*/
void logout(std::string UID, std::string pass){
	std::string path="../USERS";
	std::string loginFile_name = "_login.txt";

	if(validUID(UID) &&  validPass(pass) 
		 && !UID_free(UID) && correct_pass(UID,pass)){

		path.append("/"); path.append(UID); path.append("/");
		loginFile_name.insert(0, UID);
		path.append(loginFile_name);
		remove(path.c_str());

		std::cout << "Logged out successfully!" << std::endl;
	}
	else
		std::cout << "Incorrect UID or password" << std::endl;
}

/*GROUP LIST*/
void gls(){
	DIR *dir;
	struct dirent *diread;
	std::string path = "../GROUPS";	
	int i;

	std::vector<std::string> list;

	dir = opendir(path.c_str());

	while((diread = readdir(dir)) != nullptr){
		std::string gid;
		std::string gname;
		std::string mid;
		std::stringstream message;

		if(diread->d_name[0]=='.')
			continue;
		if(strlen(diread->d_name)>2)
			continue;

		gid = diread->d_name;
		/*gname = get_group_name(diread->d_name);
		mid = get_mid(diread->d_name);*/

		message << " [ " << gid << " " << "]";
		list.push_back(message.str());
	}
	if(numberOfGroups(path) == 0)
		std::cout << "RGL 0: No existing groups\n";
	else{
		std::cout << "RGL " << numberOfGroups(path) << std::endl;
		for(i = 0; i < list.size(); i++)
			std::cout << list[i] << std::endl;
	}
	closedir(dir);	
}

/*GROUP SUBSCRIBE*/
void gsr(std::string UID, std::string GID, std::string gname){
	std::string path = "../GROUPS";

	if(validUID(UID) && !UID_free(UID)){
		if (validGID(GID)){
			if(GID != "00"){
				if(verifyGroupName(GID, gname)){
					/*CREATE USER FILE*/
				}
				else
					std::cout << "E_GNAME: Invalid Group Name\n";
			}
			else{

			}

		}
		else
			std::cout << "E_GRP: Invalid GID\n";
	}
	else
		std::cout << "E_USR: Invalid UID\n";

}


int main(void){
	gsr("95662", "99", "wtf");

	exit(0);
}