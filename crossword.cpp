#include <cstdio>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cstdlib>


//Int to keep track of number of solutions
int solutions = 0;

//Vector of a vector of a vector of points, keeping track of the solution points for each word

std::vector <std::vector<std::vector<std::vector<int> > > > paths;

//Same thing but only for valid paths

std::vector <std::vector<std::vector<std::vector<int> > > > valid_paths;

//Find horizonal and vertical functions declaration

/*These are the arguments for each: Normal grid file, a vector to hold all the paths found
* a vector for each path, a word to search for, the letter of the word to search for,
* and x and y coordinate on the grid
*/

void findhorizontal( std::vector<std::string>& grid, std::vector<std::vector<std::vector<int> > >& wordCombo, 
	std::vector<std::vector<int> >& find, std::string& word, int let, int x, int y);
void findvertical( std::vector<std::string>& grid, std::vector<std::vector<std::vector<int> > >& wordCombo, 
	std::vector<std::vector<int> >& find, std::string& word, int let, int x, int y);

//Function to create all combinations

/*These are the arguments for each: Vector to add all path combinations to, vector of all the paths,
* vector to add more combos to, the constraints vector, int for how many paths left to equal constraint
* file size, int for path to start creating combos at
*/

void combo_creator(std::vector <std::vector<std::vector<std::vector<int> > > >& all_paths,
	std::vector<std::vector<std::vector<int> > >& wordCombo,
	std::vector<std::vector<std::vector<int> > >& combo,
	std::vector<int>& lengths, int left, int start);

//Function to check to see the valid combinations

/*These are the arguments for each: Normal grid file, a vector to hold all the combos found, 
* constraints vector and dictionary vector
*/

std::vector<std::vector<std::string> > check_combos(std::vector<std::string>& grid,
	std::vector <std::vector<std::vector<std::vector<int> > > >& all_paths,
	std::vector<int>& lengths);

//Functions to check the grid after horizontally and vertically for unwanted words

//These are the arguments for each: Vector of almost valid grids, and constraints vector and dictionary
//vector


std::vector<std::vector<std::string> > valid_grids(std::vector<std::vector<std::string> >& final_grids,
	std::vector<int>& lengths, std::vector<std::string>& dict);


int main(int argc, char* argv[]) {
	//Creating input streams, checking to see if input is correct
	if (argc != 5 && argc != 6) {
		std::cerr << "ERROR: Improper input, please retry" << std::endl;
		return 1;
	}

	std::ifstream dict(argv[1]);
	if (!dict) {
		std::cerr << "ERROR: Dictionary stream failed, try again" << std::endl;
		return 1;
	}

	std::ifstream gr_str(argv[2]);
	if (!gr_str) {
		std::cerr << "ERROR: Grid stream failed, try again" << std::endl;
		return 1;
	}
	std::string sol = argv[3]; //Solution type
	if (sol != "one_solution" && sol != "all_solutions") {
		std::cerr << "ERROR: Solution specification is invalid, try again" << std::endl;
		return 1;
	}

	std::string out = argv[4]; //Output type
	if (out != "count_only" && out != "print_boards") {
		std::cerr << "ERROR: Output specification is invalid, try again" << std::endl;
		return 1;
	}

	//Reading grid file into vectors

	std::vector<std::string> grid;
	std::vector<int> lengths;

	std::string in;

	//Checking each line for what it is (constraints, grid, comments)
	while (gr_str >> in) {
		if (in[0] == '!') {
			//Do nothing
		}
		else if (in[0] == '+') {
			std::string len;
			for (unsigned int i = 1; i < in.length(); i++) {
				len += in[i];
			}
			lengths.push_back(stoi(len));

		}
		else {
			grid.push_back(in);
		}
	}
	//Reading in dictionary file into vector
	std::vector<std::string> dictionary;

	std::string w;
	while (dict >> w) {
		//Checking to see if word is equal to constraints sizes

		for (unsigned int i = 0; i < lengths.size(); i++) {
			if (int(w.length()) == lengths[i]) {
				dictionary.push_back(w);
				break;
			}
		}
	}

	
	//Finding all the paths for each word in the valid dictionary
	std::vector<std::vector<std::vector<int> > > wordCombo = {};
	std::vector<std::vector<int> > find = {};
	for (unsigned int i = 0; i < dictionary.size(); i++) {
		std::string word = dictionary[i];

		find = {};
		findhorizontal(grid, wordCombo, find, word, 0, 0, 0);

		find = {};
		findvertical(grid, wordCombo, find, word, 0, 0, 0);

	}
	//Creating all the combos for the grid if there are enough paths
	std::vector <std::vector<std::vector<std::vector<int> > > > all_paths;
	if (wordCombo.size() < lengths.size()) {
		solutions = 0;
	}
	else {
		std::vector<std::vector<std::vector<int> > >   combo;
		int s = lengths.size();

		combo_creator(all_paths, wordCombo, combo, lengths, s, 0);

	}
	//Now checking each combination to see if it's valid
	std::vector<std::vector<std::string> > final_grids;
	//This checks for overlap and creates each grid
	final_grids = check_combos(grid, all_paths, lengths);

	std::vector<std::vector<std::string> > valid;
	//This checks for words that should not be in the grid, due to words next to each other
	valid = valid_grids(final_grids, lengths,dictionary);
	

	//Each respective output for the mode chosen by the user
	if (out == "count_only") {
		std::cout << "Number of Solution(s): " << solutions << std::endl;
	}
	else if (sol == "all_solutions") {
		std::cout << "Number of Solution(s): " << solutions << std::endl;
		//If solutions exists then print them this way
		if (solutions > 0) {
			for (unsigned int i = 0; i < valid.size(); i++) {
				std::cout << "Board:" << std::endl;
				for (unsigned int y = 0; y < valid[i].size(); y++) {
					for (unsigned int x = 0; x < valid[i][y].size(); x++) {
						std::cout << valid[i][y][x];
					}
					std::cout << std::endl;
				}
			}
		}
	}
	else {
		if (solutions > 0) {
			solutions = 1;
		}
		std::cout << "Number of Solution(s): " << solutions << std::endl;
		//If solutions exists then print them this way
		if (solutions > 0) {
			for (unsigned int i = 0; i < 1; i++) {
				std::cout << "Board:" << std::endl;
				for (unsigned int y = 0; y < valid[i].size(); y++) {
					for (unsigned int x = 0; x < valid[i][y].size(); x++) {
						std::cout << valid[i][y][x];
					}
					std::cout << std::endl;
				}
			}
		}
	}
	
}


