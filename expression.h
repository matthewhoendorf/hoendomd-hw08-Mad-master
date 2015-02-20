#include <iostream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

class Expression{
public:
  virtual string getString(){}; // getString placeholder function
  ~Expression(){};
};

/*  Terminal - Expression that stores a string (e.g. word)
*   i.e. [expression] = terminal  (non-whitespace characters) 
*/
class Terminal: public Expression{
public:
  Terminal(string s){
	  word = s;
  }
  string getString(){
    return word;
  };
private:
  Terminal(){};
  string word;
};

/*  Choice - Expression that stores n choices of expressions
*   i.e. [expression] = [expression]|[expression]|..  (choice) 
*/
class Choice: public Expression{
public:
  Choice(){};
  string getString(){
    int selection = rand() % choices.size(); // Randomly select one choice pos
    return choices[selection]->getString(); // Return choice @ rand. index
  };
  void addExpression(Expression* exp){
    choices.push_back(exp);
  };
private:
  vector<Expression*> choices;
};

/*  Sequence - Expression that stores sequence of expressions
*   i.e. [expression] = [expression] [expression] … (sequence) 
*/
class Sequence: public Expression{
public:
  Sequence(){};
  string getString(){
    string s;
    for(int i = 0; i < seq.size(); i++){ // Append every expression in seq.
      s.append(seq[i]->getString()); // to string to be returned
      if(i != seq.size()-1){
      	s.push_back(' ');
     }
    }
    return s;
  };
  void addExpression(Expression* exp){
    seq.push_back(exp);
  };
private:
  vector<Expression*> seq;
};

class Keyword: public Expression{
public:
	Keyword(string word, map<string, Expression*>* m){
			keyword = word;
			info = m;
	};
	string getString(){
    return (*info)[keyword]->getString();
  };
  ~Keyword(){
  	for(map<string, Expression*>::iterator it = info->begin(); it != info->end(); ++it){
    	delete it->second;
  	}
  }
private:
	Keyword(){};
	string keyword;
	map<string, Expression*>* info;
};