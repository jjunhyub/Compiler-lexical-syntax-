#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

char c[10001];
bool isReject = false;
string errorString = "";
int sIndex = 0, eIndex = 0;
string mainString = "";
string tokenName = "", tokenValue = "";					//always keep last token's name and value
int line = 0;


//Token recognizer & other functions
bool isWhitespace(string buffer);
bool isID(string buffer);
bool isKeyword(string buffer);
bool isType(string buffer);
bool isArith(string buffer);
bool isAssign(string buffer);
bool isPunc(string buffer);
bool isComma(string buffer);
bool isSemi(string buffer);
bool isNumber(string buffer);
bool isString(string buffer);
bool isDigit(int index);
bool isNZDigit(int index);
bool isLetter(int index);
bool isComapare(string buffer);
bool isFinalState(string buffer);
bool isUndifined(string buffer);

string output = "";																			//you can change your filename extension.
int  main(int argc, char** argv) {
	ifstream inputFile(argv[1]);															//file read
	string tempInput = argv[1];
	for (int i = 0; tempInput[i + 2]; i++)output += string(1, tempInput[i]);
	output += ".out";
	ofstream out(output);
	while (!inputFile.eof()) {
		line++;
		inputFile.getline(c, 2000);
		mainString = "";
		mainString += c;
		mainString += " \n ";
		string lastS = "";
		int mainStringSize = mainString.size();
		for (sIndex = 0; sIndex < mainStringSize; sIndex++) {
			for (eIndex = sIndex; mainString[eIndex]; eIndex++) {
				string temp = "";
				for (int i = sIndex; i <= eIndex; i++)temp += mainString[i];				//make tempstring sIndex ~ eIndex
				if (isWhitespace(temp))break;												//pass the whitespace
				if (!isUndifined(temp))														//if it start with undefined character it shows error
				{
					string s1 = "ERROR line " + to_string(line) + " : unexpected input ->" + temp + "\n";
					errorString += s1;
					isReject = true;
					break;
				}
				else if (isString(temp))	lastS = temp;
				else if (isComma(temp))	    lastS = temp;
				else if (isAssign(temp))	lastS = temp;
				else if (isComapare(temp))	lastS = temp;
				else if (isArith(temp))		lastS = temp;
				else if (isPunc(temp))		lastS = temp;
				else if (isType(temp))		lastS = temp;
				else if (isSemi(temp))		lastS = temp;
				else if (isKeyword(temp))	lastS = temp;									//KEYWORD has higher priority than ID
				else if (isID(temp))		lastS = temp;
				else if (isNumber(temp))	lastS = temp;
				else
				{
					if (lastS != "")if (isFinalState(lastS))
					{																    	//if there is no other satisfying token call the latest token if it ends at the final state ,else shows error
						//Write down tokens																								
						cout << tokenName << " : " << tokenValue << "\n";
						string temps = tokenName + " : " + tokenValue + "\n";
						tokenName = "";
						tokenValue = "";
						if (out.is_open()) {
							out << temps;
						}
					}
					lastS = "";
					sIndex = eIndex - 1;
					break;
				}
			}
		}
	}
	out.close();
	if (isReject)
	{
		cout << "we are going to reject this file for these reason : \n";
		cout << errorString << "\n";
		int isremoved = remove(output.c_str());
		if (!isremoved)cout << "Sucessfully file deleted\n";
		else cout << "Error occured during deleting file\n";
		cout << "Syntax Analyzer is not going to run\n";
	}
	else
	{
		cout << "Lexical Analyzer is sucessfully ended\n";
		cout << "Now Syntax Analyzer is going to run\n";
	}
	system("pause");
	return 0;
}
bool isUndifined(string buffer)
{
	if (isDigit(sIndex) || isLetter(sIndex) || isWhitespace(buffer))return true;
	if (buffer[0] == ')' || buffer[0] == '(' || buffer[0] == '{' || buffer[0] == '}' || buffer[0] == '+' || buffer[0] == '-' || buffer[0] == '*' || buffer[0] == '/' || buffer[0] == ';' || buffer[0] == '\"' || buffer[0] == '\'' || buffer[0] == '!' || buffer[0] == '>' || buffer[0] == '<' || buffer[0] == '=' || buffer[0] == ',')return true;
	return false;
}
bool isDigit(int index) {
	if (48 <= mainString[index] && mainString[index] <= 57)return true;								//0~9
	return false;
}

