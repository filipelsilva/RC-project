#include "../Common.hpp"
#include "./TCPClient.cpp"
#include "./UDPClient.cpp"
#include "./replies.cpp"

/*Processes the command given by the user(UDP).*/
string processUDPCommand(const char *message) {
	string cmd, remaining;
	stringstream ss;
	ss << message;
	getline(ss, cmd, ' ');
	getline(ss, remaining);
	cmd = remove_new_line(cmd);
	auto code = commands.find(cmd);
	if(code->second.compare("LOG") == 0){
		remaining = save_login(remaining);
	}
	if(code->second.compare("OUT") == 0){
		remaining = save_logout(remaining);
	}
	if(code->second.compare("GSR") == 0){
		remaining = save_subscribe(remaining);
	}
	if(code->second.compare("GUR") == 0){
		remaining = save_unsubscribe(remaining);
	}
	if(code->second.compare("GLM") == 0){
		remaining = save_my_groups(remaining);
	}
	if(code->second.compare("ERR") != 0){
		if (!remaining.empty()){
			return code->second + " " + remaining + "\n";
		}
		else{
			return code->second + "\n";
		}
	}
	else{
		return "ERR";
	}
}

/*Processes the command given by the user(TCP).*/
string processTCPCommand(const char *message, TCPClient &tcp){
	string cmd, remaining;
	stringstream ss;
	ss << message;
	getline(ss, cmd, ' ');
	getline(ss, remaining);
	cmd = remove_new_line(cmd);
	auto code = commands.find(cmd);
	if(code->second.compare("ULS") == 0){
		ulist(remaining, tcp);
		return "";
	}
	if(code->second.compare("PST") == 0){
		remaining = post(remaining, tcp);
		if(remaining.compare("ERR") == 0){
			return "ERR";
		}
		return "";
	}
	if(code->second.compare("RTV") == 0){
		retrieve(remaining, tcp);
		return "";
	}
	return "ERR";
}

/*Processes the command given by the user(Local variables).*/
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
	if(cmd.compare("exit") == 0){
		exit(0);
	}
	return reply;
}

/*Returns true if the given command by the user is supported by a TCP protocol.*/
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

/*Returns true if the given command by the user is supported by a UDP protocol.*/
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

/*Calls the function to execute the command given by the user.*/
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
	return "Something went wrong\n";
}

int main(int argc, char **argv) {
	const char* usage = "Usage: %s [-n DSIP] [-p DSport]\n"
		"\t-n DSIP\t\tIP address of the machine where the DS is running\n"
		"\t-p PORT\t\tPort where the DS server accepts requests\n";

	// Default initialization of variables and flags
	const char* DSIP = DSIP_DEFAULT;
	const char* DSport = DSPORT_DEFAULT;
	char flag;

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

	//Input cycle
	while(1){
		write(1, "> ", strlen("> "));
		char input[COMMAND_SIZE];
		string cmd, reply;
		if(fgets(input, COMMAND_SIZE, stdin) == NULL){
			fprintf(stderr, "Error: something went wrong while getting user input\n");
			exit(1);
		}

		if(isTCP(cmd.assign(input))){
			TCPClient tcp = TCPClient(DSIP, DSport);
			tcp.createSocketAndConnect();

			cmd = processTCPCommand(input, tcp);

			if(cmd.compare("ERR") == 0){
				fprintf(stderr, "Error: invalid command or something went wrong\n");
			}
			else{
			}
		}

		else if(isUDP(cmd)){
			UDPClient udp = UDPClient(DSIP, DSport);
			cmd = processUDPCommand(input);

			if(cmd.compare("ERR") != 0){
				udp.sendData(cmd.c_str(), cmd.length());
				cmd = udp.getData(COMMAND_SIZE);
				reply = functionCaller(cmd);
				fprintf(stdout, "%s", reply.c_str());
			}
			else{
				fprintf(stderr, "Error: invalid command or something went wrong\n");
			}
		}

		else{
			reply = processLocalCommand(cmd);

			if(reply.compare("ERR") != 0){
				fprintf(stdout, "%s", reply.c_str());
			}
			else{
				fprintf(stderr, "Error: invalid command or something went wrong\n");
			}
		}
	}

	return 0;
}
