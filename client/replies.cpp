#include "../Constants.hpp"

string selected_UID, selected_GID, sent_GName, password;

//TODO: por isto num ficheiro comum a todos...
string remove_new_line(string s){
	if(s.substr(s.length()-1, s.length()).compare("\n") == 0){
		return s.substr(0, s.length()-1);
	}
	return s;
}

void save_login(string remaining){
    stringstream ss;
	ss << remaining;
	getline(ss, selected_UID, ' ');
	getline(ss, password, ' ');
}

void save_logout(string remaining){

}

void save_subscribe(string remaining){

}

void save_unsubscribe(string remaining){

}

void save_my_groups(string remaining){

}

void save_ulist(string remaining){

}

void save_post(string remaining){

}

void save_retrieve(string remaining){

}

string showuid(){
    if(selected_UID.empty()){
        return "You are not logged in\n";
    }
    return "Your UID is " + selected_UID + "\n";
}

string showgid(){
    if(selected_GID.empty()){
        return "You have not selected an active group\n";
    }
    return "Group " + selected_GID + " - is the active group\n"; //TODO: por o nome do grupo?
}

string select_GID(string GID){
    selected_GID = GID;
    return "Group " + selected_GID + " - \"" + sent_GName + "\"is now the active group\n"; //TODO: por o nome do grupo?
}

string rrg(string command){
    if(command.compare("RRG OK\n") == 0){
        return "User successfully registered\n";
    }
    else if(command.compare("RRG DUP\n") == 0){
        return "User was already registered before\n";
    }
    else if(command.compare("RRG NOK\n") == 0){
        return "Something went wrong. User was not registered\n";
    }
    return "Something went wrong\n";
}

string run(string command){
    if(command.compare("RUN OK\n") == 0){
        return "User successfully unregistered\n";
    }
    else if(command.compare("RUN NOK\n") == 0){
        return "Something went wrong. User was not unregistered\n";
    }
    return "Something went wrong\n";
}

string rlo(string command){
    if(command.compare("RLO OK\n") == 0){
        return "You are now logged in\n";
    }
    else if(command.compare("RLO NOK\n") == 0){
        selected_UID = "";
        password = "";
        return "Something went wrong. User could not login\n";
    }
    selected_UID = "";
    password = "";
    return "Something went wrong\n";
}

string rou(string command){
    if(command.compare("ROU OK\n") == 0){
        selected_UID = "";
        return "You are now logged out\n";
    }
    else if(command.compare("ROU NOK\n") == 0){
        return "Something went wrong. User could not logout\n";
    }
    return "Something went wrong\n";
}

string rgl(string command){
    stringstream ss;
    string cmd, N, GID, GName, MID, reply = "";
	ss << command;
	getline(ss, cmd, ' ');
	getline(ss, N, ' ');
    N = remove_new_line(N);
    if(cmd.compare("RGL") == 0){
        if(N.compare("0") == 0){
            return "There are no groups available\n";
        }
        else{
            for(int i = stoi(N); i > 0; i--){
                getline(ss, GID, ' ');
                getline(ss, GName, ' ');
                getline(ss, MID, '\n');
                reply += "Group " + GID + " - \"" + GName + "\" " + MID + "\n";
            }
            return reply;
        }
    }
    return "Something went wrong\n";
}

string rgs(string command){
    stringstream ss;
    string cmd, status, GID;
	ss << command;
	getline(ss, cmd, ' ');
	getline(ss, status, ' ');
    status = remove_new_line(status);
    getline(ss, GID);
    if(cmd.compare("RGS") == 0){
        if(status.compare("NEW") == 0){
            return "New group created and subscribed: " + GID + " - \"" + sent_GName + "\"\n";
        }
        else if(status.compare("E_USR") == 0){
            return "User could not create nor subscribe the group. Invalid UID\n";
        }
        else if(status.compare("E_GRP") == 0){
            return "User could not create nor subscribe the group. Invalid GID\n";
        }
        else if(status.compare("E_GNAME") == 0){
            return "User could not create nor subscribe the group. Invalid group name\n";
        }
        else if(status.compare("E_FULL") == 0){
            return "User could not create the group. Reached groups limit\n";
        }
        else if(status.compare("NOK") == 0){
            return "Something went wrong. User could not subscribe nor create the group\n";
        }
    }
    return "Something went wrong\n";
}

