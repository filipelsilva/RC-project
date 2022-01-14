#include "../Common.hpp"

// TODO: save selected uid, gid and GName; remove possible \n bugs with incomplete commands in requests.cpp
int verbose = 0;

void printVerbose(string UID, string GID) {
	if (!UID.empty()) {
		cout << "[UID=" << UID << "]";
	}
	if (!GID.empty()) {
		cout << "[GID=" << GID << "]";
	}
	cout << endl;
}

/*Verifies if a User password is valid (8 alphanumerical characters).*/
bool validPass(string pass){
	if (pass.length()==PASS_LENGTH && isAlNum(pass))
		return true;
	return false;
}

/*Verifies if a User ID is valid. (5-digit number)*/
bool validUID(string UID){
	if (UID.length()==UID_LENGTH && isNumber(UID))
		return true;
	return false;
}

/*Verifies if there isn't an equal User ID already registered.*/
bool UID_free(string UID){
	string path = "USERS";
	DIR *dir;
	struct dirent *diread;

	if((dir = opendir(path.c_str())) == NULL){
		fprintf(stderr, "Error: Couldn't open USERS directory\n");
	}

	while((diread = readdir(dir)) != nullptr){
		if(diread->d_name[0]=='.')
			continue;
		if(diread->d_name == UID){
			closedir(dir);
			return false;
		}
	}
	closedir(dir);
	return true;
}

/*Verifies if the given password is the one registered for the given UID.*/
bool correct_pass(string UID, string pass){
	string path="USERS/";
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

/*Returns the number of groups available.*/
int numberOfGroups(string path){
	int i = 0;
	DIR *dir;
	struct dirent *diread;

	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){

		if(diread->d_name[0]=='.')
			continue;
		if(strlen(diread->d_name)>GID_LENGTH)
			continue;

		i++;
	}	

	closedir(dir);

	return i;
}

/*Verifies if the given group already exists.*/
bool groupExists(string GID){
	string path = "GROUPS";
	DIR *dir;
	struct dirent *diread;

	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){
		if(diread->d_name[0]=='.')
			continue;
		if(strlen(diread->d_name)>GID_LENGTH)
			continue;
		if(diread->d_name == GID){
			closedir(dir);
			return true;
		}
	}
	closedir(dir);
	return false;	
}

/*Verifies if the given Group ID is valid (2-digit number) 
and if the group exists.*/
bool validGID(string GID){
	if(GID.length() == GID_LENGTH && isNumber(GID))
		if(GID == "00" || groupExists(GID))
			return true;
	return false;
}

/*Verifies if the given Message ID is valid (4-digit number) 
and if the group exists.*/
bool validMID(string MID){
	if(MID.length() == MID_LENGTH && isNumber(MID) && MID != "0000")
		return true;
	return false;
}

/*Verifies if the name of the group with given GID
is the same as the given Gname.*/
bool existingGroupName(string GID, string GName){
	string path = "GROUPS/";
	string GName_file = "_name.txt";

	path.append(GID); path.append("/");

	GName_file.insert(0,GID);

	path.append(GName_file);

	ifstream gNameFile;
	string name;

	gNameFile.open(path.c_str());
	getline(gNameFile, name);	

	if(GName == name)
		return true;
	return false;

}

/*Verifies if given group name is valid (total of 
24 alphanumerical characters plus '-' and '_').*/
bool validGroupName(string GName){
	int i;

	for (i=0; GName[i] != '\0'; i++)
		if ((!isalnum(GName[i]) && GName[i] != '-' && GName[i] != '_') || i >= GNAME_LENGTH)
				return false;
	if(i == 0)
		return false;	
	return true;
}

/*Unsubscribe all groups that the given UID subscribes.*/
void unsubscribe_groups(string UID){
	string path = "GROUPS";
	DIR *dir;
	struct dirent *diread;

	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){
		if(diread->d_name[0]=='.')
			continue;
		if(strlen(diread->d_name)>GID_LENGTH)
			continue;

		string new_path = path;
		new_path.append("/"); new_path.append(diread->d_name);
		new_path.append("/"); new_path.append(UID); new_path.append(".txt");

		remove(new_path.c_str());
	}	
	closedir(dir);	
}

/*Returns the group name of the group with the given GID.*/
string get_group_name(string GID){
	string path = "GROUPS/";

	path.append(GID);path.append("/");
	path.append(GID);path.append("_name.txt");

	ifstream GNameFile;
	string GName;

	GNameFile.open(path.c_str());
	getline(GNameFile, GName);

	return GName;
}

