#ifndef COMMON
#define COMMON

#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstring>
#include <csignal>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
extern "C" { // For C libraries, to avoid namespace cluttering
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <dirent.h>
}

using namespace std;

// Values
#define DSPORT_DEFAULT "58013"
#define DSIP_DEFAULT "localhost"
#define COMMAND_SIZE 512
#define UID_LENGTH 5
#define PASS_LENGTH 8
#define GID_LENGTH 2
#define GNAME_LENGTH 24
#define MID_LENGTH 4
#define TSIZE_LENGTH 240
#define FNAME_LENGTH 24
#define FSIZE_LENGTH 10

static map<string, string> commands = {
	{"showuid", "showuid"},
	{"su", "showuid"},
	{"showgid", "showgid"},
	{"sg", "showgid"},
	{"select", "select"},
	{"sag", "select"},
	{"exit", "exit"},
	{"reg", "REG"},
	{"unregister", "UNR"},
	{"unr", "UNR"},
	{"login", "LOG"},
	{"logout", "OUT"},
	{"groups", "GLS"},
	{"gl", "GLS"},
	{"subscribe", "GSR"},
	{"s", "GSR"},
	{"unsubscribe", "GUR"},
	{"u", "GUR"},
	{"my_groups", "GLM"},
	{"mgl", "GLM"},
	{"ulist", "ULS"},
	{"ul", "ULS"},
	{"post", "PST"},
	{"retrieve", "RTV"},
	{"r", "RTV"},
	{"ERR", "ERR"},
};

static vector<string> ClientUser_UDP = {
	"reg",
	"unregister",
	"login",
	"logout",
	"groups",
	"subscribe",
	"unsubscribe",
	"my_groups",
};

static vector<string> ClientUser_UDP_abrev = {
	"unr",
	"gl",
	"s",
	"u",
	"mgl",
};

static vector<string> ClientUser_TCP = {
	"ulist",
	"post",
	"retrieve",
};

static vector<string> ClientUser_TCP_abrev = {
	"ul",
	"r",
};

// Functions
#define max(A,B) ((A)>=(B)?(A):(B))

inline string remove_new_line(string s){
	if(!s.empty() && s.substr(s.length()-1, s.length()).compare("\n") == 0){
		return s.substr(0, s.length()-1);
	}
	return s;
}

/*Verifies if a string only consists of digits.*/
inline bool isNumber(string str){
	for(size_t i = 0; i < str.length(); i++)
      	if(! (str[i] >= '0' && str[i] <= '9') ) 
      		return false;

	return true;
}

/*Verifies if a string only consists of alphanumerical characters.*/
inline bool isAlNum(string str){
	for (int i=0; i<PASS_LENGTH; i++)
		if (!isalnum(str[i]))
			return false;
	return true;
}

/*Deletes all files from a given directory.*/
inline void delete_files(string path){
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

/*Verifies if a file exists based on its name*/
inline bool fileExists(string Fname){
	DIR *dir;
	struct dirent *diread;

	dir = opendir(".");

	while((diread = readdir(dir))!= nullptr){

		if(Fname == diread->d_name){
			closedir(dir);
			return true;
		}
	}
	
	closedir(dir);
	return false;	
}

/*Verifies if the message text is valid (maximum of 240 characters).*/
inline bool validTextSize(string Tsize){
	return stoi(Tsize) <= TSIZE_LENGTH;
}

/*Returns the size of the file in a given message.*/
inline string getFileSize(string path){

	ifstream fileSize(path, ios::binary);
	fileSize.seekg(0, ios::end);
	return to_string(fileSize.tellg());
}

/*Return the file data of a given message.*/
inline string getFileData(string path){
	ifstream fileFile(path);
	stringstream ss;

	ss << fileFile.rdbuf();

	return ss.str();
}

#endif
