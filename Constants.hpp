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

// Functions
#define max(A,B) ((A)>=(B)?(A):(B))

// Values
#define DSPORT_DEFAULT "58013"
#define DSIP_DEFAULT "localhost"
#define COMMAND_SIZE 128

using namespace std;

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
	"exit",
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
	"exit",
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

#endif