/*Returns the latest message MID(message ID) of the group with the given GID.*/
string get_MID(string GID){
	string path = "GROUPS/";
	DIR *dir;
	struct dirent *diread;
	string MID = "0000";

	path.append(GID);path.append("/");
	path.append("MSG");

	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){
		if(diread->d_name[0]=='.')
			continue;
		if(strlen(diread->d_name)>MID_LENGTH)
			continue;

		if(atoi(MID.c_str()) < atoi(diread->d_name))
			MID = diread->d_name;
	}
	closedir(dir);
	return MID;	
}

/*Verifies if the user with the given UID is logged in.*/
bool user_logon(string UID){
	string path = "USERS/";
	string loginFile = "_login.txt";	
	DIR *dir;
	struct dirent *diread;

	loginFile.insert(0, UID);

	path.append(UID);

	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){
		if(diread->d_name[0]=='.'){
			continue;
		}

		if(loginFile == diread->d_name){
			closedir(dir);
			return true;
		}
	}
	
	closedir(dir);
	return false;	
}

/*Verifies if the user with the given UID is subscribed to the group with
the given GID.*/
bool UID_in_group(string UID, string GID){
	string path = "GROUPS/";
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

/*Returns the number of messages in the group with the given GID.*/
int max_MID(string GID){
	string path = "GROUPS/";
	DIR *dir;
	struct dirent *diread;
	int i=0;

	path.append(GID); path.append("/MSG");

	dir = opendir(path.c_str());

	while((diread = readdir(dir))!= nullptr){
		if(diread->d_name[0]=='.')
			continue;

		if(strlen(diread->d_name) != MID_LENGTH)
			continue;

		i++;
	}
	closedir(dir);
	return i;
}

/*Posts the given text message on the group with the given GID, having the 
user with the given UID as an author. */
string post_text(string UID, string GID, string text){
	string path = "GROUPS/";
	int i;
	string author, msg;

	path.append(GID); path.append("/MSG");

	i = max_MID(GID);

	string new_MID;
	i++;
	new_MID = to_string(i);

	while(strlen(new_MID.c_str()) != MID_LENGTH)
		new_MID.insert(0, "0");

	path.append("/"); path.append(new_MID);

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

	return new_MID;
}

/*Verifies if the given file name (maximum of 24 alphanumerical characters
(plus '-', '_' and '.') including the 3 letter extension) and the given
file size (which field has at most 10 digits) are valid.*/
bool validFileInfo(string Fname, string Fsize){
	int i;
	stringstream ss;
	string name;
	locale loc;
	int toBeSure = 0;

	if(strlen(Fname.c_str()) <= FNAME_LENGTH){
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
			if(!isalnum(name[i]) || i > GID_LENGTH)
				return false;
		}

		if(strlen(Fsize.c_str()) >= FSIZE_LENGTH || strlen(Fsize.c_str()) == 0)
			return false;

		return true;
	}
	return false;
}

/*Posts a file with the given file name (Fname) and the given data
on the group  with the given GID, on the message with the given MID.*/
void post_file(string Fname, string GID, string MID, string data, int bytes_read, int Fsize, TCPServer &tcp){
	string path = "GROUPS/";
	string received;
	int written = bytes_read;

	path.append(GID); path.append("/MSG/");
	path.append(MID); path.append("/");
	path.append(Fname);

	ofstream file(path, std::ios_base::binary);
	file.write(data.c_str(), bytes_read);
	while(written < Fsize){
		received.assign(tcp.getData(COMMAND_SIZE), COMMAND_SIZE);
	 	for(int i = 0; i < COMMAND_SIZE; i++){
	 		if(!strcmp(&received[i], "\n"))
	 			cout << "\n\nREQUESTS:" << written <<  "\n\n";
		   	file.write(&received[i], 1);
		   	written += 1;
		   	if(written == Fsize){
		    	break;
		   	}
	  	}
	}
	//file.write("\n", 1);
	file.close();
}

/*Returns the name of the file in a given message.*/
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
	closedir(dir);
	return name;	
}

