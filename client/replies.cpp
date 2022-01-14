#include "../Common.hpp"

/*Variables that save the current user's info*/
string selected_UID, selected_GID, sent_GName, password;
bool logged_in = false;

/*Current user asks to 'login', returns the formated command to send to the server.*/
string save_login(string remaining){
    stringstream ss;
	ss << remaining;
    if(selected_UID.empty() && password.empty()){
	    getline(ss, selected_UID, ' ');
        getline(ss, password);
    }
    return remaining;
}

/*Current user asks to 'logout', returns the formated command to send to the server.*/
string save_logout(string remaining){
    remaining += selected_UID + " " + password;
    return remaining;
}

/*Current user asks to 'subscribe' to a given group, returns the formated command
 to send to the server.*/
string save_subscribe(string remaining){
    stringstream ss;
    string GID, GName;
	ss << remaining;
    getline(ss, GID, ' ');
    getline(ss, GName);
    sent_GName = GName;
    switch (GID.length()) {
        case 1:
            GID = "0" + GID;
            break;
        case 2:
            break;
        default:
            break;
    }
    return selected_UID + " " + GID + " " + GName;
}

/*Current user asks to 'unsubscribe' to a given group, returns the formated command
 to send to the server.*/
string save_unsubscribe(string remaining){
    string GID = remaining;
    switch (GID.length()) {
        case 1:
            GID = "0" + GID;
            break;
        case 2:
            break;
        default:
            break;
    }
    return selected_UID + " " + GID;
}

/*Current user asks to see the groups he is currently subscribed to ('my_groups'),
 returns the formated command to seend to the server.*/
string save_my_groups(string remaining){
    return selected_UID + remaining;
}

/*Current user asks to show hid UID ('showuid'), returns message to show to the user.*/
string showuid(){
    if(selected_UID.empty()){
        return "You are not logged in\n";
    }
    return "Your UID is " + selected_UID + "\n";
}
/*Current user asks to show the GID of the group that is currently selected ('showgid'),
 returns message to show to the user.*/
string showgid(){
    if(selected_GID.empty()){
        return "You have not selected an active group\n";
    }
    return "Group " + selected_GID + " is the active group\n";
}

/*Current user asks to select the group with the given GID, returns message to
 show to the user.*/
string select_GID(string GID){
    if(!logged_in){
		return "User not logged in\n";
	}
    switch (GID.length()) {
        case 1:
            GID = "0" + GID;
            break;
        case 2:
            break;
        default:
            return "Inavlid group number\n";
    }
    selected_GID = GID;
    string reply = "Group " + selected_GID + " is now the active group\n";
    sent_GName = "";
    return reply;
}

/*Returns message to show to the user about the registration request status
 returned by the server.*/
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

/*Returns message to show to the user about the unregistration request status
 returned by the server.*/
string run(string command){
    if(command.compare("RUN OK\n") == 0){
        return "User successfully unregistered\n";
    }
    else if(command.compare("RUN NOK\n") == 0){
        return "Something went wrong. User was not unregistered\n";
    }
    return "Something went wrong\n";
}

/*Returns message to show to the user about the login request status
 returned by the server.*/
