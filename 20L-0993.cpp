#include <iostream>
#include <ctime>
#include<fstream>
#include<stdlib.h>

using namespace std;

inline void HomeScreen(char& );
void Run_Game(char&);
inline bool check_format(char* name);
char** input(char*, int&, int&);
char* input_word(ifstream&, ofstream&, int&, int&);
inline int StringLength(char*);
char** ReadWordsListFromFile(char*, int&, int&, int&);
char** GridIntialization(char**, int& , int&);
void PrintGrid(ofstream&, char**, int& ,int&);
inline int RandomSelect(int);
void PlaceWord(char**&, char**, int&, int&, int&);
bool CheckPlace(char** , char* , int& , int& , int&, int& , int& );
void PlaceWords(char**& , int& , int& );
bool search(char **, char*, int&, int& , int , int , int&, int& );
int WordMatch(char ** grid, char* , int& , int& , int& , int& , int& , int& );
void output(ofstream& , char * , bool , const int , const int , const int , const int );
inline void Deallocation(char**, int&);

void main()
{
	system("color 5f");
	
	char option;

	HomeScreen(option);
	Run_Game(option);

	system("pause");
}
//Displays Home Screen Menu
inline void HomeScreen(char & option)
{
	cout << endl << endl << "------------------------------- WORD SEARCH -------------------------------------" << endl << endl;
	cout << endl << "Press \n\tC to Create Grid.\n\tS to Search Words from Grid.\n\tQ to Quit Game." << endl;
	do
	{
		cin >> option;
		if (option != 'C' && option != 'S' && option != 'Q')
		{
			cout << endl << "Please select one option from given menu.";
			cout << endl << "Press \n\tC to Create Grid.\n\tS to Search Words from Grid.\n\tQ to Quit Game." << endl;
		}

	} while (option != 'C' && option != 'S' && option != 'Q');
}
//Starts the Game according to selected option
void Run_Game(char& option)
{
	if (option == 'C')
	{
		char input_file_name[20];
		char output_file_name[20];
		int no_of_words = 0, grid_rows = 0, grid_cols = 0;
		char** list = 0;
		char** grid = 0;
		int dim = 0;

		cout << endl << "To create grid please provide some required information" << endl << endl;
		do
		{
			cout << "\t1)Name of the Input File with .txt extension:\t";
			cin >> input_file_name;
		} while (check_format(input_file_name) == false);
		cout << endl << "\t2)Number of Words in input file:\t";
		cin >> no_of_words;
		cout << endl << "\t3)Number of Rows in grid:\t";
		cin >> grid_rows;
		cout << endl << "\t4)Number of Columns in grid:\t";
		cin >> grid_cols;
		do
		{
			cout << endl << "\t5)Name of the Output File with .txt extension:\t";
			cin >> output_file_name;
		} while (check_format(output_file_name) == false);

		cout << endl << endl << "\t\t\t\tPlease wait for a moment" << endl << endl;
		list = ReadWordsListFromFile(input_file_name, no_of_words, grid_rows, grid_cols);
		if (list)
		{
			ofstream output_file(output_file_name);
			grid = GridIntialization(list, grid_rows, grid_cols);
			PlaceWord(grid, list, no_of_words, grid_rows, grid_cols);
			PrintGrid(output_file, grid, grid_rows, grid_cols);
			cout << endl << "Congratulations! Grid Created Successfully in " << output_file_name << endl;
		}
		else
			cout << endl << endl << "Grid cannot be formed." << endl;

		Deallocation(list, no_of_words);
		Deallocation(grid, grid_rows);

		cout << endl << "Press any key to return to Home Screen " << endl;
		cin >> option;
		HomeScreen(option);
		Run_Game(option);
	}
	else if (option == 'S')
	{
		char input_grid_file_name[20];
		char input_words_file_name[20];
		char results_file_name[20];

		cout << endl << "To search words from grid please provide some required information" << endl << endl;
		do
		{
			cout << "\t1)Name of the Input File containing grid with .txt extension:\t";
			cin >> input_grid_file_name;
		} while (check_format(input_grid_file_name) == false);;
		do
		{
			cout << endl << "\t2)Name of the Input File containing words to search with .txt extension:\t";
			cin >> input_words_file_name;
		} while (check_format(input_words_file_name) == false);
		do
		{
			cout << endl << "\t3)Name of the Output File with .txt extension:\t";
			cin >> results_file_name;
		} while (check_format(results_file_name) == false);

		int rows = 0, cols = 0, test_cases = 0;
		ifstream words_file(input_words_file_name);
		char *word = 0;
		if (words_file)
		{
			words_file >> rows;
			words_file >> cols;
			char** grid = input(input_grid_file_name, rows, cols);

			words_file >> test_cases;
			ofstream output_file(results_file_name);
			for (int i = 1; i <= test_cases; i++)
			{
				PrintGrid(output_file, grid, rows, cols);
				int found = 0;
				word = input_word(words_file, output_file, rows, cols);
				int start_row = 0, end_row = 0, start_col = 0, end_col = 0;
				found = WordMatch(grid, word, rows, cols, start_row, start_col, end_row, end_col);
				output(output_file, word, found, start_row, start_col, end_row, end_col);
			}
			words_file.close();
			delete[]word;
			output_file.close();
			cout << endl << "Congratulations! Search Words Operation Completed Successfully. Results are saved in  " << results_file_name << endl;
			Deallocation(grid, rows);
			cout << endl << "Press any key to return to Home Screen " << endl;
			cin >> option;
			HomeScreen(option);
			Run_Game(option);
		}
		else
			cout << endl << "Unable to find the Input File containing words to search. " << endl;
	}
	else if (option == 'Q')
	{
		cout << endl << "Exiting from Word Search\n\t\t\t Unloading......... " << endl;
	}
}
//Check the extension of file
inline bool check_format(char * name)
{
	int length = StringLength(name) - 1;
	if (name[length] == 't' && name[length - 1] == 'x' && name[length - 2] == 't' && name[length - 3] == '.')
		return true;
	else
		return false;
}
//Returns length of string
inline int StringLength(char* str)
{
	int strLen = 0;
	int i = 0;

	while (str[i] != '\0')
	{
		strLen++;
		i++;
	}

	return strLen;
}
//Creates a grid and stores words in it by reading from file and returns that grid
char** ReadWordsListFromFile(char * name, int& rows, int& max_row, int& max_col)
{
	ifstream file(name);
	char** List_of_Words = new char*[rows];

	if (file)
	{
		char temp[80];
		int i = 0;
		while (i < rows)
		{
			file.getline(temp, 80);
			int cols = 0;
			cols = StringLength(temp);
			if (cols >= max_row && cols >= max_row)
				return NULL;

			*(List_of_Words + i) = new char[cols + 1];
			int j = 0;
			for (j = 0; j < cols; j++)
				*(*(List_of_Words + i) + j) = temp[j];
			List_of_Words[i][j] = '\0';
			i++;
		}
	}
	file.close();

	return List_of_Words;
}
//Initializes grid to _
char** GridIntialization(char** list, int& rows, int& cols)
{
	char** grid = new char*[rows];

	for (int i = 0; i < rows; i++)
		grid[i] = new char[rows + 1];

	for (int i = 0; i < rows; i++)
	{
		int j = 0;
		for (; j < cols; j++)
			grid[i][j] = '_';
		grid[i][j] = '\0';
	}
	return grid;
}
//Displays grid on output file and console
void PrintGrid(ofstream& out, char** grid, int& rows, int& cols)
{
	if (out)
	{
		out << endl << endl << "------------------------------- WORD SEARCH -------------------------------------" << endl << endl;
		cout << endl << endl << "------------------------------- WORD SEARCH -------------------------------------" << endl << endl;
		out << "   ";
		cout << "   ";

		for (int j = 0; j < cols; j++)
		{
			if (j < 10)
			{
				out << j << "   ";
				cout << j << "   ";
			}
			else
			{
				out << j << "  ";
				cout << j << "  ";
			}
		}
		cout << endl;
		out << endl;
		for (int i = 0; i < rows; i++)
		{
			out << i << " ";
			cout << i << " ";
			if (i < 10)
			{
				out << " ";
				cout << " ";
			}
			for (int j = 0; j < cols; j++)
			{
				out << grid[i][j] << "   ";
				cout << grid[i][j] << "   ";
			}
			out << endl;
			cout << endl;
		}
	}
}
//Randomly select alphabets
inline int RandomSelect(int number)
{
	int random;
	srand(time(0));
	random = 1 + rand() % number;
	return random;
}