/*Registers a new user with a given UID and a given password.*/
string reg(string command){
	stringstream ss;
	string reply = "RRG NOK\n";
	string cmd, UID, pass;
	string path="USERS";
	string passFile_name = "_pass.txt";
	ss << command;
	getline(ss, cmd, ' ');
	getline(ss, UID, ' ');
	getline(ss, pass);
	if(cmd.compare("REG") != 0){
		fprintf(stderr, "ERR\n");
		return "ERR\n";
	}
	if(UID.empty() || pass.empty()){
		fprintf(stderr, "NOK: Missing argument(s)\n");
		return reply;
	}
	
	/*Provavelmente pode tornanr-se num && uma vez que a exceção é a mesma nos dois casos*/
	if (validUID(UID)){

		/*verifica se NÃO existe uma diretoria com o mesmso UID, FALTA DAR RETURN*/
		if(!UID_free(UID)){
			cout << "DUP: UID used already" << endl;
			reply = "RRG DUP\n";
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

				cout << "OK: Registered successfully!" << endl;
				reply = "RRG OK\n";
			}
			else{
				cout << "NOK: Invalid password" << endl;
				reply = "RRG NOK\n";
			}
		}	
	}
	else{
		cout << "NOK: Invalid UID" << endl;
		reply = "RRG NOK\n";
	}
	return reply;
}

/*Unregisters the user with the given UID and the given password.*/
string unr(string command){
	stringstream ss;
	string reply = "RUN NOK\n";
	string cmd, UID, pass;
	string path="USERS";
	ss << command;
	getline(ss, cmd, ' ');
	getline(ss, UID, ' ');
	getline(ss, pass);
	if(cmd.compare("UNR") != 0){
		fprintf(stderr, "ERR\n");
		return "ERR\n";
	}
	if(UID.empty() || pass.empty()){
		fprintf(stderr, "NOK: Missing argument(s)\n");
		return reply;
	}

	if(validUID(UID) && validPass(pass)){
		if(!UID_free(UID)){
			path.append("/"); path.append(UID); 
			if(correct_pass(UID, pass)){

				delete_files(path);
				rmdir(path.c_str());

				unsubscribe_groups(UID);

				cout << "OK: Unregistered successfully!" << endl;
				reply = "RUN OK\n";
			}
			else{
				cout << "NOK: Wrong password" << endl;
				reply = "RUN NOK\n";
			}
		}
		else{
			cout << "NOK: No registration for that UID" << endl;
			reply = "RUN NOK\n";
		}
	}
	else{
		cout << "NOK: Invalid UID or password" << endl;
		reply = "RUN NOK\n";	
	}
	return reply;
}

/*Logs in the user with the given UID and given password.*/
string log(string command){
	stringstream ss;
	string reply = "RLO NOK\n";
	string cmd, UID, pass;
	string path="USERS";
	string loginFile_name = "_login.txt";
	ss << command;
	getline(ss, cmd, ' ');
	getline(ss, UID, ' ');
	getline(ss, pass);
	if(cmd.compare("LOG") != 0){
		fprintf(stderr, "ERR\n");
		return "ERR\n";
	}
	if(UID.empty() || pass.empty()){
		fprintf(stderr, "NOK: Missing argument(s)\n");
		return reply;
	}

	if(validUID(UID) &&  validPass(pass) 
		 && !UID_free(UID) && correct_pass(UID,pass)){

		path.append("/"); path.append(UID); path.append("/");
		loginFile_name.insert(0, UID);
		ofstream loginFile(path.append(loginFile_name));

		loginFile.close();

		cout << "OK: Logged in successfully!" << endl;
		reply = "RLO OK\n";
	}
	else{
		cout << "NOK: Incorrect UID or password" << endl;
		reply = "RLO NOK\n";
	}
	return reply;
}

/*Logs out the user with the given UID and given password.*/
string out(string command){
	stringstream ss;
	string reply = "ROU NOK\n";
	string cmd, UID, pass;
	string path="USERS";
	string loginFile_name = "_login.txt";
	ss << command;
	getline(ss, cmd, ' ');
	getline(ss, UID, ' ');
	getline(ss, pass);
	if(cmd.compare("OUT") != 0){
		fprintf(stderr, "ERR\n");
		return "ERR\n";
	}
	if(UID.empty() || pass.empty()){
		fprintf(stderr, "NOK: Missing argument(s)\n");
		return reply;
	}

	if(validUID(UID) &&  validPass(pass) 
		 && !UID_free(UID) && correct_pass(UID,pass)){

		path.append("/"); path.append(UID); path.append("/");
		loginFile_name.insert(0, UID);
		path.append(loginFile_name);
		remove(path.c_str());

		cout << "OK: Logged out successfully!" << endl;
		reply = "ROU OK\n";
	}
	else{
		cout << "NOK: Incorrect UID or password" << endl;
		reply = "ROU NOK\n";
	}
	if (verbose) {
		printVerbose(UID, "");
	}
	return reply;
}