string rlo(string command){
    if(command.compare("RLO OK\n") == 0){
        logged_in = true;
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

/*Returns message to show to the user about the logout request status
 returned by the server.*/
string rou(string command){
    if(command.compare("ROU OK\n") == 0){
        selected_UID = "";
        password = "";
        logged_in = false;
        return "You are now logged out\n";
    }
    else if(command.compare("ROU NOK\n") == 0){
        return "Something went wrong. User could not logout\n";
    }
    return "Something went wrong\n";
}

/*Returns the existing groups after the user requests them to the server('ulist').*/
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

/*Returns message to show to the user about the susbcribe request status
 returned by the server.*/
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
        else if(status.compare("OK") == 0){
            return "User successfully subscribed to group\n";
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

/*Returns message to show to the user about the unsubscribe request status
 returned by the server.*/
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

/*Returns the list of groups subscribed by the user after the user
 requests them to the server('my_groups') or returns a message about the
 request status returned by the server.*/
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

/*Returns the list of users subscribed to the group that is requested by the current user
 or returns a message about the request status returned by the server.*/
void ulist(string remaining, TCPClient &tcp){
    string request, reply;
    if(!logged_in){
        fprintf(stdout, "User not logged in\n");
        return;
    }
    if(selected_GID.compare("") != 0){
        request = "ULS " + selected_GID + remaining + "\n";
    }
    else{
        request = "ULS\n";
    }
    tcp.sendData(request.c_str(), request.length());
    reply = tcp.getData(COMMAND_SIZE);
    stringstream ss;
    string cmd, status, GName, UID, prev_UID;
	ss << reply;
	getline(ss, cmd, ' ');
    getline(ss, status, ' ');
    getline(ss, GName, ' ');
    GName = remove_new_line(GName);
    if(cmd.compare("RUL") == 0){
        if(status.compare("OK") == 0){
            fprintf(stdout, "Users subscribed to group \"%s\":\n", GName.c_str());
            while(reply.length() != 0){
                while(ss.tellg() != -1){
                    getline(ss, UID, ' ');
                    if(prev_UID != ""){
                        UID = prev_UID + UID;
                        prev_UID = "";
                    }
                    if(UID.length() == UID_LENGTH){
                        fprintf(stdout, "%s\n", UID.c_str());
                    }
                    else if(UID.length() == 6){
                        fprintf(stdout, "%s", UID.c_str());
                        return;
                    }
                    else if(UID.length() < UID_LENGTH){
                        prev_UID = UID;
                    }
                }
                reply = tcp.getData(COMMAND_SIZE);
                ss.clear();
                ss.str("");
                ss << reply;
            }
            return;
        }
        else if(status.compare("NOK") == 0){
            fprintf(stdout, "Something went wrong or the group doesn't exist\n");
            return;
        }
    }
    fprintf(stdout, "Something went wrong\n");
    return;
}

/*Sends a message to the group selected by the user (could contain a file or not),
 returns the request status returned by the server.*/
string post(string remaining, TCPClient &tcp){
    string request, reply;
    stringstream ss;
    string text, Fname;
    string c1, c2;
    string Tsize, Fsize;
    char data[COMMAND_SIZE];
    char c;
	ss << remaining;
    ss.get(c);
    c1 = c;
    c = 'i';
    while((c != '\"') && (ss.tellg() != -1)){
        ss.get(c);
        text += c;
    }
    text.pop_back();

    c2 = c;
    ss.get(c);

    if(c != '\n'){
        getline(ss, Fname);
    }

    if(c1.compare("\"") != 0 || c2.compare("\"") != 0){
        return "ERR";
    }
    if(!Fname.empty() && !fileExists(Fname)){
        return "ERR";
    }
    
    Tsize = to_string(text.length());

    if(selected_UID.compare("") == 0 || selected_GID.compare("") == 0){
        request = "PST\n";
    }
    else{
        request = "PST " + selected_UID + " " + selected_GID + " " + Tsize + " " + text;
    }
    if(Fname.empty()){
        request += "\n";
    }
    tcp.sendData(request.c_str(), request.length());

    if(!Fname.empty() && fileExists(Fname)){
        Fsize = getFileSize(Fname);
        request = " " + Fname + " " + Fsize + " ";
        tcp.sendData(request.c_str(), request.length());
        ifstream fileFile(Fname, std::ios_base::binary);
        while(true){
            memset(data, 0, COMMAND_SIZE);
            fileFile.read(data, COMMAND_SIZE);
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
        fileFile.close();
        tcp.sendData("\n", 1);
    }
    reply = tcp.getData(COMMAND_SIZE);
    string cmd, status;
    ss.clear();
    ss.str("");
	ss << reply;
	getline(ss, cmd, ' ');
	getline(ss, status, '\n');
    if(cmd.compare("RPT") == 0){
        if(status.compare("NOK") == 0){
            fprintf(stdout, "Something went wrong. User could not post\n");
            return "";
        }
        else{
            fprintf(stdout, "posted message number %s to group %s\n", status.c_str(), selected_GID.c_str());
            return "";
        }
    }
    fprintf(stdout, "Something went wrong\n");
    return "";
}

/*Retrieves up to 20 messages from the group selected by the user, starting from the one
also chosen by the user. Can also return a message to the user about the request status
returned by the server.*/
void retrieve(string remaining, TCPClient &tcp){
    string MID = remaining;
    string request;
    switch (MID.length()) {
        case 1:
            MID = "000" + MID;
            break;
        case 2:
            MID = "00" + MID;
            break;
        case 3:
            MID = "0" + MID;
            break;
        default:
            break;
    }
    if(selected_UID.compare("") == 0 || selected_GID.compare("") == 0 || MID.compare("") == 0){
        request = "RTV\n";
    }
    else{
        request = "RTV " + selected_UID + " " + selected_GID + " " + MID + "\n";
    }
    tcp.sendData(request.c_str(), request.length());
    
    string space, cmd, status, N, UID, Tsize, text, bar, Fname, Fsize;

    cmd.assign(tcp.getDataRetrieve(3));
	space.assign(tcp.getDataRetrieve(1));

    status.assign(tcp.getDataRetrieve(GID_LENGTH));
	space.assign(tcp.getDataRetrieve(1));
    if(space.compare(" ") != 0){
        status += space;
        space.assign(tcp.getDataRetrieve(1));
    }

    if(cmd.compare("RRT") == 0){
        if(status.compare("OK") == 0){

            N.assign(tcp.getDataRetrieve(1));
            space.assign(tcp.getDataRetrieve(1));
            if(space.compare(" ") != 0){
                N += space;
                space.assign(tcp.getDataRetrieve(1));
            }

            fprintf(stdout, "%s message(s) retrieved:\n", N.c_str());
            bool found_bar = true;
            for(int i = stoi(N); i > 0; i--){
                if(!found_bar){
                    MID.assign(tcp.getDataRetrieve(3));
                    MID = bar + MID;
                    space.assign(tcp.getDataRetrieve(1));
                }
                else{
                    MID.assign(tcp.getDataRetrieve(MID_LENGTH));
                    space.assign(tcp.getDataRetrieve(1));
                }
                

                UID.assign(tcp.getDataRetrieve(UID_LENGTH));
                space.assign(tcp.getDataRetrieve(1));

                Tsize.assign(tcp.getDataRetrieve(1));
                space.assign(tcp.getDataRetrieve(1));
                while(space.compare(" ") != 0){
                    Tsize += space;
                    space.assign(tcp.getDataRetrieve(1));
                }

                text.assign(tcp.getDataRetrieve(stoi(Tsize)), stoi(Tsize));
                fprintf(stdout, "%s - \"%s\";", MID.c_str(), text.c_str());

                space.assign(tcp.getDataRetrieve(1));
                if(space.compare("\n") == 0){
                    fprintf(stdout, "\n");
                    return;
                }

                bar.assign(tcp.getDataRetrieve(1));
                found_bar = true;
                if(bar.compare("/") != 0){
                    fprintf(stdout, "\n");
                    found_bar = false;
                    continue;
                }
                
                space.assign(tcp.getDataRetrieve(1));
                Fname.assign(tcp.getDataRetrieve(1));
                space.assign(tcp.getDataRetrieve(1));
                while(space.compare(" ") != 0){
                    Fname += space;
                    space.assign(tcp.getDataRetrieve(1));
                }

                Fsize.assign(tcp.getDataRetrieve(1));
                space.assign(tcp.getDataRetrieve(1));
                while(space.compare(" ") != 0){
                    Fsize += space;
                    space.assign(tcp.getDataRetrieve(1));
                }

                tcp.getFileData(Fname, stoi(Fsize));

                fprintf(stdout, " file stored: %s\n", Fname.c_str());

                space.assign(tcp.getDataRetrieve(1));
                if(space.compare("\n") == 0){
                    return;
                }
            }
            return;
        }
        else if(status.compare("EOF") == 0){
            fprintf(stdout, "There are no messages available\n");
            return;
        }
        else if(status.compare("NOK") == 0){
            fprintf(stdout, "Something went wrong. User could not retrieve any messages\n");
            return;
        }
    }
    fprintf(stdout, "Something went wrong\n");
    return;
}
