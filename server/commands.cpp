#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <csignal>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
extern "C" {
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <dirent.h>
}

using namespace std;

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
		if(diread->d_name == GID){
			closedir(dir);
			return true;
		}
	}
	closedir(dir);
	return false;	
}

bool validGID(std::string GID){
	if(GID.length() == 2 && isNumber(GID))
		if(GID == "00" || groupExists(GID))
			return true;
	return false;
}

bool existingGroupName(std::string GID, std::string gname){
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

bool validGroupName(std::string gname){
	int i;

	for (i=0; gname[i] != '\0'; i++)
		if ((!isalnum(gname[i]) && gname[i] != '-' && gname[i] != '_') || i >= 24)
				return false;
	if(i == 0)
		return false;	
	return true;
}

void unsubscribe_groups(std::string UID){
	std::string path = "../GROUPS";
	DIR *dir;
	struct dirent *diread;

	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){
		if(diread->d_name[0]=='.')
			continue;
		if(strlen(diread->d_name)>2)
			continue;

		std::string new_path = path;
		new_path.append("/"); new_path.append(diread->d_name);
		new_path.append("/"); new_path.append(UID); new_path.append(".txt");

		remove(new_path.c_str());
	}	
	closedir(dir);	
}

std::string get_group_name(std::string GID){
	std::string path = "../GROUPS/";

	path.append(GID);path.append("/");
	path.append(GID);path.append("_name.txt");

	std::ifstream gnameFile;
	std::string gname;

	gnameFile.open(path.c_str());
	getline(gnameFile, gname);

	return gname;
}

std::string get_mid(std::string GID){
	std::string path = "../GROUPS/";
	DIR *dir;
	struct dirent *diread;
	std::string MID = "0000";

	path.append(GID);path.append("/");
	path.append("MSG");

	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){
		if(diread->d_name[0]=='.')
			continue;
		if(strlen(diread->d_name)>4)
			continue;

		if(atoi(MID.c_str()) < atoi(diread->d_name))
			MID = diread->d_name;
	}
	closedir(dir);
	return MID;	
}

bool user_logon(std::string UID){
	std::string path = "../USERS/";
	std::string loginFile = "_login.txt";	
	DIR *dir;
	struct dirent *diread;

	loginFile.insert(0, UID);

	path.append(UID);

	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){
		if(diread->d_name[0]=='.')
			continue;

		if(loginFile == diread->d_name)
			closedir(dir);
			return true;
	}
	closedir(dir);
	return false;	
}

bool UID_in_group(std::string UID, std::string GID){
	std::string path = "../GROUPS/";
	std::string UIDsubscribed = ".txt";
	DIR *dir;
	struct dirent *diread;

	path.append(GID);
	UIDsubscribed.insert(0, UID);

	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){
		if(diread->d_name[0]=='.')
			continue;

		if(UIDsubscribed == diread->d_name){
			closedir(dir);
			return true;
		}
	}
	closedir(dir);
	return false;
}

bool validTextSize(string Tsize){
	return stoi(Tsize) <= 240;
}

int max_mid(string GID){
	string path = "../GROUPS/";
	DIR *dir;
	struct dirent *diread;
	int i=0;

	path.append(GID); path.append("/MSG");

	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){
		if(diread->d_name[0]=='.')
			continue;

		if(strlen(diread->d_name) != 4)
			continue;

		i++;
	}

	return i;
}

string post_text(string UID, string GID, string text){
	string path = "../GROUPS/";
	DIR *dir;
	struct dirent *diread;
	int i;
	string author, msg;

	path.append(GID); path.append("/MSG");

	i = max_mid(GID);

	string new_mid;
	i++;
	new_mid = to_string(i);

	while(strlen(new_mid.c_str()) != 4)
		new_mid.insert(0, "0");

	path.append("/"); path.append(new_mid);

	mkdir(path.c_str(), 0777);

	author = path;
	msg = path;
	author.append("/A U T H O R.txt");
	msg.append("/T E X T.txt");

	ofstream authorFile(author);
	ofstream msgFile(msg);

	authorFile << UID;

	msgFile << text;

	authorFile.close();
	msgFile.close();

	return new_mid;
}

