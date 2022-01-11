#include "../Common.hpp"
#include "./TCPClient.cpp"
#include "./UDPClient.cpp"
#include "./replies.cpp"

// TODO:
// buffer nao pode ser so 10000
// ficheiros grandes....
// verificar login e talvez select
string processCommand(const char *message) {
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
	if(code->second.compare("ULS") == 0){
		remaining = save_ulist(remaining);
	}
	if(code->second.compare("PST") == 0){
		remaining = save_post(remaining);
		if(remaining.compare("ERR") == 0){
			return "ERR";
		}
	}
	if(code->second.compare("RTV") == 0){
		remaining = save_retrieve(remaining);
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
	if(cmd.compare("RRT") == 0)
		return rrt(command);
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

			cmd = processCommand(input);

			if(cmd.compare("ERR") != 0){
				// write(1, cmd.c_str(), strlen(cmd.c_str()));
				tcp.sendDataString(cmd);
				cmd = tcp.getData(COMMAND_SIZE);
				reply = functionCaller(cmd);
				fprintf(stdout, "%s", reply.c_str());
			}
			else{
				fprintf(stderr, "Error: invalid command or something went wrong\n");
			}
		}

		else if(isUDP(cmd)){
			UDPClient udp = UDPClient(DSIP, DSport);
			
			//Logout before exiting
			if(cmd.compare("exit\n") == 0){
				exit(0);
			}

			cmd = processCommand(input);

			if(cmd.compare("ERR") != 0){
				// write(1, cmd.c_str(), strlen(cmd.c_str()));
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
				// write(1, cmd.c_str(), strlen(cmd.c_str()));
				fprintf(stdout, "%s", reply.c_str());
			}
			else{
				fprintf(stderr, "Error: invalid command or something went wrong\n");
			}
		}
	}

	return 0;
}
