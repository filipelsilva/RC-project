#include "../Constants.hpp"
#include "./TCPClient.cpp"
#include "./UDPClient.cpp"
#include "./replies.cpp"

string processCommand(const char *message) {
	//int index;
	string cmd, remaining;
	stringstream ss;
	ss << message;
	getline(ss, cmd, ' ');
	getline(ss, remaining);
	if(remaining.empty()){
		cmd = remove_new_line(cmd);
		remaining = "\n";
	}
	else{
		remaining = " " + remaining + "\n";
	}

	auto code = commands.find(cmd);
	if(code->second.compare("ERR") != 0){
		return code->second + remaining;
	}
	else{
		return "ERR";
	}

	//TODO: fazer diferente para ulist, post e retrieve
	//tbh refazer isto e percorrer simplesmente uma lista com todos pq o q esta 
	//em cima e pq isto nao funciona para o ultimo de cada vetor
	/*
	auto location = find(ClientUser_TCP.begin(), ClientUser_TCP.end(), cmd);
	if (location != ClientUser_TCP.end()) {
		index = location - ClientUser_TCP.begin();
		return UserDS_TCP[index] + " " + remaining + "\n";
	}

	location = find(ClientUser_TCP_abrev.begin(), ClientUser_TCP_abrev.end(), cmd);
	if (location != ClientUser_TCP_abrev.end()) {
		index = location - ClientUser_TCP_abrev.begin();
		return UserDS_TCP[index] + " " + remaining + "\n";
	}

	location = find(ClientUser_UDP.begin(), ClientUser_UDP.end(), cmd);
	if (location != ClientUser_UDP.end()) {
		index = location - ClientUser_UDP.begin();
		return UserDS_UDP[index] + " " + remaining + "\n";
	}

	location = find(ClientUser_UDP_abrev.begin(), ClientUser_UDP_abrev.end(), cmd);
	if (location != ClientUser_UDP_abrev.end()) {
		index = location - ClientUser_UDP_abrev.begin();
		return UserDS_UDP[index] + " " + remaining + "\n";
	}
	*/
}

string processLocalCommand(string command){
	stringstream ss;
	string reply = "ERR";
	string cmd, GID;
	ss << command;
	getline(ss, cmd, ' ');
	getline(ss, GID);
	cmd = remove_new_line(cmd);
	if(cmd.compare("showuid") == 0 || cmd.compare("su") == 0)
		reply = showuid();
	if(cmd.compare("showgid") == 0 || cmd.compare("sg") == 0)
		reply = showgid();
	if(cmd.compare("select") == 0 || cmd.compare("sag") == 0)
		reply = select_GID(GID);
	if(cmd.compare("exit") == 0)
		exit(0);
	return reply;
}

bool isTCP(string input){
	string command;
	stringstream ss;
	ss << input;
	getline(ss, command, ' ');
	command = remove_new_line(command);
	for (string cmd: ClientUser_TCP){
		if (command.compare(cmd) == 0)
			return true;
	}
	for (string cmd: ClientUser_TCP_abrev){
		if (command.compare(cmd) == 0)
			return true;
	}
	return false;
}

bool isUDP(string input){
	string command;
	stringstream ss;
	ss << input;
	getline(ss, command, ' ');
	command = remove_new_line(command);
	for (string cmd: ClientUser_UDP){
		if (command.compare(cmd) == 0)
			return true;
	}
	for (string cmd: ClientUser_UDP_abrev){
		if (command.compare(cmd) == 0)
			return true;
	}
	return false;
}

string functionCaller(string command){
	string cmd = command.substr(0, 3);
	if(cmd.compare("RRG") == 0)
		return rrg(command);
	if(cmd.compare("RUN") == 0)
		return run(command);
	if(cmd.compare("RLO") == 0)
		return rlo(command);
	if(cmd.compare("ROU") == 0)
		return rou(command);
	if(cmd.compare("RGL") == 0)
		return rgl(command);
	if(cmd.compare("RGS") == 0)
		return rgs(command);
	if(cmd.compare("RGU") == 0)
		return rgu(command);
	if(cmd.compare("RGM") == 0)
		return rgm(command);
	if(cmd.compare("RUL") == 0)
		return rul(command);
	if(cmd.compare("RPT") == 0)
		return rpt(command);
	/*
	if(cmd.compare("RRT") == 0)
		return rrt(command);
	*/
	return "Something went wrong\n";
}