bool validFileInfo(string Fname, string Fsize, string data){
	int i;
	stringstream ss;
	string name;
	locale loc;
	int toBeSure = 0;

	if(0 <= strlen(Fname.c_str()) <= 24){
		for (i=0; Fname[i] != '\0'; i++)
			if (!isalnum(Fname[i]) && Fname[i] != '-' && Fname[i] != '_' && Fname[i] != '.')
				return false;

		ss << Fname;

		while(getline(ss, name, '.'))
			toBeSure++;

		if(toBeSure < 2)
			return false;

		if(strlen(name.c_str()) != 3)
			return false;

		for(i=0; name[i] != '\0'; i++){
			if(!isalpha(name[i]) || i > 2)
				return false;
		}

		if(strlen(Fsize.c_str()) >= 10 || strlen(Fsize.c_str()) == 0)
			return false;

		return true;
	}
	return false;
}

void post_file(string Fname, string data, string GID, string MID){
	string path = "../GROUPS/";

	path.append(GID); path.append("/MSG/");
	path.append(MID); path.append("/");
	path.append(Fname);

	ofstream file(path);

	file << data;

	file.close();
}

string getFileName(string path){
	DIR *dir;
	struct dirent *diread;
	string name = "";
	string author = "A U T H O R.txt";
	string text = "T E X T.txt";


	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){
		if(diread->d_name[0]=='.')
			continue;
		if(strcmp(diread->d_name, author.c_str()) == 0)
			continue;
		if(strcmp(diread->d_name, text.c_str()) == 0)
			continue;
		name = diread->d_name;
	}

	return name;	
}

string getFileSize(string path){

	ifstream fileSize(path, ios::binary);
	fileSize.seekg(0, ios::end);
	return to_string(fileSize.tellg());
}

string getFileData(string path){
	char* data;

	ifstream fileFile(path);
	stringstream ss;

	ss << fileFile.rdbuf();

	return ss.str();
}

/*REGISTER*/
/*Falta enviar os códigos, aka dar returns*/
void reg(std::string UID, std::string pass){
	std::string path="../USERS";
	std::string passFile_name = "_pass.txt";
	
	/*Provavelmente pode tornanr-se num && uma vez que a exceção é a mesma nos dois casos*/
	if (validUID(UID)){

		/*verifica se NÃO existe uma diretoria com o mesmso UID, FALTA DAR RETURN*/
		if(!UID_free(UID)){
			std::cout << "DUP: UID used already" << std::endl;
		}
		else{
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

				std::cout << "OK: Registered successfully!" << std::endl;			
			}
			else 
				std::cout << "NOK: Invalid password" << std::endl;
		}	
	}
	else
		std::cout << "NOK: Invalid UID" << std::endl;
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

				unsubscribe_groups(UID);

				std::cout << "OK: Unregistered successfully!" << std::endl;
			}
			else
				std::cout << "NOK: Wrong password" << std::endl;
		}
		else
			std::cout << "NOK: No registration for that UID" << std::endl;
	}
	else
		std::cout << "NOK: Invalid UID or password" << std::endl;	
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

		std::cout << "OK: Logged in successfully!" << std::endl;
	}
	else
		std::cout << "NOK: Incorrect UID or password" << std::endl;
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

		std::cout << "OK: Logged out successfully!" << std::endl;
	}
	else
		std::cout << "NOK: Incorrect UID or password" << std::endl;
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
		std::string MID;
		std::stringstream message;

		if(diread->d_name[0]=='.')
			continue;
		if(strlen(diread->d_name)>2)
			continue;

		gid = diread->d_name;
		gname = get_group_name(diread->d_name);
		MID = get_mid(diread->d_name);

		message << " [ " << gid << " " << gname << " " << MID << "]";
		list.push_back(message.str());
	}
	if(numberOfGroups(path) == 0)
		std::cout << "RGL 0: No existing groups\n";
	else{
		std::sort(list.begin(), list.end());
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
				if(existingGroupName(GID, gname)){
					path.append("/"); path.append(GID); path.append("/");
					path.append(UID); path.append(".txt");

					std::ofstream UID_File(path);

					UID_File.close();

					std::cout << "OK: User successfully subscribed group!\n";
				}

				else
					std::cout << "E_GNAME: Invalid Group Name\n";
			}

			else{
				if(validGroupName(gname)){
					int i;
					i = numberOfGroups(path);
					if(i < 99){
						i++;

						std::string newGID;
						std::string gname_path;
						std::string uid_path;
						std::string msg_path;

						if(i <= 9)
							newGID.append("0");

						newGID.append(std::to_string(i));

						path.append("/"); path.append(newGID);
						mkdir(path.c_str(), 0777);

						path.append("/");

						gname_path = path;
						gname_path.append(newGID); gname_path.append("_name.txt");
						std::ofstream new_Gname_file(gname_path);
						new_Gname_file << gname << std::endl;
						new_Gname_file.close();

						uid_path = path;
						uid_path.append(UID); uid_path.append(".txt");
						std::ofstream new_uid_file(uid_path);
						new_uid_file.close();

						msg_path = path;
						msg_path.append("MSG");
						mkdir(msg_path.c_str(), 0777);

						std::cout << "NEW GID: New group created!\n";	
					}
					else
						std::cout << "E_FULL: Number of groups has reached it's limit...\n";
				}
				else
					std::cout << "E_GNAME: Invalid group name\n";
			}
		}
		else
			std::cout << "E_GRP: Invalid GID\n";
	}
	else
		std::cout << "E_USR: Invalid UID\n";
}