bool isNZDigit(int index) {
	if (49 <= mainString[index] && mainString[index] <= 57)return true;								//1~9
	return false;
}

bool isLetter(int index) {
	if (65 <= mainString[index] && mainString[index] <= 90)return true;								//UpperCase Alphabet
	if (97 <= mainString[index] && mainString[index] <= 122)return true;							//LowerCase Alphabet
	return false;
}

bool isWhitespace(string buffer) {
	if (buffer == "\n" || buffer == "\t" || buffer == " ")return true;								//Tab, Linefeed, Blank
	return false;
}

bool isID(string buffer) {
	if (isLetter(sIndex) == false)return false;
	for (int i = sIndex + 1; i <= eIndex; i++) if (!isLetter(i) && !isDigit(i))return false;
	tokenName = "ID";
	tokenValue = buffer;
	return true;
}

bool isKeyword(string buffer) {
	string keywords[10] = { "IF","if", "ELSE", "else", "WHILE", "while", "RETURN", "return" };		//KEYWORD check
	for (int i = 0; i < 10; ++i) if (keywords[i] == buffer) {
		tokenName = "Keyword";
		tokenValue = keywords[i];
		return true;
	}
	return false;
}

bool isType(string buffer) {
	string types[6] = { "INT","int", "CHAR", "char", "string", "STRING" };							//TYPE check
	for (int i = 0; i < 6; ++i) if (types[i] == buffer) {
		tokenName = "Type";
		tokenValue = types[i];
		return true;
	}
	return false;
}

bool isArith(string buffer) {																		//Arithmetic check
	if (buffer.size() != 1)return false;
	char arith[4] = { '+','-','*','/' };
	for (int i = 0; i < 4; i++)if (buffer[0] == arith[i]) {
		tokenName = "Arith";
		tokenValue = string(1, arith[i]);
		return true;
	}
	return false;
}

bool isAssign(string buffer) {																		//Assign check
	if (buffer.size() != 1)return false;
	if (buffer == "=") {
		tokenName = "Assign";
		tokenValue = "=";
		return true;
	}
	return false;
}

bool isSemi(string buffer) {																		//Semicolon check
	if (buffer.size() != 1)return false;
	if (buffer == ";") {
		tokenName = "Semi";
		tokenValue = ";";
		return true;
	}
	return false;
}

bool isPunc(string buffer) {																		//Punctuation check
	if (buffer.size() != 1)return false;
	char punc[7] = { ',','(',')','{','}','\"','\'' };
	for (int i = 0; i < 7; i++)if (buffer[0] == punc[i]) {
		tokenName = "Punc";
		tokenValue = string(1, punc[i]);
		return true;
	}
	return false;
}

