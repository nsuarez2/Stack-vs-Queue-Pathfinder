#include "main.h"

using namespace std;


int main(int argc, char **argv) {
    std::ios_base::sync_with_stdio(false);
    
    bool ringFound = 0, listOut = 0;
    string in, trashLine, inMode;
    int roomSize, numRooms, i, stack = 0, queue = 0;
    ostringstream os;
    deque<coord> deck;
    coord here, next;
    char portals[11] = {"abcdfghijk"};
    listCoord lHere;
    deque<listCoord> coords;
    char trash;
    
    // FOR XCODE TESTING
    ifstream arq(getenv("test6"));
    cin.rdbuf(arq.rdbuf());
    
	int opt = 0, index = 0;
	while((opt = getopt_long (argc, argv, "sqo:h", longOpts, &index)) != -1) {
		switch(opt) {
			case 's':
                stack++;
				break;
            case 'q':
                queue++;
				break;
			case 'o':
                if (*optarg == 'L') {
                    listOut = 1;
                }
				break;
            case 'h':
                cout << "Choose either stack or queue scheme by adding the flag"
                     << "s '-s' or '-q'.\nTo choose the output mode, add the fl"
                     << "ag '-o' followed by 'M' or 'L' for map or list."
                     << endl;
				exit(0);
			case '?':
				cerr << "I didn't recognize one of your flags" << endl;
				exit(1);
		}
	}
    // Check that only one flag is flagged
    if ((stack + queue) != 1) {
        cerr << "You must have exactly one stack or queue input flag" << endl;
        exit(1);
    }
    
    cin >> inMode >> roomSize >> numRooms;
    
    os << roomSize << "\n" << numRooms;
    getline(cin, in);
    
    vector<vector<char> > map(numRooms, vector<char>(roomSize*roomSize, '.'));
    vector<vector<char> > mapCopy(numRooms, vector<char>(roomSize*roomSize,
                                                         '.'));
    
    //----------------------------MAP INPUT---------------------------------
    if (inMode == "M") {
        for (int r = 0; r < numRooms; r++) {
            i = 0;
            while (i < roomSize) {
                getline(cin, in);
                if (notComment(in)) {
                    for (int j = 0; j < roomSize; j++) {
                        if (validInput(in[j])) {
                            map[r][roomSize * i + j] = in[j];
                            mapCopy[r][roomSize * i + j] = in[j];
                            if (in[j] == 'S') {
                                here.room = r;
                                here.spot = (roomSize * i) + j;
                                if (stack) deck.push_front(here);
                                else deck.push_back(here);
                                mapCopy[r][roomSize * i + j] = '$';
                            }
                        }
                    }
                    i++;
                }
            }
        }
    }
    //---------------------------LIST INPUT---------------------------------
    else {
        while (cin) {
            cin >> trash;
            if (trash == '/') {
                cin >> trash;
                if (trash == '/') {
                    // do nothing and trash the rest
                    getline(cin, trashLine);
                }
                else {
                    cerr << "'/' is not a valid input character" << endl;
                    exit(1);
                }
            }
            else {
                cin >> lHere.row >> trash >> lHere.col >> trash >> lHere.room
                                        >> trash >> lHere.character >> trash;
                
                if (validListCoord(&lHere, roomSize, numRooms)) {
                    mapCopy[lHere.room][(roomSize * lHere.row) + lHere.col] =
                                                                lHere.character;
                    map[lHere.room][(roomSize * lHere.row) + lHere.col] =
                                                                lHere.character;
                    if (lHere.character == 'S') {
                        here.room = lHere.room;
                        here.spot = (roomSize * lHere.row) + lHere.col;
                        if (stack) deck.push_front(here);
                        else deck.push_back(here);
                        mapCopy[here.room][here.spot] = '$';
                    }
                }
            }
        }
    }
    
    
    do {
        here = deck.front();
        deck.pop_front();
        
        //---------------------------NORTH---------------------------------
        if ((here.spot >= roomSize) &&
            neverSeen(mapCopy[here.room][here.spot - roomSize]) &&
            !(ringFound)) {
            
            switch (neighbor(map[here.room][here.spot - roomSize])) {
                // Neighbor is a wall
                case 0:
                    break;
                // Neighbor is a dot
                case 1:
                    next.room = here.room;
                    next.spot = here.spot - roomSize;
                    if (stack) deck.push_front(next);
                    else deck.push_back(next);
                    mapCopy[next.room][next.spot] = 'n';
                    break;
                // Neighbor is a portal
                case 2:
                    next.room = here.room;
                    next.spot = here.spot - roomSize;
                    if (stack) deck.push_front(next);
                    else deck.push_back(next);
                    mapCopy[next.room][next.spot] = 'n';
                    break;
                // Neighbor is the ring
                case 3:
                    ringFound = 1;
                    map[here.room][here.spot] = 'n';
                    lHere.room = here.room;
                    lHere.row = here.spot / roomSize;
                    lHere.col = here.spot % roomSize;
                    lHere.character = 'n';
                    coords.push_back(lHere);
                    break;
                // This should never happen
                case 4:
                    cerr << "Unrecoginzed character on the map somehow?"
                    << endl;
                    exit(1);
            }
        }
        //----------------------------EAST---------------------------------
        if (((here.spot + 1) % roomSize) &&
            neverSeen(mapCopy[here.room][here.spot + 1]) &&
            !(ringFound)) {
            
            switch (neighbor(map[here.room][here.spot + 1])) {
                // Neighbor is a wall
                case 0:
                    break;
                // Neighbor is a dot
                case 1:
                    next.room = here.room;
                    next.spot = here.spot + 1;
                    if (stack) deck.push_front(next);
                    else deck.push_back(next);
                    mapCopy[next.room][next.spot] = 'e';
                    break;
                // Neighbor is a portal
                case 2:
                    next.room = here.room;
                    next.spot = here.spot + 1;
                    if (stack) deck.push_front(next);
                    else deck.push_back(next);
                    mapCopy[next.room][next.spot] = 'e';
                    break;
                // Neighbor is the ring
                case 3:
                    ringFound = 1;
                    map[here.room][here.spot] = 'e';
                    lHere.room = here.room;
                    lHere.row = here.spot / roomSize;
                    lHere.col = here.spot % roomSize;
                    lHere.character = 'e';
                    coords.push_back(lHere);
                    break;
                // This should never ever happen
                case 4:
                    cerr << "Unrecoginzed character on the map somehow?"
                    << endl;
                    exit(1);
            }
        }
        //---------------------------SOUTH---------------------------------
        if (here.spot < (roomSize * (roomSize - 1)) &&
            neverSeen(mapCopy[here.room][here.spot + roomSize]) &&
            !(ringFound)) {
            
            switch (neighbor(map[here.room][here.spot + roomSize])) {
                // Neighbor is a wall
                case 0:
                    break;
                // Neighbor is a dot
                case 1:
                    next.room = here.room;
                    next.spot = here.spot + roomSize;
                    if (stack) deck.push_front(next);
                    else deck.push_back(next);
                    mapCopy[next.room][next.spot] = 's';
                    break;
                // Neighbor is a portal
                case 2:
                    next.room = here.room;
                    next.spot = here.spot + roomSize;
                    if (stack) deck.push_front(next);
                    else deck.push_back(next);
                    mapCopy[next.room][next.spot] = 's';
                    break;
                // Neighbor is the ring
                case 3:
                    ringFound = 1;
                    map[here.room][here.spot] = 's';
                    lHere.room = here.room;
                    lHere.row = here.spot / roomSize;
                    lHere.col = here.spot % roomSize;
                    lHere.character = 's';
                    coords.push_back(lHere);
                    break;
                // This should never happen
                case 4:
                    cerr << "Unrecoginzed character on the map somehow?"
                    << endl;
                    exit(1);
            }
        }
        //----------------------------WEST---------------------------------
        if ((here.spot % roomSize) &&
            neverSeen(mapCopy[here.room][here.spot - 1]) &&
            !(ringFound)) {
            
            switch (neighbor(map[here.room][here.spot - 1])) {
                // Neighbor is a wall
                case 0:
                    break;
                // Neighbor is a dot
                case 1:
                    next.room = here.room;
                    next.spot = here.spot - 1;
                    if (stack) deck.push_front(next);
                    else deck.push_back(next);
                    mapCopy[next.room][next.spot] = 'w';
                    break;
                // Neighbor is a portal
                case 2:
                    next.room = here.room;
                    next.spot = here.spot - 1;
                    if (stack) deck.push_front(next);
                    else deck.push_back(next);
                    mapCopy[next.room][next.spot] = 'w';
                    break;
                // Neighbor is the ring
                case 3:
                    ringFound = 1;
                    map[here.room][here.spot] = 'w';
                    lHere.room = here.room;
                    lHere.row = here.spot / roomSize;
                    lHere.col = here.spot % roomSize;
                    lHere.character = 'w';
                    coords.push_back(lHere);
                    break;
                // This should never happen
                case 4:
                    cerr << "Unrecoginzed character on the map somehow?"
                    << endl;
                    exit(1);
            }
        }
        if (isPortal(map[here.room][here.spot])) {
            // make sure the portal leads to a valid spot
            if ((map[here.room][here.spot]-'0') < numRooms) {
                next.room = map[here.room][here.spot] - '0';
                next.spot = here.spot;
                
                // safe to add
                if ((mapCopy[next.room][next.spot] == '.') ||
                    (((mapCopy[next.room][next.spot] - '0') < 10) &&
                     ((mapCopy[next.room][next.spot] - '0') >= 0))) {
                    mapCopy[next.room][next.spot] = portals[here.room];
                    if (stack) deck.push_front(next);
                    else deck.push_back(next);
                }
                // not safe to add
                else if (!(neverSeen(mapCopy[next.room][next.spot]))) {
                    // do nothing
                }
                else if (mapCopy[next.room][next.spot] == 'R') {
                    ringFound = 1;
                    map[here.room][here.spot] = 'p';
                    lHere.room = here.room;
                    lHere.row = here.spot / roomSize;
                    lHere.col = here.spot % roomSize;
                    lHere.character = 'p';
                    coords.push_back(lHere);
                }
            }
        }
        
    } while ((!deck.empty()) && (!ringFound));
    
    if (ringFound) {
        do {
            
            switch (pathFinder(mapCopy[here.room][here.spot])) {
                // Go north next
                case 10:
                    next.room = here.room;
                    next.spot = here.spot - roomSize;
                    map[next.room][next.spot] = 's';
                    if (listOut) {
                        lHere.room = next.room;
                        lHere.row = next.spot / roomSize;
                        lHere.col = next.spot % roomSize;
                        lHere.character = 's';
                        coords.push_front(lHere);
                    }
                    break;
                // Go east next
                case 11:
                    next.room = here.room;
                    next.spot = here.spot + 1;
                    map[next.room][next.spot] = 'w';
                    if (listOut) {
                        lHere.room = next.room;
                        lHere.row = next.spot / roomSize;
                        lHere.col = next.spot % roomSize;
                        lHere.character = 'w';
                        coords.push_front(lHere);
                    }
                    break;
                // Go south next
                case 12:
                    next.room = here.room;
                    next.spot = here.spot + roomSize;
                    map[next.room][next.spot] = 'n';
                    if (listOut) {
                        lHere.room = next.room;
                        lHere.row = next.spot / roomSize;
                        lHere.col = next.spot % roomSize;
                        lHere.character = 'n';
                        coords.push_front(lHere);
                    }
                    break;
                // Go west next
                case 13:
                    next.room = here.room;
                    next.spot = here.spot - 1;
                    map[next.room][next.spot] = 'e';
                    if (listOut) {
                        lHere.room = next.room;
                        lHere.row = next.spot / roomSize;
                        lHere.col = next.spot % roomSize;
                        lHere.character = 'e';
                        coords.push_front(lHere);
                    }
                    break;
                // You're done backtracking
                case 14:
                    break;
                // Go through a portal
                default:
                    next.room = pathFinder(mapCopy[here.room][here.spot]);
                    next.spot = here.spot;
                    map[next.room][next.spot] = 'p';
                    if (listOut) {
                        lHere.room = next.room;
                        lHere.row = next.spot / roomSize;
                        lHere.col = next.spot % roomSize;
                        lHere.character = 'p';
                        coords.push_front(lHere);
                    }
                    break;
            }
            
            here.room = next.room;
            here.spot = next.spot;
            
            
        } while (mapCopy[here.room][here.spot] != '$');
    }
    
    // Output for list mode
    if (listOut) {
        os << "\n//path taken\n";
        if (ringFound) {
            while (!(coords.empty())) {
                lHere = coords.front();
                os << "(" << lHere.row << "," << lHere.col << "," << lHere.room
                                            << "," << lHere.character << ")\n";
                coords.pop_front();
            }
        }
    }
    // Output for map mode
    else {
        for (int r = 0; r < numRooms; r++) {
            os << "\n//room " << r << "\n";
            for (int i = 0; i < roomSize * roomSize; i++) {
                if (!(i%roomSize) && i != 0) {
                    os << "\n";
                }
                os << map[r][i];
            }
        }
        os << "\n";
    }
    
    // flush it
    cout << os.str();
	return 0;
}