/*Unsubscribe from group*/
void gur(std::string UID, std::string GID){
	std::string path = "../GROUPS/";
	DIR *dir;
	struct dirent *diread;

	if(validUID(UID) && !UID_free(UID)){
		if(validGID(GID) && groupExists(GID)){
			path.append(GID);path.append("/");
			path.append(UID);path.append(".txt");

			if(remove(path.c_str()) != 0)
				std::cout << "NOK: User doesn't susbscribe to this group!\n";
	
		}
		else
			std::cout << "E_GRP: Invalid GID.\n";
	}
	else
		std::cout << "E_USR: Invalid UID.\n";
}

/*List of Groups subscribed by a specific user*/
void glm(std::string UID){
	DIR *dir;
	struct dirent *diread;
	std::string path = "../GROUPS";	
	int i = 0;
	std::vector<std::string> list;

	if(validUID(UID) && !UID_free(UID) && user_logon(UID)){
			
		dir = opendir(path.c_str());

		while((diread = readdir(dir)) != nullptr){
			std::string gid;
			std::string gname;
			std::string MID;
			std::stringstream message;

			if(diread->d_name[0]=='.')
				continue;
			if(strlen(diread->d_name)>2)
				continue;
			
			if(UID_in_group(UID, diread->d_name)){
				i++;
				gid = diread->d_name;
				gname = get_group_name(diread->d_name);
				MID = get_mid(diread->d_name);

				message << " [ " << gid << " " << gname << " " << MID << "]";
				list.push_back(message.str());
			}
		}
		if(i == 0)
			std::cout << "RGM 0: No groups subscribed\n";
		else{
			std::sort(list.begin(), list.end());
			std::cout << "RGM " << i << std::endl;
			for(i = 0; i < list.size(); i++)
				std::cout << list[i] << std::endl;
		}
		closedir(dir);
	}
	else
		std::cout << "E_USR: Invalid  UID or user isn't logged in.\n";	
}

/*List of users subscribed to a given group (TCP)*/
void uls(std::string GID){
	DIR *dir;
	struct dirent *diread;
	std::string path = "../GROUPS/";	
	std::vector<std::string> list;
	int i;

	if(validGID(GID)){
			
		path.append(GID);

		dir = opendir(path.c_str());
		
		while((diread = readdir(dir)) != nullptr){
			std::string uid;
			std::string gname;
			std::stringstream message;
			if(diread->d_name[0]=='.')
				continue;
			if(strlen(diread->d_name) != 9)
				continue;


			/*Removing '.txt'*/
			std::string is_uid = diread->d_name;
			is_uid.erase(5, 4);

			if(validUID(is_uid) && !UID_free(is_uid)){
				uid = is_uid;

				message << " [" << uid << "]";
				list.push_back(message.str());
			}
			
		}
		std::sort(list.begin(), list.end());
		std::cout << "RGM " << get_group_name(GID) << std::endl;
		for(i = 0; i < list.size(); i++)
			std::cout << list[i] << std::endl;
		
		closedir(dir);
	}
	else
		std::cout << "NOK: Invalid GID or group doesn't exist.\n";	
}

