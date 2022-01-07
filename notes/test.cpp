#include <regex>
#include <iostream>

using namespace std;

int main() {
	string sp = "static vector<string> ClientUser_UDP = {";
	regex re("vector<(\\w+)> (\\w+)");

	// flag type for determining the matching behavior
	// && here it is for matches on strings.
	smatch match;

	// we can use member function on match
	// to extract the matched pattern.
	if (regex_search(sp, match, re) == true) {

		// The size() member function indicates the
		// number of capturing groups plus one for the overall match
		// match size = Number of capturing group + 1
		// (.*) which "forgeeks" ).
		cout << "Match size = " << match.size() << endl;

		// Capturing group is index from 0 to match_size -1
		// .....here 0 to 2
		// pattern at index 0 is the overall match "geeksforgeeks"
		// pattern at index 1 is the first capturing group "geeks"
		// pattern at index 2 is the 2nd capturing group "forgeeks"

		cout << "Whole match : " << match.str(0) << endl;
		cout << "First capturing group is '" << match.str(1)
			<< "' which is captured at index " << match.position(1)
			<< endl;
		cout << "Second capturing group is '" << match.str(2)
			<< "' which is captured at index " << match.position(2)
			<< endl;
	}
	else {
		cout << "No match is found" << endl;
	}
	return 0;
}