/*Lists all the existing groups.*/
string gls(string command){
	string reply = "ERR\n";
	DIR *dir;
	struct dirent *diread;
	string path = "GROUPS";	
	size_t i;
	if(command.compare("GLS") != 0){
		fprintf(stderr, "ERR\n");
		return "ERR\n";
	}

	vector<string> list;

	dir = opendir(path.c_str());

	while((diread = readdir(dir)) != nullptr){
		string gid;
		string GName;
		string MID;
		stringstream message;

		if(diread->d_name[0]=='.')
			continue;
		if(strlen(diread->d_name)>GID_LENGTH)
			continue;

		gid = diread->d_name;
		GName = get_group_name(diread->d_name);
		MID = get_MID(diread->d_name);

		message << gid << " " << GName << " " << MID;
		list.push_back(message.str());
	}
	if(numberOfGroups(path) == 0){
		cout << "RGL 0: No existing groups\n";
		reply = "RGL 0\n";
	}
	else{
		sort(list.begin(), list.end());
		cout << "RGL " << numberOfGroups(path) << " ";
		reply = "RGL " + to_string(numberOfGroups(path));
		for(i = 0; i < list.size(); i++){
			cout << list[i] << endl;
			reply += " " + list[i];
		}
		reply += "\n";
	}
	closedir(dir);
	return reply;
}

/*The user with the given UID subscribes the group with the given GID and
the given name (Gname).*/
string gsr(string command){
	stringstream ss;
	string reply = "RGS NOK\n";
	string cmd, UID, GID, GName;
	string path = "GROUPS";
	ss << command;
	getline(ss, cmd, ' ');
	getline(ss, UID, ' ');
	getline(ss, GID, ' ');
	getline(ss, GName);
	if(cmd.compare("GSR") != 0){
		fprintf(stderr, "ERR\n");
		return "ERR\n";
	}
	if(UID.empty() || GID.empty() || GName.empty()){
		fprintf(stderr, "NOK: Missing argument(s)\n");
		return reply;
	}
	if(!user_logon(UID)){
		fprintf(stderr, "NOK: User not logged in\n");
		return reply;
	}

	if(validUID(UID) && !UID_free(UID)){
		if (validGID(GID)){
			if(GID != "00"){
				if(existingGroupName(GID, GName)){
					path.append("/"); path.append(GID); path.append("/");
					path.append(UID); path.append(".txt");

					ofstream UID_File(path);

					UID_File.close();

					cout << "OK: User successfully subscribed group!\n";
					reply = "RGS OK\n";
				}

				else{
					cout << "E_GNAME: Invalid Group Name\n";
					reply = "RGS E_GNAME\n";
				}
			}

			else{
				if(validGroupName(GName)){
					int i;
					i = numberOfGroups(path);
					if(i < 99){
						i++;

						string newGID;
						string GName_path;
						string uid_path;
						string msg_path;

						if(i <= 9)
							newGID.append("0");

						newGID.append(to_string(i));

						path.append("/"); path.append(newGID);
						mkdir(path.c_str(), 0777);

						path.append("/");

						GName_path = path;
						GName_path.append(newGID); GName_path.append("_name.txt");
						ofstream new_Gname_file(GName_path);
						new_Gname_file << GName << endl;
						new_Gname_file.close();

						uid_path = path;
						uid_path.append(UID); uid_path.append(".txt");
						ofstream new_uid_file(uid_path);
						new_uid_file.close();

						msg_path = path;
						msg_path.append("MSG");
						mkdir(msg_path.c_str(), 0777);

						cout << "NEW GID: New group created!\n";
						reply = "RGS NEW " + newGID + "\n";	
					}
					else{
						cout << "E_FULL: Number of groups has reached it's limit...\n";
						reply = "RGS E_FULL\n";
					}
				}
				else{
					cout << "E_GNAME: Invalid group name\n";
					reply = "RGS E_GNAME\n";
				}
			}
		}
		else{
			cout << "E_GRP: Invalid GID\n";
			reply = "RGS E_GRP\n";
		}
	}
	else{
		cout << "E_USR: Invalid UID\n";
		reply = "RGS E_USR\n";
	}
	if (verbose) {
		printVerbose(UID, GID);
	}
	return reply;
}

