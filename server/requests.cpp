#include "../Constants.hpp"

using namespace std;

bool isNumber(string str){
	for(int i = 0; i < str.length(); i++)
      	if(! (str[i] >= '0' && str[i] <= '9') ) 
      		return false;

	return true;
}

bool isAlNum(string str){
	int i;

	for (i=0; i<8; i++)
		if (!isalnum(str[i]))
			return false;
	return true;
}

bool validPass(string pass){
	if (pass.length()==8 && isAlNum(pass))
		return true;
	return false;
}

bool validUID(string UID){
	if (isNumber(UID) && UID.length()==5)
		return true;
	return false;
}

bool UID_free(string UID){
	string path = "../USERS";
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

void delete_files(string path){
	DIR *dir;
	struct dirent *diread;	
	dir = opendir(path.c_str());

	while((diread = readdir(dir)) != nullptr){
		if(diread->d_name[0]=='.')
			continue;
		string current_path = path;
		current_path.append("/"); current_path.append(diread->d_name);

		remove(current_path.c_str());
	}
}

bool correct_pass(string UID, string pass){
	string path="../USERS/";
	path.append(UID);

	string passFile_name = "_pass.txt";
	passFile_name.insert(0, UID);

	path.append("/"); path.append(passFile_name);

	ifstream passwordFile;
	string password;

	passwordFile.open(path.c_str());
	getline(passwordFile, password);

	if(password == pass)
		return true;
	return false;

}

int numberOfGroups(string path){
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

bool groupExists(string GID){
	string path = "../GROUPS";
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

bool validGID(string GID){
	if(GID.length() == 2 && isNumber(GID))
		if(GID == "00" || groupExists(GID))
			return true;
	return false;
}

bool existingGroupName(string GID, string gname){
	string path = "../GROUPS/";
	string gname_file = "_name.txt";

	path.append(GID); path.append("/");

	gname_file.insert(0,GID);

	path.append(gname_file);

	ifstream gNameFile;
	string name;

	gNameFile.open(path.c_str());
	getline(gNameFile, name);	

	if(gname == name)
		return true;
	return false;

}

bool validGroupName(string gname){
	int i;

	for (i=0; gname[i] != '\0'; i++)
		if ((!isalnum(gname[i]) && gname[i] != '-' && gname[i] != '_') || i >= 24)
				return false;
	if(i == 0)
		return false;	
	return true;
}

void unsubscribe_groups(string UID){
	string path = "../GROUPS";
	DIR *dir;
	struct dirent *diread;

	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){
		if(diread->d_name[0]=='.')
			continue;
		if(strlen(diread->d_name)>2)
			continue;

		string new_path = path;
		new_path.append("/"); new_path.append(diread->d_name);
		new_path.append("/"); new_path.append(UID); new_path.append(".txt");

		remove(new_path.c_str());
	}	
	closedir(dir);	
}

string get_group_name(string GID){
	string path = "../GROUPS/";

	path.append(GID);path.append("/");
	path.append(GID);path.append("_name.txt");

	ifstream gnameFile;
	string gname;

	gnameFile.open(path.c_str());
	getline(gnameFile, gname);

	return gname;
}

string get_mid(string GID){
	string path = "../GROUPS/";
	DIR *dir;
	struct dirent *diread;
	string mid = "0000";

	path.append(GID);path.append("/");
	path.append("MSG");

	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){
		if(diread->d_name[0]=='.')
			continue;
		if(strlen(diread->d_name)>4)
			continue;

		if(atoi(mid.c_str()) < atoi(diread->d_name))
			mid = diread->d_name;
	}
	closedir(dir);
	return mid;	
}

bool user_logon(string UID){
	string path = "../USERS/";
	string loginFile = "_login.txt";	
	DIR *dir;
	struct dirent *diread;

	loginFile.insert(0, UID);

	path.append(UID);

	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){
		if(diread->d_name[0]=='.')
			continue;

		if(loginFile == diread->d_name){
			closedir(dir);
			return true;
		}
	}
	closedir(dir);
	return false;	
}