//Function to search for word in grid horizontally
void findhorizontal( std::vector<std::string>& grid, std::vector<std::vector<std::vector<int> > >& wordCombo, std::vector<std::vector<int> >& find, std::string& word, int let, int x, int y) {
	int max_y = grid.size();
	int max_x = grid[0].size();

	/*
	* This functions uses recursion to find each of the different paths that a given word has in the 
	* grid. The load of the if else statements are to check for each corner case, depending on where
	* is being checked on the grid. Each statement is fairly self explanatory and just to sure proof that
	* there is no corner case unhandled.
	*/

	//Various if statements to check for all corner cases
	if (int(word.length()) > max_x) {
		return;
	}
	//These two if on the last spot in the grid
	if (x == max_x - 1 && y == max_y-1 && (let != int(word.length() - 1) || word[let] != grid[y][x])) {
		find = {};
		return;
	}
	else if (x == max_x - 1 && y == max_y-1 && let == int(word.length() - 1) && word[let] == grid[y][x]) {
		std::vector<int> point = { y, x };
		find.push_back(point);
		wordCombo.push_back(find);
		return;
	}
	//These next three if at the end of one line of the grid
	else if (x == max_x - 1 && let != int(word.length()-1)) {
		let = 0;
		x = 0;
		find = {};
		findhorizontal(grid, wordCombo, find, word, let, x, y + 1);
	}
	else if (x == max_x - 1 && let == int(word.length() - 1) && word[let] == grid[y][x]) {
		std::vector<int> point = { y, x };
		find.push_back(point);
		wordCombo.push_back(find);
		find = {};
		let = 0;
		x = 0;
		findhorizontal(grid, wordCombo, find, word, let, x, y + 1);
	}
	else if (x == max_x - 1 && let == int(word.length() - 1) && word[let] != grid[y][x]) {
		find = {};
		let = 0;
		x = 0;
		findhorizontal(grid, wordCombo, find, word, let, x, y + 1);
	}
	//Next four are for if not at the end of one line of the grid
	else if (x != max_x - 1 && let == int(word.length() - 1) && word[let] == grid[y][x]) {
		std::vector<int> point = { y, x };
		find.push_back(point);
		wordCombo.push_back(find);
		find = {};
		let = 0;
		findhorizontal(grid, wordCombo, find, word, let, x+1, y);
	}
	else if (word[let] == grid[y][x] && x != max_x-1 && let != int(word.length()-1)) {
		let++;
		std::vector<int> point = { y, x };
		find.push_back(point);
		findhorizontal(grid, wordCombo, find, word, let, x+1, y);
	}
	else if (word[let] == grid[y][x] && x != max_x - 1 && let == int(word.length() - 1)) {
		let = 0;
		std::vector<int> point = { y, x };
		find.push_back(point);
		wordCombo.push_back(find);
		find = {};
		findhorizontal(grid, wordCombo, find, word, let, x + 1, y);
	}
	else if (word[let] != grid[y][x] && x != max_x - 1) {
		let= 0;
		find = {};
		findhorizontal(grid, wordCombo, find, word, let, x + 1, y);
	}
	//One final check if at the end of a line in the grid
	else if (word[let] != grid[y][x] && x == max_x - 1) {
		let = 0;
		find = {};
		findhorizontal(grid, wordCombo, find, word, let, 0, y+1);
	}

}