int main(int argc, char **argv) {
	/*
	for (auto str : ClientUser_TCP) {
		char command[COMMAND_SIZE];
		memset(command, 0, COMMAND_SIZE);
		strcat(command, processCommand(str.c_str()));
		// strcat(command, "\n");
		printf("%s %s\n", str.c_str(), command);
	}
	for (auto str : ClientUser_TCP_abrev) {
		char command[COMMAND_SIZE];
		memset(command, 0, COMMAND_SIZE);
		strcat(command, processCommand(str.c_str()));
		// strcat(command, "\n");
		printf("%s %s\n", str.c_str(), command);
	}
	for (auto str : ClientUser_UDP) {
		char command[COMMAND_SIZE];
		memset(command, 0, COMMAND_SIZE);
		strcat(command, processCommand(str.c_str()));
		// strcat(command, "\n");
		printf("%s %s\n", str.c_str(), command);
	}
	for (auto str : ClientUser_UDP_abrev) {
		char command[COMMAND_SIZE];
		memset(command, 0, COMMAND_SIZE);
		strcat(command, processCommand(str.c_str()));
		// strcat(command, "\n");
		printf("%s %s\n", str.c_str(), command);
	}
	*/
	const char* usage = "Usage: %s [-n DSIP] [-p DSport]\n"
		"\t-n DSIP\t\tIP address of the machine where the DS is running\n"
		"\t-p PORT\t\tPort where the DS server accepts requests\n";

	// Default initialization of variables and flags
	const char* DSIP = DSIP_DEFAULT;
	const char* DSport = DSPORT_DEFAULT;
	char flag;
	//char command[COMMAND_SIZE];

	// Argument parser
	while ((flag = getopt(argc, argv, "n:p:")) != -1) {
		switch (flag) {
			case 'n':
				DSIP = optarg;
				break;

			case 'p':
				if (strtol(optarg, NULL, 10) == 0) {
					fprintf(stderr, "Error: invalid value for -p flag\n");
					fprintf(stderr, usage, argv[0]);
					exit(1);
				}
				DSport = optarg;
				break;

			default:
				fprintf(stderr, usage, argv[0]);
				exit(1);
				break;
		}
	}

	printf("DSIP: %s\nDSport: %s\n", DSIP, DSport);

	while(1){
		write(1, "> ", strlen("> "));
		char input[COMMAND_SIZE];
		string cmd, reply;
		if(fgets(input, COMMAND_SIZE, stdin) == NULL){
			fprintf(stderr, "Error: something went wrong while getting user input\n");
			exit(1);
		}

		//memset(&command, 0, sizeof(command));

		if(isTCP(cmd.assign(input))){
			TCPClient tcp = TCPClient(DSIP, DSport);
			//strcat(command, processCommand(input));
			cmd = processCommand(input);
			if(cmd.compare("ERR") != 0){
				tcp.sendData(cmd.c_str());
				reply = functionCaller(tcp.getData());
				fprintf(stdout, "%s", reply.c_str());
			}
			else{
				fprintf(stderr, "Error: invalid command\n");
			}
		}
		else if(isUDP(cmd)){
			UDPClient udp = UDPClient(DSIP, DSport);
			//strcat(command, processCommand(input));
			cmd = processCommand(input);
			if(cmd.compare("ERR") != 0){
				udp.sendData(cmd.c_str());
				reply = functionCaller(udp.getData());
				fprintf(stdout, "%s", reply.c_str());
			}
			else{
				fprintf(stderr, "Error: invalid command\n");
			}
		}
		else{
			reply = processLocalCommand(cmd);
			if(reply.compare("ERR") != 0){
				fprintf(stdout, "%s", reply.c_str());
			}
			else{
				fprintf(stderr, "Error: invalid command\n");
			}
		}
	}

	return 0;
}