/*The user with the given UID unsubscribes the group with the given GID.*/
string gur(string command){
	stringstream ss;
	string reply = "RGU NOK\n";
	string cmd, UID, GID;
	string path = "GROUPS/";
	ss << command;
	getline(ss, cmd, ' ');
	getline(ss, UID, ' ');
	getline(ss, GID);
	if(cmd.compare("GUR") != 0){
		fprintf(stderr, "ERR\n");
		return "ERR\n";
	}
	if(UID.empty() || GID.empty()){
		fprintf(stderr, "NOK: Missing argument(s)\n");
		return reply;
	}
	if(!user_logon(UID)){
		fprintf(stderr, "NOK: User not logged in\n");
		return reply;
	}

	if(validUID(UID) && !UID_free(UID)){
		if(validGID(GID) && groupExists(GID)){
			path.append(GID);path.append("/");
			path.append(UID);path.append(".txt");

			if(remove(path.c_str()) != 0){
				cout << "NOK: User doesn't susbscribe to this group!\n";
				reply = "RGU NOK\n";
			}
			else{
				cout << "OK: User successfully unsusbscribed to this group!\n";
				reply = "RGU OK\n";
			}
		}
		else{
			cout << "E_GRP: Invalid GID.\n";
			reply = "RGU E_GRP\n";
		}
	}
	else{
		cout << "E_USR: Invalid UID.\n";
		reply = "RGU E_USR\n";
	}
	if (verbose) {
		printVerbose(UID, GID);
	}
	return reply;
}

/*List of Groups subscribed by the user with the given UID.*/
string glm(string command){
	stringstream ss;
	string reply = "ERR\n";
	string cmd, UID;
	DIR *dir;
	struct dirent *diread;
	string path = "GROUPS";	
	size_t i = 0;
	vector<string> list;
	ss << command;
	getline(ss, cmd, ' ');
	getline(ss, UID);
	if(cmd.compare("GLM") != 0){
		fprintf(stderr, "ERR\n");
		return "ERR\n";
	}
	if(UID.empty()){
		fprintf(stderr, "ERR: Missing argument(s)\n");
		return reply;
	}

	if(validUID(UID) && !UID_free(UID) && user_logon(UID)){
		
		dir = opendir(path.c_str());

		while((diread = readdir(dir)) != nullptr){
			string gid;
			string GName;
			string MID;
			stringstream message;

			if(diread->d_name[0]=='.')
				continue;
			if(strlen(diread->d_name)>GID_LENGTH)
				continue;
			
			if(UID_in_group(UID, diread->d_name)){
				i++;
				gid = diread->d_name;
				GName = get_group_name(diread->d_name);
				MID = get_MID(diread->d_name);

				message << gid << " " << GName << " " << MID;
				list.push_back(message.str());
			}
		}
		if(i == 0){
			cout << "RGM 0: No groups subscribed\n";
			reply = "RGM 0\n";
		}
		else{
			sort(list.begin(), list.end());
			cout << "RGM " << i;
			reply = "RGM " + to_string(list.size());
			for(i = 0; i < list.size(); i++){
				cout << list[i] << endl;
				reply += " " + list[i];
			}
			reply += "\n";
		}
		closedir(dir);
	}
	else{
		cout << "E_USR: Invalid  UID or user isn't logged in.\n";
		reply = "RGM E_USR\n";
	}
	if (verbose) {
		printVerbose(UID, "");
	}
	return reply;
}

