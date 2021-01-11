#include <iostream>
#include <string>
#include <fstream>

using namespace std;

string keywords [32] = { "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", 
"extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct",
"switch", "typedef", "union", "unsigned", "void", "volatile", "while"};

string operators [30] = {"=", "|", "||", "&&", "==", "!=", "<", ">", "<=", ">=", "+", "-", "*", "/", "%", "&", "!", "++", "--",
".", "->", "(", ")", "[", "]", "{", "}", ";", ":", ","};

char ignoreChars [5] = {'t', 'n', 'f', 'v', 'r'};

ofstream outputFile;

bool is_keyword_or_operator(string value, string *array, int length);
bool in_ignore(char c, char *array, int length);

void case_integers (string s);
void case_strings (string s);
void case_identifiers (string s);
void case_keywords (string s);
void case_operators (string s);

int main(int argc, char* argv[]) {
	string token;

	cout << "Input File: " << argv[1] << endl;
	cout << "Output File: " << argv[2] << endl;

	ifstream myfile (argv[1]);
	outputFile.open(argv[2]);

	while (myfile.is_open()) {
		int c = myfile.get();

		while(!myfile.eof()) {
			token.clear(); // Erases the contents of the string, which becomes an empty string (with a length of 0 characters).

			// ignore whitespace
			if (isspace(c)) {
				c = myfile.get();
			}

			// ignore ctrl characters
			else if (c == '\\' && in_ignore(c, ignoreChars, 5)) {
				c = myfile.get();
				c = myfile.get();
			}

			// comments																																																																																																																																																																																																																															
			else if (c == '/' && myfile.peek() == '*') {
				c = myfile.get();
				c = myfile.get();
				while(!(c == '*' && myfile.peek() == '/') && !myfile.eof()) {
					token += c;
					c = myfile.get();
				}

				c = myfile.get();
				c = myfile.get();
			}

			// if first character is a digit we can conclude the token will be an int
			else if (isdigit(c)) {
				while (isdigit(c) && !myfile.eof()){ 
					token += c;
					c = myfile.get();
				}
					
				case_integers(token);
			}

			// operators
			else if (is_keyword_or_operator(string(1, c), operators, 30)) {
				token += c;

				if (is_keyword_or_operator(token + string(1, myfile.peek()), operators, 30)) {
					c = myfile.get();
					token += c;

					case_operators(token);
				}
				else {
					case_operators(token);
				}
				c = myfile.get();
			}

			// if first character is a letter or an underscore, we can conclude the token will be a keyword or identifier
			else if (isalnum(c) || c == '_') {
				while ((isalnum(c) || c == '_') && !myfile.eof()) /* need to check for any terminating character for a identifier */ {
					token += c;
					if (is_keyword_or_operator(token, keywords, 32) && !isalnum(myfile.peek())) {
						case_keywords(token);
						c = myfile.get();
						break;
					}
					c = myfile.get();
				}
				
				if (!is_keyword_or_operator(token, keywords, 32)){
					case_identifiers(token);
				}
			}

			// quotes
			else if (c == '\"') {
				token += c;
				c = myfile.get();

				while (true && !myfile.eof()) {
					if (c == '\\' && myfile.peek() == '\"') {
						token += c;
						c = myfile.get();
						token += c;
						c = myfile.get();
					}
					else if (c == '\"'){
						token += c;
						c = myfile.get();
						break;
					}
					else {
						token += c;																																		
						c = myfile.get();
					}
				}

				case_strings(token);																																														
			}

			else {
				c = myfile.get();
			}
		}

		myfile.close();
		outputFile.close();
	}
}

bool is_keyword_or_operator(string value, string *array, int length){
	for (int i = 0; i < length; ++i) {
		if (array[i] == value) {
			return true;
		}
	}

	return false;
}

bool in_ignore(char c, char *array, int length) {
	for (int i = 0; i < length; ++i) {
		if (array[i] == c){
			return true;
		}
	}

	return false;
}

// deal with integers
void case_integers (string s) {
	outputFile << "int:" << s << endl;
} 

// deal with strings
void case_strings (string s) {
	outputFile << "string:" << s << endl;
	// cout << format("string:{}\n", s);
} 

// deal with identifiers
void case_identifiers (string s) {
	outputFile << "identifier:" << s << endl;
	// cout << format("identifier:{}\n", s);
} 

// deal with keywords
void case_keywords (string s) {
	outputFile << "keyword:" << s << endl;
	// cout << format("keyword:{}\n", s);
} 

// deal with operators
void case_operators (string s) {
	outputFile << "operator:" << s << endl;
	// cout << format("operator:{}\n", s);
} 