bool UID_in_group(string UID, string GID){
	string path = "../GROUPS/";
	string UIDsubscribed = ".txt";
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

/*REGISTER*/
/*Falta enviar os códigos, aka dar returns*/
string reg(string command){
	string reply = "reg\n";
	string cmd, UID, pass;
	string path="../USERS";
	string passFile_name = "_pass.txt";
	if(sscanf(command.c_str(), "%s %s %s\n", cmd, UID, pass) != 3){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}
	if(cmd.compare("REG") != 0){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}
	
	/*Provavelmente pode tornanr-se num && uma vez que a exceção é a mesma nos dois casos*/
	if (validUID(UID)){

		/*verifica se NÃO existe uma diretoria com o mesmso UID, FALTA DAR RETURN*/
		if(!UID_free(UID)){
			std::cout << "DUP: UID used already" << endl;
		}
		else{
			/*Cria o ficheiro com a passe*/ 
			if(validPass(pass)){

				path.append("/"); path.append(UID);

				/*Cria nova diretoria*/
				mkdir(path.c_str(), 0777);
				path.append("/");

				passFile_name.insert(0, UID);
				ofstream passFile(path.append(passFile_name));
				passFile << pass << endl;

				passFile.close();

				std::cout << "OK: Registered successfully!" << endl;			
			}
			else 
				std::cout << "NOK: Invalid password" << endl;
		}	
	}
	else
		std::cout << "NOK: Invalid UID" << endl;
	return reply;
}

/*UNREGISTER*/
string unr(string command){
	string reply = "unr\n";
	string cmd, UID, pass;
	string path="../USERS";
	if(sscanf(command.c_str(), "%s %s %s\n", cmd, UID, pass) != 3){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}
	if(cmd.compare("UNR") != 0){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}

	if(validUID(UID) && validPass(pass)){
		if(!UID_free(UID)){
			path.append("/"); path.append(UID); 
			if(correct_pass(UID, pass)){

				delete_files(path);
				rmdir(path.c_str());

				unsubscribe_groups(UID);

				std::cout << "OK: Unregistered successfully!" << endl;
			}
			else
				std::cout << "NOK: Wrong password" << endl;
		}
		else
			std::cout << "NOK: No registration for that UID" << endl;
	}
	else
		std::cout << "NOK: Invalid UID or password" << endl;	
	return reply;
}

/*LOGIN*/
string log(string command){
	string reply = "log\n";
	string cmd, UID, pass;
	string path="../USERS";
	string loginFile_name = "_login.txt";
	if(sscanf(command.c_str(), "%s %s %s\n", cmd, UID, pass) != 3){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}
	if(cmd.compare("LOG") != 0){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}

	if(validUID(UID) &&  validPass(pass) 
		 && !UID_free(UID) && correct_pass(UID,pass)){

		path.append("/"); path.append(UID); path.append("/");
		loginFile_name.insert(0, UID);
		ofstream loginFile(path.append(loginFile_name));

		loginFile.close();

		std::cout << "OK: Logged in successfully!" << endl;
	}
	else
		std::cout << "NOK: Incorrect UID or password" << endl;
	return reply;
}

/*LOGOUT*/
string out(string command){
	string reply = "out\n";
	string cmd, UID, pass;
	string path="../USERS";
	string loginFile_name = "_login.txt";
	if(sscanf(command.c_str(), "%s %s %s\n", cmd, UID, pass) != 3){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}
	if(cmd.compare("OUT") != 0){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}

	if(validUID(UID) &&  validPass(pass) 
		 && !UID_free(UID) && correct_pass(UID,pass)){

		path.append("/"); path.append(UID); path.append("/");
		loginFile_name.insert(0, UID);
		path.append(loginFile_name);
		remove(path.c_str());

		std::cout << "OK: Logged out successfully!" << endl;
	}
	else
		std::cout << "NOK: Incorrect UID or password" << endl;
	return reply;
}

/*GROUP LIST*/
string gls(string command){
	string reply = "gls\n";
	DIR *dir;
	struct dirent *diread;
	string path = "../GROUPS";	
	int i;
	if(command.compare("GLS") != 0){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}

	vector<string> list;

	dir = opendir(path.c_str());

	while((diread = readdir(dir)) != nullptr){
		string gid;
		string gname;
		string mid;
		stringstream message;

		if(diread->d_name[0]=='.')
			continue;
		if(strlen(diread->d_name)>2)
			continue;

		gid = diread->d_name;
		gname = get_group_name(diread->d_name);
		mid = get_mid(diread->d_name);

		message << " [ " << gid << " " << gname << " " << mid << "]";
		list.push_back(message.str());
	}
	if(numberOfGroups(path) == 0)
		std::cout << "RGL 0: No existing groups\n";
	else{
		sort(list.begin(), list.end());
		std::cout << "RGL " << numberOfGroups(path) << endl;
		for(i = 0; i < list.size(); i++)
			std::cout << list[i] << endl;
	}
	closedir(dir);
	return reply;
}

/*GROUP SUBSCRIBE*/
string gsr(string command){
	string reply = "gsr\n";
	string cmd, UID, GID, gname;
	string path = "../GROUPS";
	if(sscanf(command.c_str(), "%s %s %s %s\n", cmd, UID, GID, gname) != 4){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}
	if(cmd.compare("GSR") != 0){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}

	if(validUID(UID) && !UID_free(UID)){
		if (validGID(GID)){
			if(GID != "00"){
				if(existingGroupName(GID, gname)){
					path.append("/"); path.append(GID); path.append("/");
					path.append(UID); path.append(".txt");

					ofstream UID_File(path);

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

						string newGID;
						string gname_path;
						string uid_path;
						string msg_path;

						if(i <= 9)
							newGID.append("0");

						newGID.append(to_string(i));

						path.append("/"); path.append(newGID);
						mkdir(path.c_str(), 0777);

						path.append("/");

						gname_path = path;
						gname_path.append(newGID); gname_path.append("_name.txt");
						ofstream new_Gname_file(gname_path);
						new_Gname_file << gname << endl;
						new_Gname_file.close();

						uid_path = path;
						uid_path.append(UID); uid_path.append(".txt");
						ofstream new_uid_file(uid_path);
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
	return reply;
}

/*Unsubscribe from group*/
string gur(string command){
	string reply = "gur\n";
	string cmd, UID, GID;
	string path = "../GROUPS/";
	DIR *dir;
	struct dirent *diread;
	if(sscanf(command.c_str(), "%s %s %s\n", cmd, UID, GID) != 3){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}
	if(cmd.compare("GUR") != 0){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}
	

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
	return reply;
}

/*List of Groups subscribed by a specific user*/
string glm(string command){
	string reply = "glm\n";
	string cmd, UID;
	DIR *dir;
	struct dirent *diread;
	string path = "../GROUPS";	
	int i = 0;
	vector<string> list;
	if(sscanf(command.c_str(), "%s %s\n", cmd, UID) != 2){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}
	if(cmd.compare("GLM") != 0){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}

	if(validUID(UID) && !UID_free(UID) && user_logon(UID)){
			
		dir = opendir(path.c_str());

		while((diread = readdir(dir)) != nullptr){
			string gid;
			string gname;
			string mid;
			stringstream message;

			if(diread->d_name[0]=='.')
				continue;
			if(strlen(diread->d_name)>2)
				continue;
			
			if(UID_in_group(UID, diread->d_name)){
				i++;
				gid = diread->d_name;
				gname = get_group_name(diread->d_name);
				mid = get_mid(diread->d_name);

				message << " [ " << gid << " " << gname << " " << mid << "]";
				list.push_back(message.str());
			}
		}
		if(i == 0)
			std::cout << "RGM 0: No groups subscribed\n";
		else{
			sort(list.begin(), list.end());
			std::cout << "RGM " << i << endl;
			for(i = 0; i < list.size(); i++)
				std::cout << list[i] << endl;
		}
		closedir(dir);
	}
	else
		std::cout << "E_USR: Invalid  UID or user isn't logged in.\n";
	return reply;
}

/*List of users subscribed to a given group (TCP)*/
string uls(string command){
	string reply = "uls\n";
	string cmd, GID;
	DIR *dir;
	struct dirent *diread;
	string path = "../GROUPS/";	
	vector<string> list;
	int i;
	if(sscanf(command.c_str(), "%s %s\n", cmd, GID) != 2){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}
	if(cmd.compare("ULS") != 0){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}

	if(validGID(GID)){
			
		path.append(GID);

		dir = opendir(path.c_str());
		
		while((diread = readdir(dir)) != nullptr){
			string uid;
			string gname;
			stringstream message;
			if(diread->d_name[0]=='.')
				continue;
			if(strlen(diread->d_name) != 9)
				continue;


			/*Removing '.txt'*/
			string is_uid = diread->d_name;
			is_uid.erase(5, 4);

			if(validUID(is_uid) && !UID_free(is_uid)){
				uid = is_uid;

				message << " [" << uid << "]";
				list.push_back(message.str());
			}
			
		}
		sort(list.begin(), list.end());
		std::cout << "RGM " << get_group_name(GID) << endl;
		for(i = 0; i < list.size(); i++)
			std::cout << list[i] << endl;
		
		closedir(dir);
	}
	else
		std::cout << "NOK: Invalid GID or group doesn't exist.\n";
	return reply;
}

string pst(string command){
	string reply = "pst\n";
	string cmd, UID, GID, Tsize, text, Fname, Fsize, data;
	int nArgs = sscanf(command.c_str(), "%s %s %s %s %s %s %s %s\n", cmd, UID, GID, Tsize, text, Fname, Fsize, data);
	if(nArgs != 5 && nArgs != 8){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}

	if(cmd.compare("PST") != 0){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}

	return reply;
}

string rtv(string command){
	string reply = "rtv\n";
	string cmd, UID, GID, MID;
	if(sscanf(command.c_str(), "%s %s\n", cmd, GID) != 4){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}
	if(cmd.compare("RTV") != 0){
		fprintf(stderr, "ERR");
		return "ERR\n";
	}

	return reply;
}

/*TODO:
	- Does the user have to be log on to subscribe/unsubscribe?
	- special UIDs and GROUPs
	- How to deal with files when they are sent*/