//Logically the same as the check horizontal function except that this checks vertically
void findvertical( std::vector<std::string>& grid, std::vector<std::vector<std::vector<int> > >& wordCombo, std::vector<std::vector<int> >& find,  std::string& word, int let, int x, int y) {
	int max_y = grid.size();
	int max_x = grid[0].size();

	/*
	* This functions uses recursion to find each of the different paths that a given word has in the
	* grid. The load of the if else statements are to check for each corner case, depending on where
	* is being checked on the grid. Each statement is fairly self explanatory and just to sure proof that
	* there is no corner case unhandled.
	*/

	if (int(word.length()) > max_y) {
		return;
	}

	//These next two are for if at the final spot in the grid
	if (x == max_x - 1 && y == max_y - 1 && (let != int(word.length() - 1) || word[let] != grid[y][x])) {
		find = {};
		return;
	}
	else if (x == max_x - 1 && y == max_y - 1 && let == int(word.length() - 1) && word[let] == grid[y][x]) {
		std::vector<int> point = { y, x };
		find.push_back(point);
		wordCombo.push_back(find);
		return;
	}
	//Checks for if the spot is not at the bottom of the grid
	else if (y != max_y - 1) {
		if (let == int(word.length() - 1) && word[let] == grid[y][x]) {
			std::vector<int> point = { y, x };
			find.push_back(point);
			wordCombo.push_back(find);
			let = 0;
			find = {};
		}
		else if (let == int(word.length() - 1) && word[let] != grid[y][x]) {
			find = {};
			let = 0;
		}
		else if (let != int(word.length() - 1) && word[let] == grid[y][x]) {
			std::vector<int> point = { y, x };
			find.push_back(point);
			let++;

		}
		else if (let != int(word.length() - 1) && word[let] != grid[y][x]) {
			find = {};
			let = 0;
		}
		findvertical(grid, wordCombo, find, word, let, x, y + 1);
	}
	//Checks for if the spot is at the bottom of the grid
	else if (y == max_y - 1) {
		if (let == int(word.length() - 1) && word[let] == grid[y][x]) {
			std::vector<int> point = { y, x };
			find.push_back(point);
			wordCombo.push_back(find);
			let = 0;
			find = {};
		}
		else {
			let = 0;
			find = {};
		}
		findvertical(grid, wordCombo, find, word, let, x + 1, 0);
	}

}


//Function to create all the combinations
void combo_creator(std::vector <std::vector<std::vector<std::vector<int> > > >& all_paths,
	std::vector<std::vector<std::vector<int> > >& wordCombo,
	std::vector<std::vector<std::vector<int> > >& combo,
	std::vector<int>& lengths, int left, int start) {
	std::vector<int> cons = lengths;
	//Length of all the paths
	int size = wordCombo.size();
	bool valid = true;
	//If combo is the size of the constraints file
	if (left < 1) {
		//Here checking to see if combo has all the same constraints as the constraints file
		std::sort(lengths.begin(), lengths.end());
		std::vector<int> len;
			
		all_paths.push_back(combo);//If it does then add it to a vector of all paths

		
		return;
	}

	//Making every unique combination
	for (int i = start; i <= size - left; i++) {
		valid = true;
		//Adding one path onto the previous path, then creating all unique combos for that new path
		std::vector<int>::iterator it;
				combo.push_back(wordCombo[i]);
				it = std::find(cons.begin(), cons.end(), wordCombo[i].size());
				

				if (it != cons.end()) {
					cons.erase(it);
					combo_creator(all_paths, wordCombo, combo, cons, left - 1, i + 1);
				}
				//Taking out the path and adding a different one next time to loop goes around
				combo.pop_back();
				cons = lengths;
	}

}

