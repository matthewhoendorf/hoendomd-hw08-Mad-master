/*    @file MadLib.cpp    
      @author <fill me in>
      @date <fill me in>  

			@description This program accepts a grammar instance text file and 
			prints out a random instance from that grammar.
*/

#include <iostream>
#include <cstdlib>
#include <map>
#include <fstream>

#include "expression.h"

using namespace std;

Expression* parse(map<string, Expression*>* keywords, string input){
  if(input.find('|') != string::npos){
    Choice* output = new Choice;
    string lhs = input.substr(0, input.find("|"));
    string rhs = input.substr(input.find("|")+1, input.length()-1);
    output->addExpression(parse(keywords, lhs));
    while(rhs.size() > 0){
      int next;
      string current;
      if(rhs.find("|") != string::npos){
        next = rhs.find("|");
        current = rhs.substr(0,next);
        rhs = rhs.substr(next+1,input.length()-1);
      }
      else{
        next = rhs.length() -1;
        current = rhs.substr(0, next+1);
        rhs = "";
      }
      output->addExpression(parse(keywords,current));
    }
    return output;
  }
  else if(input.find(' ') != string::npos){
    Sequence* s_temp = new Sequence;
    string lhs = input.substr(0, input.find(" "));
    string rhs = input.substr(input.find(" ")+1, input.length()-1);
    s_temp->addExpression(parse(keywords, lhs));
    if(rhs.length() > 0){
      s_temp->addExpression(parse(keywords, rhs));  
    }
    return s_temp;
  }
  else if(input.find('<') != string::npos){
    string keyword = input.substr(1,input.length()-2);
    Keyword* k_temp = new Keyword(keyword,keywords);
    return k_temp;
  }
  else{
    Terminal* base = new Terminal(input);
    return base;
  }
}

// Returns substring that is to the right of the colon
string getRHS(string s){
  string rhs;
  int first = s.find(":") + 1;
  int last = s.length() - 1;
  rhs = s.substr(first, last);
  return rhs;
}

// Returns the keyword (before the colon)
string getKeyword(string s){
  
  string keyword;
  int last = s.find(":");
  keyword = s.substr(0, last);
  return keyword;
}

int main(int argc, char* argv[]){
  // Randomize number generator
  srand(time(0));

  // Open file
  ifstream read_file(argv[1]);
  string read_in;
  if(!read_file.is_open()){
    cerr << "\nUsage: " << argv[0] << " <file_name>.txt - please provide a valid filename.\n" << endl;
    return 1;
  }

  // Create map of strings to expressions
  map<string, Expression*>* m = new map<string, Expression*>;

  // For every line in the file, send the right side to parse
  // and put the resulting Expression pointer into the map.
  while(getline(read_file, read_in)){
    string rhs = getRHS(read_in);
    string keyword = getKeyword(read_in);
    (*m)[keyword] = parse(m, rhs);
  }  
  // Get start expression and print out sentence!
  Expression* s1 = (*m)["start"];
  cout << "\n" << s1->getString() << "\n" << endl; 
  return 0;

}