bool isComma(string buffer) {
	if (buffer[0] != ',')return false;
	tokenName = "Comma";
	tokenValue = ",";
	return false;
}
bool isNumber(string buffer) {																		//Number check
	if (buffer.size() == 1 && buffer[0] == '0') {//0일때 예외
		tokenName = "Number";
		tokenValue = buffer;
		return true;
	}
	int state = 0;
	for (int i = sIndex; i <= eIndex; i++) {
		if (state == 0) {
			if (mainString[i] == '-')state = 2;
			else if (isNZDigit(i))state = 1;
			else return false;
		}
		else if (state == 1) {
			if (isDigit(i))state = 1;
			else return false;
		}
		else if (state == 2) {
			if (isNZDigit(i))state = 1;
			else return false;
		}
	}
	tokenName = "Number";
	tokenValue = buffer;
	return true;
}
bool isString(string buffer)																		//String check
{
	int state = 0;
	for (int i = sIndex; i <= eIndex; i++) {
		if (state == 0) {
			if (mainString[i] == '"')state = 1;
			else return false;
		}
		else if (state == 1) {
			if (isDigit(i) || string(1, mainString[i]) == "\t" || string(1, mainString[i]) == " " || isLetter(i))state = 1;
			else if (mainString[i] == '"')state = 2;
			else return false;
		}
		else return false;

	}
	if (state != 2 && !mainString[eIndex + 1])
	{
		string s1 = "ERROR line " + to_string(line) + " : string is not at final state ->" + buffer + "\n";
		errorString += s1;
		isReject = true;
		tokenName = "";
		tokenValue = "";
		sIndex = eIndex;
		return true;
	}
	tokenName = "Literal";
	tokenValue = buffer;
	return true;
}
bool isComapare(string buffer)																		//Compare check
{
	int state = 0;
	for (int i = sIndex; i <= eIndex; i++) {
		if (state == 0) {
			if (mainString[i] == '!' || mainString[i] == '=')state = 1;
			else if (mainString[i] == '>' || mainString[i] == '<')state = 2;
			else return false;
		}
		else if (state == 1 || state == 2) {
			if (mainString[i] == '=')state = 3;
			else return false;
		}
		else return false;
	}
	tokenName = "Compare";
	tokenValue = buffer;
	return true;
}
bool isFinalState(string buffer)																	//Check token is at the termination state or not
{
	if (tokenName == "Number")
	{
		int size = buffer.size();
		if (size == 1 && buffer[0] == '0')return true;
		int state = 0;
		for (int i = sIndex; i < eIndex; i++) {
			if (state == 0) {
				if (mainString[i] == '-')state = 2;
				else if (isNZDigit(i))state = 1;
				else return false;
			}
			else if (state == 1) {
				if (isDigit(i))state = 1;
				else return false;
			}
			else if (state == 2) {
				if (isNZDigit(i))state = 1;
				else return false;
			}
		}
		if (state == 1) {
			return true;
		}
		else
		{
			string s1 = "ERROR line " + to_string(line) + " : string is not at final state ->" + buffer + "\n";
			errorString += s1;
			isReject = true;
			tokenName = "";
			tokenValue = "";
			return false;
		}
	}
	else if (tokenName == "Literal")
	{
		int state = 0;
		for (int i = sIndex; i < eIndex; i++) {
			if (state == 0) {
				if (mainString[i] == '"')state = 1;
				else return false;
			}
			else if (state == 1) {
				if (isDigit(i) || isWhitespace(string(1, mainString[i])) || isLetter(i))state = 1;
				else if (mainString[i] == '"')state = 2;
				else return false;
			}
			else return false;
		}
		if (state == 2) return true;
		else
		{
			string s1 = "ERROR line " + to_string(line) + " : string is not at final state ->" + buffer + "\n";
			errorString += s1;
			isReject = true;
			tokenName = "";
			tokenValue = "";
			return false;
		}
	}
	else if (tokenName == "Compare")
	{
		int size = buffer.size();
		int state = 0;
		for (int i = 0; i < size; i++) {
			if (state == 0) {
				if (buffer[i] == '!' || buffer[i] == '=')state = 1;
				else if (buffer[i] == '>' || buffer[i] == '<')state = 2;
				else return false;
			}
			else if (state == 1 || state == 2) {
				if (buffer[i] == '=')state = 3;
				else return false;
			}
			else return false;
		}
		if (state == 2 || state == 3) return true;
		else
		{
			string s1 = "ERROR line " + to_string(line) + " : compare is not at final state ->" + buffer + "\n";
			errorString += s1;
			isReject = true;
			tokenName = "";
			tokenValue = "";
			return false;
		}
	}
	return true;
}