void left_to_right(char**& Grid, char** list,int& wordno,int& row, int& col, int& length)
{
	for (int j = 0; j <= length; j++)	//Checks left to right
		Grid[row][col + j] = list[wordno][j];
}
void right_to_left(char**& Grid, char** list, int& wordno, int& row, int& col, int& length)
{
	for (int j = 0; j <= length; j++)	//Checks right to left
		Grid[row][col - j] = list[wordno][j];
}
void vertical_down(char**& Grid, char** list, int& wordno, int& row, int& col, int& length)
{
	for (int j = 0; j <= length; j++)	//Checks vertical down
		Grid[row + j][col] = list[wordno][j];
}
void vertical_up(char**& Grid, char** list, int& wordno, int& row, int& col, int& length)
{
	for (int j = 0; j <= length; j++)	//Checks vertical up
		Grid[row - j][col] = list[wordno][j];
}
void diagonal_up_right(char**& Grid, char** list, int& wordno, int& row, int& col, int& length)
{
	for (int j = 0; j <= length; j++)	//Checks diagonal up right
		Grid[row + j][row + j] = list[wordno][j];
}
void diagonal_up_left(char**& Grid, char** list, int& wordno, int& row, int& col, int& length)
{
	for (int j = 0; j <= length; j++)	//Checks diagonal up left
		Grid[row + j][row - j] = list[wordno][j];
}
void diagonal_down_right(char**& Grid, char** list, int& wordno, int& row, int& col, int& length)
{
	for (int j = 0; j <= length; j++)	//Checks diagonal down right
		Grid[row - j][row + j] = list[wordno][j];
}
void diagonal_down_left(char**& Grid, char** list, int& wordno, int& row, int& col, int& length)
{
	for (int j = 0; j <= length; j++)	//Checks diagonal down left
		Grid[row - j][row - j] = list[wordno][j];
}