/*List of users subscribed to a group with the given GID.*/
void uls(TCPServer &tcp){
	string reply = "RUL NOK\n";
	string space, GID;
	DIR *dir;
	struct dirent *diread;
	string path = "GROUPS/";	
	vector<string> list;
	size_t i;

	space.assign(tcp.getData(1));
	if(space.compare("\n") == 0){
		fprintf(stderr, "NOK: Missing argument(s)\n");
		tcp.sendData(reply.c_str(), reply.length());
		return;
	}

	GID.assign(tcp.getData(GID_LENGTH));
	space.assign(tcp.getData(1));

	if(GID.empty()){
		fprintf(stderr, "NOK: Missing argument(s)\n");
		tcp.sendData(reply.c_str(), reply.length());
		return;
	}

	if(validGID(GID) && space.compare("\n") == 0){
			
		path.append(GID);

		dir = opendir(path.c_str());
		
		while((diread = readdir(dir)) != nullptr){
			string uid;
			string GName;
			stringstream message;
			if(diread->d_name[0]=='.')
				continue;
			if(strlen(diread->d_name) != 9)
				continue;


			/*Removing '.txt'*/
			string is_uid = diread->d_name;
			is_uid.erase(UID_LENGTH, MID_LENGTH);

			if(validUID(is_uid) && !UID_free(is_uid)){
				uid = is_uid;

				message << uid;
				list.push_back(message.str());
			}
			
		}

		cout << "RUL OK: " << get_group_name(GID) << " ";
		reply = "RUL OK " + get_group_name(GID) + " ";
		tcp.sendData(reply.c_str(), reply.length());

		for(i = 0; i < list.size(); i++){
			if(i == list.size()-1){
				cout << list[i] << "\n";
				reply = list[i] + "\n";
			}
			else{
				cout << list[i] << " ";
				reply = list[i] + " ";
			}
			tcp.sendData(reply.c_str(), reply.length());
		}
		closedir(dir);
		return;
	}
	else{
		cout << "NOK: Invalid GID or group doesn't exist.\n";
		reply = "RUL NOK\n";
		tcp.sendData(reply.c_str(), reply.length());
		return;
	}
	tcp.sendData(reply.c_str(), reply.length());
	return;
}

/*The user with the given UID posts a given message (text, which size is Tsize)
on the group with the given GID. It can include a file with the given name (Fname),
given size (Fsize) and given data.*/
void pst(TCPServer &tcp){
	string request;
	string reply = "RPT NOK\n";
	string space, UID, GID, Tsize, text, Fname, Fsize, data;	
	string status;
	
	
	space.assign(tcp.getData(1));
	
	UID.assign(tcp.getData(UID_LENGTH));
	space.assign(tcp.getData(1));
	
	GID.assign(tcp.getData(GID_LENGTH));
	space.assign(tcp.getData(1));
	
	Tsize.assign(tcp.getData(1));
	space.assign(tcp.getData(1));
	while(space.compare(" ") != 0){
		Tsize += space;
		space.assign(tcp.getData(1));
	}

	if(UID.empty() || GID.empty() || Tsize.empty()){
		fprintf(stderr, "NOK: Missing argument(s)\n");
		tcp.sendData(reply.c_str(), reply.length());
		return;
	}

	text.assign(tcp.getData(stoi(Tsize)), stoi(Tsize));

	if(text.empty()){
		fprintf(stderr, "NOK: Missing argument(s)\n");
		tcp.sendData(reply.c_str(), reply.length());
		return;
	}

	space.assign(tcp.getData(1));
	
	if(space.compare("\n") != 0){
		Fname.assign(tcp.getData(1));
		space.assign(tcp.getData(1));
		while(space.compare(" ") != 0){
			Fname += space;
			space.assign(tcp.getData(1));
		}

		Fsize.assign(tcp.getData(1));
		space.assign(tcp.getData(1));
		while(space.compare(" ") != 0){
			Fsize += space;
			space.assign(tcp.getData(1));
		}
	}

	if(validUID(UID) && !UID_free(UID) && validGID(GID) && UID_in_group(UID, GID)){
		if(validTextSize(Tsize)){
			if(Fname.empty()){
				if(max_MID(GID) == 9999){
					cout << "NOK: Maximum number of messages reached (9999).\n";
					tcp.sendData(reply.c_str(), reply.length());
					return;
				}
				else{
					status = post_text(UID, GID, text);
					cout << "RPT " << status << endl;
					reply = "RPT " + status + "\n";
					if(space.compare("\n") == 0){
						tcp.sendData(reply.c_str(), reply.length());
						return;
					}
				}

			}
			else{
				if(validFileInfo(Fname, Fsize)){
					if(max_MID(GID) == 9999){
						cout << "NOK: Maximum number of messages reached (9999).\n";
						tcp.sendData(reply.c_str(), reply.length());
						return;
					}
					else{
						status = post_text(UID, GID, text);
						string path = "GROUPS/";
						path.append(GID); path.append("/MSG/");
						path.append(status); path.append("/");
						path.append(Fname);

						tcp.getFileData(path, stoi(Fsize)), stoi(Fsize);
						cout << "RPT " << status << endl;
						reply = "RPT " + status + "\n";
						if(space.compare("\n") == 0){
							tcp.sendData(reply.c_str(), reply.length());
							return;
						}
					}
				}
				else{
					cout << "NOK: Invalid file info.\n";
					tcp.sendData(reply.c_str(), reply.length());
					return;
				}
			}
		}
		else{
			cout << "NOK: Invalid text info.\n";
			tcp.sendData(reply.c_str(), reply.length());
			return;
		}

	}
	else{
		cout << "NOK: Invalid arguments.\n";
		tcp.sendData(reply.c_str(), reply.length());
		return;
	}
	tcp.sendData(reply.c_str(), reply.length());
	if (verbose) {
		printVerbose(UID, GID);
	}
	return;
}