string rgu(string command){
    if(command.compare("RGU OK\n") == 0){
        return "User unsubscribed from group\n";
    }
    else if(command.compare("RGU E_USR\n") == 0){
        return "User could not unsubscribe from group. Invalid UID\n";
    }
    else if(command.compare("RGU E_GRP\n") == 0){
        return "User could not unsubscribe from group. Invalid GID\n";
    }
    else if(command.compare("RGU NOK\n") == 0){
        return "Something went wrong. User could not unsubscribe from group\n";
    }
    return "Something went wrong\n";
}

string rgm(string command){
    stringstream ss;
    string cmd, N, GID, GName, MID, reply = "";
	ss << command;
	getline(ss, cmd, ' ');
	getline(ss, N, ' ');
    N = remove_new_line(N);
    if(cmd.compare("RGM") == 0){
        if(N.compare("0") == 0){
            return "User has not subscribed to any groups\n";
        }
        else if(N.compare("E_USR") == 0){
            return "User is not logged in or UID is invalid\n";
        }
        else{
            for(int i = stoi(N); i > 0; i--){
                getline(ss, GID, ' ');
                getline(ss, GName, ' ');
                getline(ss, MID, '\n');
                reply += "Group " + GID + " - \"" + GName + "\" " + MID + "\n";
            }
            return reply;
        }
    }
    return "Something went wrong\n";
}

string rul(string command){
    stringstream ss;
    string cmd, status, GName, UID, line, reply = "";
	ss << command;
	getline(ss, cmd, ' ');
	getline(ss, status, ' ');
    getline(ss, GName, ' ');
    GName = remove_new_line(GName);
    if(cmd.compare("RGM") == 0){
        if(status.compare("OK") == 0){
            reply = "Users subscribed to group \"" + GName + "\":\n";
            while(getline(ss, line, '\n')){
                reply += line + "\n";
            }
            return reply;
        }
        else if(status.compare("NOK") == 0){
            return "Something went wrong or the group doesn't exist\n";
        }
    }
    return "Something went wrong\n";
}

string rpt(string command){
    stringstream ss;
    string cmd, status;
	ss << command;
	getline(ss, cmd, ' ');
	getline(ss, status, '\n');
    if(cmd.compare("RPT") == 0){
        if(status.compare("NOK") == 0){
            return "Something went wrong. User could not post\n";
        }
        else{
            return "posted message number " + status + " to group " + selected_GID + " - \"" + sent_GName + "\"\n";
        }
    }
    return "Something went wrong\n";
}

string rrt(string command){
    stringstream ss;
    string cmd, status, N, MID, UID, Tsize, text, Fname, Fsize, data, new_line, reply = "";
	char *text_p, *data_p, *new_line_p;
    ss << command;
	getline(ss, cmd, ' ');
	getline(ss, status, ' ');
    status = remove_new_line(status);
    getline(ss, N, ' ');
    if(cmd.compare("RRT") == 0){
        if(status.compare("OK") == 0){
            reply = N + " message(s) retrieved:\n";
            for(int i = stoi(N); i > 0; i--){
                getline(ss, MID, ' ');
                getline(ss, UID, ' ');
                getline(ss, Tsize, ' ');
                ss.read(text_p, stoi(Tsize));
                text.assign(text_p);
                reply += MID + " - \"" + text + "\";";
                ss.read(new_line_p, 1);
                new_line.assign(new_line_p);
                if(new_line.compare("\n") == 0){
                    reply += "\n";
                    continue;
                }
                getline(ss, Fname, ' ');
                getline(ss, Fsize, ' ');
                ss.read(data_p, stoi(Fsize));
                data.assign(data_p);
                ss.read(new_line_p, 1);
                reply += " file stored: " + Fname + "\n";
            }
            return reply;
        }
        else if(status.compare("EOF") == 0){
            return "There are no messages available\n";
        }
        else if(status.compare("NOK") == 0){
            return "Something went wrong. User could not retrieve any messages\n";
        }
    }
    return "Something went wrong\n";
}