//Writes words on grid by checking suitable place
void PlaceWord(char**& Grid, char** list, int& total_words, int& grid_row, int& grid_col)
{
	for (int wordNumber = total_words - 1; wordNumber >= 0; wordNumber--)
	{
		int row, col, place_word;
		int length = StringLength(list[wordNumber]);
		length = length - 1;
		bool check = false;

		col = total_words - length - 1;
		if (wordNumber == total_words - 1)
		{
			for (int j = 0; j <= length; j++)
				Grid[wordNumber - j][wordNumber - j] = list[wordNumber][j];
			wordNumber--;
			length = StringLength(list[wordNumber]);
			length = length - 1;
		}
		do
		{
			row = RandomSelect(grid_row);
			row = row - 1;

			place_word = RandomSelect(7);
			check = CheckPlace(Grid, list[wordNumber], row, col, place_word, grid_row, grid_col);

		} while (check == false);
		if (check == true)
		{
			switch (place_word)
			{
			case 1:
			{
				left_to_right(Grid, list, wordNumber, row, col, length);
				break;
			}
			case 2:
			{
				right_to_left(Grid, list, wordNumber, row, col, length);
				break;
			}
			case 3:
			{
				vertical_down(Grid, list, wordNumber, row, col, length);
				break;
			}
			case 4:
			{
				vertical_up(Grid, list, wordNumber, row, col, length);
				break;
			}
			case 5:
			{
				diagonal_up_right(Grid, list, wordNumber, row, col, length);
				break;
			}
			case 6:
			{
				diagonal_up_left(Grid, list, wordNumber, row, col, length);
				break;
			}
			case 7:
			{
				diagonal_down_right(Grid, list, wordNumber, row, col, length);
				break;
			}
			case 8:
			{
				diagonal_down_left(Grid, list, wordNumber, row, col, length);
				break;
			}
			}
		}
	}
	PlaceWords(Grid, grid_row, grid_col);
}
//Checks that if a word can place at particular location
bool CheckPlace(char** Grid, char* list, int& row, int& col, int& place, int& max_row, int& max_col)
{
	int length = StringLength(list);
	length = length - 1;

	bool check = true;
	switch (place)
	{
	case 1:
	{
		for (int n = col; n <= col + length && check == true; n++)
		{
			if (Grid[row][n] != '_' || col + length >= max_col)
				check = false;
		}
		break;
	}
	case 2:
	{
		for (int n = col; n >= col - length && check == true; n--)
		{
			if (Grid[row][n] != '_' || col - length <= 0)
				check = false;
		}
		break;
	}
	case 3:
	{
		for (int m = row; m < max_row && check == true; m++)
		{
			if (Grid[m][col] != '_' || row + length >= max_row)
				check = false;
		}
		break;
	}
	case 4:
	{
		for (int m = row; m >= row - length && m < max_row && check == true; m--)
		{
			if (Grid[m][col] != '_' || row - length <= 0)
				check = false;
		}
		break;
	}
	case 5:
	{
		if ((row + length) > max_row - 1 || (row - length) < 0)
		{
			check = false;
			return check;
		}

		for (int i = 0; i <= length && check == true; i++)
		{
			if (Grid[row + i][row + i] != '_')
				check = false;
		}
		break;
	}
	case 6:
	{
		if ((row + length) > max_row - 1 || (row - length) < 0)
		{
			check = false;
			return check;
		}
		for (int i = 0; i <= length && check == true; i++)
		{
			if (Grid[row + i][row - i] != '_')
				check = false;
		}
		break;
	}
	case 7:
	{
		if ((row + length) > max_row - 1 || (row - length) < 0)
		{
			check = false;
			return check;
		}
		for (int i = 0; i <= length && check == true; i++)
		{
			if (Grid[row - i][row + i] != '_')
				check = false;
		}
		break;
	}
	case 8:
	{
		if ((row - length) < 0)
		{
			check = false;
			return check;
		}
		for (int i = 0; i <= length && check == true && (row - length) > -1; i++)
		{
			if (Grid[row - i][row - i] != '_')
				check = false;
		}
		break;
	}
	}
	return check;
}
//Place words in grid sequentially
void PlaceWords(char**& grid, int& grid_row, int& grid_col)
{
	for (int i = 0; i < grid_row; i++)
	{
		for (int j = 0; j < grid_col; j++)
		{
			if (grid[i][j] == '_')
			{
				do
				{
					grid[i][j] = rand();
				} while (!(grid[i][j] >= 65 && grid[i][j] <= 90));
			}
		}
	}
}
//Inputs words to search from fiile containing words
char* input_word(ifstream& file, ofstream& out, int& rows, int& cols)
{
	int length = 0;
	char temp[20] = { 0 };

	if (out)
	{
		cout << endl << "Word :	";
		out << endl << "Word :	";
		file.getline(temp, 20);
		out << temp << endl;
		cout << temp << endl;
		length = StringLength(temp);
		if (length >= rows && length >= cols)
		{
			out << endl << "The length of word is not within boundary of grid." << endl;
			throw"Out of BOUND";
		}

		else
		{
			char* word = new char[length + 1];
			for (int i = 0; i < length; i++)
				*(word + i) = temp[i];
			*(word + length) = '\0';
			return word;
		}
	}
}
//Generates a word grid and inputs data from file
char** input(char* file_name, int& rows, int& cols)
{
	char** grid = 0;
	ifstream fin(file_name);

	if (fin.is_open())
	{
		grid = new char*[rows];
		for (int i = 0; i < rows; i++)
			grid[i] = new char[cols];
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				fin >> *(*(grid + i) + j);
		fin.close();
	}
	else
		cout << endl << "Error: Unable to open the Input File " << endl;
	return grid;
}
// Search word in all 8 directions
bool search(char **grid, char*  word,int& rows, int& cols, int i, int j, int &end_row, int& end_col)
{
	int len = StringLength(word);
	int x[] = { 0, 0, -1,  -1, -1,  1, 1, 1 };
	int y[] = { 1, -1,  0, -1, 1, -1, 0, 1 };
	// Search word in all 8 directions
	for (int dir = 0; dir < 8; dir++)
	{
		int k;
		end_row = i + x[dir], end_col = j + y[dir];
		for (k = 1; k < len; k++)
		{
			if (end_row >= rows || end_row < 0 || end_col >= cols || end_col < 0)
				break;
			if (grid[end_row][end_col] != word[k])
				break;
			end_row += x[dir], end_col += y[dir];	// Selecting direction to move
		}
		if (k == len)	// If all characters of the word matched, then value of k must be equal to length of word
		{
			switch (dir) // To remove last increment in ending coordinates
			{
			case 0:
			{
				end_col -= 1;
				break;
			}
			case 1:
			{
				end_col += 1;
				break;
			}
			case 2:
			{
				end_row += 1;
				break;
			}
			case 3:
			{
				end_row += 1;
				end_col += 1;
				break;
			}
			case 4:
			{
				end_row += 1;
				end_col -= 1;
				break;
			}
			case 5:
			{
				end_row -= 1;
				end_col += 1;
				break;
			}
			case 6:
			{
				end_row -= 1;
				break;
			}
			case 7:
			{
				end_row -= 1;
				end_col -= 1;
				break;
			}
			}
			return true;
		}

	}
	return false;
}
// Function to check if the word exists in the grid or not 
int WordMatch(char ** grid, char* word, int& _rows, int& _cols, int& start_r, int& start_c, int& end_r, int& end_c)
{
	int length = StringLength(word);
	int result = 0;

	for (int i = 0; i < _rows; i++)
	{
		for (int j = 0; j < _cols; j++)
		{
			if (grid[i][j] == word[0]) // If first letter matches, then check the remaining pattern 
			{
				start_r = i;
				start_c = j;
				result = search(grid, word, _rows, _cols, i, j, end_r, end_c);
				if (result)
					return result;
			}
		}
	}
	return 0;
}
//Prints results of word search with indices
void output(ofstream& out, char * word, bool found, const int start_r, const int start_c, const int end_r, const int end_c)
{
	if(out);
	if (found == 0)
	{
		out << word << "\tOops! Could not lookup the word." << endl << endl;
		cout << word << "\tOops! Could not lookup the word." << endl << endl;
	}
	else
	{
		out << word << "  Hurrah! Word Found " << endl;
		out << "The Starting Index is : " << "{ " << start_r << " , " << start_c << " }" << endl;
		out << "The Ending Index is : " << "{ " << end_r << " , " << end_c << " }" << endl << endl;

		cout << word << "  Hurrah! Word Found " << endl;
		cout << "The Starting Index is : " << "{ " << start_r << " , " << start_c << " }" << endl;
		cout << "The Ending Index is : " << "{ " << end_r << " , " << end_c << " }" << endl << endl;
	}
}
//Deallocates memory on heap
inline void Deallocation(char** array, int& rows)
{
	if (array != 0)
	{
		for (int i = 0; i < rows; i++)
			delete[]array[i];
		delete[] array;
	}
}