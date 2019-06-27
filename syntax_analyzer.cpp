#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <windows.h>
#include <vector>

using namespace std;

int SLRtable[100][100];

pair<int, int> reduce[50] = { {0,0},{1,-1},{2,20},{2,20},{3,21},{3,23},{3,24},{1,26},{9,22},{2,25},{4,26},{7,26},{11,26},{1,27},{1,27},{3,28},{1,28},{3,29},{1,29},{1,30},{1,30},{3,30},{3,31},{3,32},{0,20},{0,23},{0,24},{0,25} };
int token[100000];
string tokenSpeaker[50] = { "","vtype","num","literal","id","if","else","while","return","addsub","multidiv","assign","comp","semi","comma","lparen","rparen","lbrace","rbrace","$","CODE","VDECL","FDECL","ARG","MOREARGS","BLOCK","STMT","RHS","EXPR","TERM","FACTOR","COND","RETURN" };
char c[10001];
int line = 0;
stack<int> st;
void showStack();

int  main(int argc, char** argv) {

	ifstream inputFile(argv[1]);
	//ifstream inputFile("test.out");																		  //file read
	while (!inputFile.eof()) {
		string mainString = ""; inputFile.getline(c, 2000); mainString = ""; mainString += c;
		line++;
		if      (mainString.substr(0, 6) == "Type :"									)	token[line] = 1;  //vtype       20 CODE  
		else if (mainString.substr(0, 8) == "Number :"									)	token[line] = 2;  //num         21 VDECL
		else if (mainString.substr(0, 9) == "Literal :"									)	token[line] = 3;  //literal     22 FDECL
		else if (mainString.substr(0, 4) == "ID :"										)	token[line] = 4;  //id          23 ARG
		else if (mainString == "Keyword : IF"		|| mainString == "Keyword : if"		)	token[line] = 5;  //if			24 MOREARGS
		else if (mainString == "Keyword : ELSE"		|| mainString == "Keyword : else"	)	token[line] = 6;  //else		25 BLOCK
		else if (mainString == "Keyword : WHILE"	|| mainString == "Keyword : while"	)	token[line] = 7;  //while		26 STMT
		else if (mainString == "Keyword : RETURN"	|| mainString == "Keyword : return"	)	token[line] = 8;  //return		27 RHS
		else if (mainString == "Arith : +"			|| mainString == "Arith : -"		)	token[line] = 9;  //addsub		28 EXPR
		else if (mainString == "Arith : *"			|| mainString == "Arith : /"		)	token[line] = 10; //multidiv	29 TERM
		else if (mainString == "Assign : ="												)	token[line] = 11; //assign		30 FACTOR
		else if (mainString == "Compare : =="		|| mainString == "Compare : <=" ||
				 mainString == "Compare : >="		|| mainString == "Compare : <"  ||
				 mainString == "Compare : >"											)	token[line] = 12; //comp		31 COND
		else if (mainString == "Semi : ;"												)	token[line] = 13; //semi		32 RETURN
		else if (mainString == "Comma : ,"												)	token[line] = 14; //comma
		else if (mainString == "Punc : ("												)	token[line] = 15; //lparen
		else if (mainString == "Punc : )"												)	token[line] = 16; //rparen
		else if (mainString == "Punc : {"												)	token[line] = 17; //lbrace
		else if (mainString == "Punc : }"												)	token[line] = 18; //rbrace
	}																								    //19; //$

	ifstream file("SLRtable.txt");
	if (file.is_open())for (int i = 1; i <= 67; i++)for (int j = 1; j <= 32; j++)file >> SLRtable[i][j];	  //SLRtable input
	token[line] = 19;																					   

	int index = 1;

	for (int i = 1; i <= line; i++)cout << tokenSpeaker[token[i]] << " ";									  //Show token stream
	cout << "\n";

	st.push(1);																								  //push starting state
	while (1)
	{
		showStack();

		int currentState = st.top();	cout << "\ncurrent state : " << currentState << "\n";				  //show currentState

		int nextToken = token[index];	cout << "nextToken : " << tokenSpeaker[nextToken] << "\n";			  //show nextToken\


		if (currentState == 2 && nextToken == 19)															  //syntax_analyzer terminate condition
		{
			cout << "\n\n\nNo problem in syntax_analyzer! Well done!\n";
			system("pause");
			return 0;
		}
		
		int result = SLRtable[currentState][nextToken];														  //REDUCE or SHIFT 

		if (result == 0)break;																			      //accessing blank in SLRtable -> reject

		if (result < 100)																					  //SHIFT
		{
			cout << "shift -> state " << result << "\n";
			index++;
			st.push(result);
		}
		else if (result > 100)																				  //REDUCE
		{
			cout << "reduce -> " << result - 100 << "\n";
			int reduceNum = reduce[result - 100].first;
			cout << "pop " << reduceNum << " times\n";
			while (reduceNum--)st.pop();
			showStack();

			currentState = st.top();	cout << "reduced top : " << currentState << "\n";					  //show currentState after reducing
			cout << "goto -> " << tokenSpeaker[reduce[result - 100].second] << "\n";
			result = SLRtable[currentState][reduce[result - 100].second];
			if (result == 0)break;																			  //accessing blank in SLRtable -> reject
			st.push(result);
		}
	}
	cout << "\n\nThere is a syntax problem in this input.\nPlease check your code again.\n";
	system("pause");
	return 0;
}
void showStack()
{
	vector<int> v;
	while (!st.empty())
	{
		v.push_back(st.top());
		st.pop();
	}
	cout << "\n\n\ncurrent stack : ";
	int size = v.size();
	for (int k = size - 1; k >= 0; k--)
	{
		cout << v[k] << " ";
		st.push(v[k]);
	}
	v.clear();
}