/*Doing a post on a certain group*/
void pst(string UID, string GID, string Tsize, string text, string Fname = "", string Fsize = "", string data = ""){
	DIR *dir;
	struct dirent *diread;
	string path = "../GROUPS/";	
	int i;
	string status;

	if(validUID(UID) && !UID_free(UID) && validGID(GID) && UID_in_group(UID,GID)){
		if(validTextSize(Tsize)){
			if(Fname == "" && Fsize == "" && data == ""){
				if(max_mid(GID) == 9999)
					cout << "NOK: Maximum number of messages reached (9999).\n";
				else
					status = post_text(UID, GID, text);
					cout << "RPT " << status << endl;

			}
			else{
				if(validFileInfo(Fname, Fsize, data)){
					if(max_mid(GID) == 9999)
						cout << "NOK: Maximum number of messages reached (9999).\n";
					else
						status = post_text(UID, GID, text);
						post_file(Fname, data, GID, status);

						cout << "RPT " << status << endl;
				}
				else{
					cout << "NOK: Invalid file info.\n";
				}
			}
		}
		else
			cout << "NOK: Invalid text info.\n";

	}
	else
		cout << "NOK: Invalid arguments.\n";		
}

void rtv(string UID, string GID, string MID){
	string path = "../GROUPS/";
	DIR *dir;
	struct dirent *diread;
	int i = 0;

	vector<string> list;

	path.append(GID); path.append("/MSG");

	dir = opendir(path.c_str());

	if(max_mid(GID) == 0){
		cout << "RRT EOF [0]\n";
		return;
	}

	while((diread = readdir(dir))!= nullptr && i < 20){
		if(diread->d_name[0]=='.')
			continue;
		if(strlen(diread->d_name) != 4)
			continue;
		if(stoi(diread->d_name) < stoi(MID))
			continue;

		string currentMID;
		string currentUID;
		string Tsize;
		string text;
		string Fname = "";
		string Fsize = "";
		string data = "";
		stringstream message;

		string msg_path = path;
		string author_path;
		string text_path;
		string file_path;

		currentMID = diread->d_name;

		msg_path.append("/"); msg_path.append(currentMID); 

		author_path = msg_path;author_path.append("/A U T H O R.txt");
		ifstream authorFile(author_path);
		getline(authorFile, currentUID);

		text_path = msg_path;
		text_path.append("/T E X T.txt");
		ifstream textFile(text_path);
		getline(textFile, text);

		Tsize = to_string(strlen(text.c_str()));

		if((Fname = getFileName(msg_path)) != ""){
			file_path = msg_path; file_path.append("/"); file_path.append(Fname);
			Fsize = getFileSize(file_path);
			data = getFileData(file_path);
		}

		message << "[" << currentMID << ", " << currentUID << ", " <<
		Tsize << ", " << text << "[ / " << Fname << " " << Fsize << " "
		<< data << "]]"; 

		list.push_back(message.str());

		i++;
	}
	sort(list.begin(), list.end());
	cout << "RRT [" << i << std::endl;
	for(i = 0; i < list.size(); i++)
		cout << list[i] << endl;

	cout << "]\n";

}

int main(void){
	pst("95662", "02", "16", "Bom dia!", "ola.png", "123", "alala");
	rtv("95662", "02", "0001");
	

	exit(0);
}


/*TODO:
	- Does the user have to be log on to subscribe/unsubscribe?
	- special UIDs and GROUPs;
	- How to deal with non txt files
	- Sizes um bocado estranhos
	- '\n'?
	- f) EOF? No messages or no new messages?*/