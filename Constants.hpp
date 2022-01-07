#ifndef CONSTANTS
#define CONSTANTS

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
#include <regex>
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

// Functions
#define max(A,B) ((A)>=(B)?(A):(B))

inline string remove_new_line(string s){
	if(s.substr(s.length()-1, s.length()).compare("\n") == 0){
		return s.substr(0, s.length()-1);
	}
	return s;
}

// Values
#define DSPORT_DEFAULT "58013"
#define DSIP_DEFAULT "localhost"
#define COMMAND_SIZE 128

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

static vector<string> ClientUser_local = {
	"showuid",
	"showgid",
	"select",
};

static vector<string> ClientUser_local_abrev = {
	"su",
	"sg",
	"sag",
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
	"reg",
	"unr",
	"login",
	"logout",
	"gl",
	"s",
	"u",
	"mgl",
};

static vector<string> ClientUser_TCP = {
	"post",
	"retrieve",
	"ulist",
};

static vector<string> ClientUser_TCP_abrev = {
	"post",
	"r",
	"ul",
};

/*
static vector<string> UserDS_UDP = {
	"REG",
	"UNR",
	"LOG",
	"OUT",
	"GLS",
	"GSR",
	"GUR",
	"GLM",
};

static vector<string> UserDS_UDP_response = {
	"RRG",
	"RUN",
	"RLO",
	"ROU",
	"RGL",
	"RGS",
	"RGU",
	"RGM",
};

static vector<string> UserDS_TCP = {
	"ULS",
	"PST",
	"RTV",
};

static vector<string> UserDS_TCP_response = {
	"RUL",
	"RPT",
	"RRT",
};
*/
#endif