/*List of messages(max 20) from the group with the given GID
with MID greater or equal than the given MID, to the subscribed user with
the given UID.*/
void rtv(TCPServer &tcp){
	string reply = "RRT NOK\n";
	string space, UID, GID, MID;
	space.assign(tcp.getData(1));
	
	UID.assign(tcp.getData(UID_LENGTH));
	space.assign(tcp.getData(1));
	
	GID.assign(tcp.getData(GID_LENGTH));
	space.assign(tcp.getData(1));

	MID.assign(tcp.getData(MID_LENGTH), MID_LENGTH);
	space.assign(tcp.getData(1));

	if(UID.empty() || GID.empty() || MID.empty()){
		fprintf(stderr, "ERR: Missing argument(s)\n");
		tcp.sendData(reply.c_str(), reply.length());
		return;
	}
	if(!validUID(UID) || !validGID(GID) || !validMID(MID)){
		cout << reply;
		tcp.sendData(reply.c_str(), reply.length());
		return;
	}

	string path = "GROUPS/";
	DIR *dir;
	struct dirent *diread;
	int i = 0, N;

	path.append(GID); path.append("/MSG");

	dir = opendir(path.c_str());

	if(max_MID(GID) == 0 || stoi(MID) > max_MID(GID)){
		cout << "RRT EOF 0\n";
		reply = "RRT EOF 0\n";
		tcp.sendData(reply.c_str(), reply.length());
		return;
	}

	N = max_MID(GID)-stoi(MID)+1;
	cout << "RRT OK " << N << " ";
	reply = "RRT OK " + to_string(N) + " ";
	tcp.sendData(reply.c_str(), reply.length());
	while((diread = readdir(dir))!= nullptr && i < N){
		if(diread->d_name[0]=='.')
			continue;
		if(strlen(diread->d_name) != MID_LENGTH)
			continue;
		if(stoi(diread->d_name) < stoi(MID))
			continue;

		string currentMID;
		string currentUID;
		string Tsize;
		string text;
		string Fname;
		string Fsize = "";
		char data[COMMAND_SIZE];

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
		text = getFileData(text_path);

		Tsize = to_string(strlen(text.c_str()));
		Fname = getFileName(msg_path);
		i++;
		if(Fname.compare("") == 0){
			reply = currentMID + " " + currentUID + " " + Tsize + " " + text;
			cout << reply;
			tcp.sendData(reply.c_str(), reply.length());
		}
		else{
			file_path = msg_path; file_path.append("/"); file_path.append(Fname);
			Fsize = getFileSize(file_path);
			reply = currentMID + " " + currentUID + " " + Tsize + " " + text;
			cout << reply;
			tcp.sendData(reply.c_str(), reply.length());
			reply = " / " + Fname + " " + Fsize + " ";
			cout << reply;
			tcp.sendData(reply.c_str(), reply.length());
			ifstream fileFile(file_path, std::ios_base::binary);
			while(true){
				memset(data, 0, COMMAND_SIZE);
            	fileFile.read(data, COMMAND_SIZE);
				cout << data;
				if(stoi(Fsize) < COMMAND_SIZE){
					tcp.sendData(data, stoi(Fsize));
				}
				else{
					tcp.sendData(data, COMMAND_SIZE);
				}
				if(fileFile.tellg() == -1){
					break;
				}
			}
		}
		if(i != N){
			cout << " ";
			tcp.sendData(" ", 1);
		}
		else{
			cout << "\n";
			tcp.sendData("\n", 1);
		}
	}
	closedir(dir);
	if (verbose) {
		printVerbose(UID, GID);
	}
	return;
}

/*TODO:
	- Does the user have to be log on to subscribe/unsubscribe?
	- special UIDs and GROUPs
	- How to deal with files when they are sent*/
