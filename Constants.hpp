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

static vector<string> ClientUser_UDP = {
	"reg",
	"unregister",
	"login",
	"logout",
	"exit",
	"groups",
	"subscribe",
	"unsubscribe",
	"my_groups",
	"select",
};

static vector<string> ClientUser_UDP_abrev = {
	"unr",
	"gl",
	"s",
	"u",
	"mgl",
	"sag",
};

static vector<string> ClientUser_TCP = {
	"post",
	"retrieve",
};

static vector<string> ClientUser_TCP_abrev = {
	"r",
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
	"PST",
	"RTV",
};

static vector<string> UserDS_TCP_response = {
	"RPT",
	"RRT",
};

#endif