bool notComment(string in) {
    if (in[0] == '/') {
        if (in[1] == '/') {
            return false;
        }
        cerr << "'/' is not a valid input character" << endl;
        exit(1);
    }
    else return true;
}

bool validInput(char in) {
    if (in == '.') return true;
    else if (in == '#') return true;
    else if (in == 'S') return true;
    else if (in == 'R') return true;
    else if (((in - '0') < 10) && ((in - '0') >= 0)) return true;
    else if (in == '\0') return false;
    else {
        cerr << "Invalid input character" << endl;
        exit(1);
    }
}

bool validListCoord(listCoord* lHere, int sizeRooms, int numRooms) {
    
    if ((lHere->row >= 0) && (lHere->row < sizeRooms)){
        if ((lHere->col >= 0) && (lHere->col < sizeRooms)){
            if ((lHere->room >= 0) && (lHere->room < numRooms)){
                if (validInput(lHere->character)){
                    return true;
                }
            }
        }
    }
    cerr << "Invalid input somewhere" << endl;
    exit(1);
}

int neighbor(char spot) {
    if (spot == '.') return 1;
    else if (spot == '#') return 0;
    else if (spot == 'R') return 3;
    else if (spot == 'S') return 0;
    else if (((spot - '0') < 10) && ((spot - '0') >= 0)) return 2;
    else return 4;
}