//Functions to check the combinations
std::vector<std::vector<std::string> > check_combos(std::vector<std::string>& grid, 
	std::vector <std::vector<std::vector<std::vector<int> > > >& all_paths,
	std::vector<int>& lengths) {
	//Valid grids to store all the grids with the right constraints
	std::vector<std::vector<std::string > > valid_grids;
	//Counter to count how many same points in two paths
	int counter;
	//Constraints
	std::vector<int> len;
	//Vectors storing paths after each check
	std::vector <std::vector<std::vector<std::vector<int> > > > new_paths;
	std::vector <std::vector<std::vector<std::vector<int> > > > right_paths;
	std::sort(lengths.begin(), lengths.end());
	new_paths = all_paths;
	

	//Checking each path group to see if they have too many repeat points
	for (unsigned int i = 0; i < new_paths.size(); i++) {
		counter = 0;
		bool valid;
		//Looping through words after first word
		for (unsigned int j = 0; j < new_paths[i].size() - 1; j++) {
			counter = 0;
			valid = true;
			//Looping through all of the first paths points
			for (unsigned int n = j + 1; n < new_paths[i].size(); n++) {
				for (unsigned int p = 0; p < new_paths[i][j].size(); p++) {
					//If point is found in second path, add one to the counter
					if (std::find(new_paths[i][n].begin(), new_paths[i][n].end(),
						new_paths[i][j][p]) != new_paths[i][n].end()) {
						counter++;
					}
				}
				//If counter is greater than 1, then don't add the path to the valid paths
				if (counter > 1) {
					valid = false;
				}
			}

		}
		if (valid) {
			right_paths.push_back(new_paths[i]);
		}
	}
	std::vector<std::string> new_grid;
	std::vector<std::string> grid_check;
	//For each path group in the right paths vector, creating a grid for it
		for(unsigned int i = 0; i < right_paths.size(); i++){
			std::vector<std::string> new_grid = grid;
			std::vector<std::string> grid_check = grid;
			//Marking all paths with 1's on them
		for (unsigned int j = 0; j < right_paths[i].size(); j++) {
			for (unsigned int g = 0; g < right_paths[i][j].size(); g++) {
				int y = right_paths[i][j][g][0];
				int x = right_paths[i][j][g][1];
				grid_check[y][x] = '1';
			}
		}
		//Creating valid path by putting a # wherever there isn't a 1
		//More efficient than checking if each point was in the path
		for (unsigned int t = 0; t < grid_check.size(); t++) {
			for (unsigned int h = 0; h < grid_check[t].size(); h++) {
				if (grid_check[t][h] != '1') {
					new_grid[t][h] = '#';
				}
			}

		}
		
		valid_grids.push_back(new_grid);
	}

	return valid_grids;
}

//Checking to see if extra words are in the grid by mistake
std::vector<std::vector<std::string> > valid_grids(std::vector<std::vector<std::string> >& final_grids,
	std::vector<int>& lengths, std::vector<std::string>& dict) {

	std::vector<std::vector<std::string> > valid_grids;
	std::vector<std::string> words;

	std::string word1;
	std::string word2;
	bool valid;

	//Checks all of the words that exist horizontally
	for (unsigned int i = 0; i < final_grids.size(); i++) {
		words = {};
		valid = true;
		for (unsigned int y = 0; y < final_grids[i].size(); y++) {
			for (unsigned int x = 0; x < final_grids[i][y].size(); x++) {
				//Adding letter to word if its not a #
				if (final_grids[i][y][x] != '#') {
					word1 += final_grids[i][y][x];
				}
				//If there is a # or word is at the end of a line, if it's over a letter, add it to vector
				else if (final_grids[i][y][x] == '#') {
					if (word1.length() > 1) {
						words.push_back(word1);
						
					}
					word1 = "";
				}
				if (x == final_grids[i][y].size() - 1) {
					if (word1.length() > 1) {
						words.push_back(word1);

					}
					word1 = "";
				}
			}
		}
		unsigned int width = final_grids[i][0].size();

		//Same thing here, just checks for vertical words instead of horizontal
		for (unsigned int x = 0; x < width; x++) {
			for (unsigned int y = 0; y < final_grids[i].size(); y++) {
				//Adding letter to word if its not a #

				if (final_grids[i][y][x] != '#') {
					word2 += final_grids[i][y][x];
				}
				//If there is a # or word is at the end of a line, if it's over a letter, add it to vector

				else if (final_grids[i][y][x] == '#') {
					if (word2.length() > 1) {
						words.push_back(word2);

					}
					word2 = "";
				}
				if (y == final_grids[i].size() - 1) {
					if (word2.length() > 1) {
						words.push_back(word2);

					}
					word2 = "";
				}
			}
		}
		//If the amount of words found is more than the words in the constraints it is not valid
		for (unsigned int i = 0; i < words.size(); i++) {
			if (std::find(dict.begin(), dict.end(), words[i]) == dict.end()) {
				valid = false;
			}
		}
		
		if (words.size() == lengths.size() && valid) {
			valid_grids.push_back(final_grids[i]);
		}
	}
	//Setting global solutions integer equal to this final, valid grid size
	solutions = valid_grids.size();
	return valid_grids;

}