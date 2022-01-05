#include "../Constants.hpp"

using namespace std;

string current_UID, selected_GID;

string showuid(){
    return "Your UID is " + current_UID + "\n";
}

string showgid(){
    return "Group " + selected_GID + " - is the active group\n"; //TODO: por o nome do grupo?
}

string select_GID(string GID){
    selected_GID = GID;
    return "Group " + GID + " - is now the active group\n"; //TODO: por o nome do grupo?
}

string rrg(string command){
    return "Placeholder\n";
}

string run(string command){
    return "Placeholder\n";
}

string rlo(string command){
    return "Placeholder\n";
}

string rou(string command){
    return "Placeholder\n";
}

string rgl(string command){
    return "Placeholder\n";
}

string rgs(string command){
    return "Placeholder\n";
}

string rgu(string command){
    return "Placeholder\n";
}

string rgm(string command){
    return "Placeholder\n";
}

string rul(string command){
    return "Placeholder\n";
}

string rpt(string command){
    return "Placeholder\n";
}

string rrt(string command){
    return "Placeholder\n";
}