bool neverSeen(char spot) {
    if (spot == '.') return true;
    else if (spot == 'R') return true;
    else if (spot == 'n' ||
             spot == 's' ||
             spot == 'e' ||
             spot == 'w' ||
             spot == 'p' ||
             spot == '$' ||
             spot == 'a' ||
             spot == 'b' ||
             spot == 'c' ||
             spot == 'd' ||
             spot == 'f' ||
             spot == 'g' ||
             spot == 'h' ||
             spot == 'i' ||
             spot == 'j' ||
             spot == 'k' ||
             spot == '#' ||
             spot == 'S'   ) return false;
    else if (((spot - '0') < 10) && ((spot - '0') >= 0)) return true;
    else return false;
}

bool isPortal(char spot) {
    if (((spot - '0') >= 0) && ((spot - '0') < 10)) return true;
    else return false;
}

int pathFinder(char dir) {
    if (dir == 's') return 10;
    if (dir == 'w') return 11;
    if (dir == 'n') return 12;
    if (dir == 'e') return 13;
    if (dir == '$') return 14;
    if (dir == 'a') return 0;
    if (dir == 'b') return 1;
    if (dir == 'c') return 2;
    if (dir == 'd') return 3;
    if (dir == 'f') return 4;
    if (dir == 'g') return 5;
    if (dir == 'h') return 6;
    if (dir == 'i') return 7;
    if (dir == 'j') return 8;
    if (dir == 'k') return 9;
    else {
        cerr << "This should never happen" << endl;
        exit(1);
    }
}




