#include<iostream>
#include<fstream>
#include<string>
#include<list>
#include<boost/tokenizer.hpp>
#include<map>
#include<vector>

using namespace std;
using namespace boost;

fstream myReadFile;
char* FILE_NAME;
int moduleCount = 0;
int moduleBaseAddress = 0;
int lNumber = 0;
int lOffset = 0;

//Data Structures
typedef struct eachToken{
  string value;
  int lineNumber;
  int lineOffset;  
}token;
list<token> tokens;

//map<int,int> module;
map<string,int> symbolTable;
vector<string> symbols;

void second_pass(){
  cout << "dummy" << endl ;
  return;
}

void getNextTokens() {

  string myLine;
  while(!myReadFile.eof()){
    //  if (tokens.empty()){
    //  if (myLine != ""){
    getline(myReadFile,myLine);
    lNumber++;
    char_separator<char> sep(" \n\t\0");
    tokenizer<char_separator<char> > toks(myLine,sep);
//    tokens.insert(tokens.end(),toks.begin(),toks.end());

    for(tokenizer<char_separator<char> >::iterator it = toks.begin(); it != toks.end(); ++it)
    { 
      token t;
      t.value = *it;
      t.lineNumber = lNumber;
      t.lineOffset = it.base() - myLine.begin() - it->size();
      tokens.insert(tokens.end(),t);
      cout << "token :  " << t.value << "  lineNumber: " << t.lineNumber << "  lineOffset  : " << t.lineOffset << endl;
    } 
    // } 
    // }
  }
  return;
}

bool instructionTypeCheck(string token){

  if (token == "R" || "E" || "A" || "I"){
    return true;
  }
  else{return false ;}
}


int digitCheck(string token){

  int returnVal;
  if (isdigit(token.c_str()[0])){
    returnVal = atoi(token.c_str());
    return returnVal;
  } 
  else{
    return -1;
  }
}

bool symbolCheck(string token){
  if (isdigit(token.c_str()[0])){
    return false;}
  else {return true;}
}

void programTextRead(){

  int count =digitCheck(tokens.front().value);
  string instructionType; 
  string addressValue;
  if (count > 0) {
    cout << "program text count : " << count  << endl;}
  else {
    cout << "program text count not found error" << endl;
  }
  tokens.pop_front();
  for (int i=0; i<count; i++){
    instructionType = tokens.front().value;
    //reading instruction type
    if (instructionTypeCheck(instructionType)){
      cout << "InstructionType: " + instructionType  << endl;
    }
    else{
      cout << "InstructionType Mismatch Error" << endl; }
    tokens.pop_front();

    //reading addresses

    addressValue = tokens.front().value;
    if (digitCheck(addressValue) > 0){
      cout << "address of the instruction : " << digitCheck(addressValue) << endl;
    }
    else { cout << "address of the instruction Error in Program Text" << endl; }
    tokens.pop_front();
  }
  moduleBaseAddress = moduleBaseAddress + count;
  moduleCount++;

}

void useListRead(){
  int count = digitCheck(tokens.front().value);
  tokens.pop_front();
  if (count == -1){
    cout << "count error" << endl;
  }
  else{
    cout << "count : " << count << endl; 
  }
  //reading symbols
  while(count != 0 ) {

    if (symbolCheck(tokens.front().value)){
      cout << "symbol: " + tokens.front().value << endl;
      tokens.pop_front();}
    else{
      cout << "symbol error in use list" << endl;
    }
    count--;
  }
}

void defListRead(){

  // validateDefList();
  int count;
  int address;
  //   getNextTokens(); 
  string count_str = tokens.front().value;
  tokens.pop_front();
  if (isdigit(count_str.c_str()[0]))
  {
    cout << "in if deflistread" << endl;
    count = atoi(count_str.c_str());
    for (int i=0;i < count;i++){

      //checking symbols
      //     getNextTokens();
      string symbol = tokens.front().value;
      tokens.pop_front();
      if (isdigit(symbol.c_str()[0]))
      {  
        cout<< "error" << endl;
      }

      //checking address
      //    getNextTokens();
      string address_str = tokens.front().value;
      tokens.pop_front();
      if (!(isdigit(address_str.c_str()[0])))
      {
        cout << "error address" << endl;
      }
      else
      {
        address = atoi(address_str.c_str());
      }  
      cout << symbol + ": " <<  address << endl;
      symbolTable[symbol] = moduleBaseAddress + address;
      symbols.push_back(symbol);

    }// end for

  }
  else
  {
    cout << "in else" << endl;
    return;
  }
}



void first_pass(){
  myReadFile.open(FILE_NAME,ios::in | ios::out);
  if(myReadFile.good() && !myReadFile.eof()){
    getNextTokens();
    while (!tokens.empty()) {
    defListRead();
    useListRead();
    programTextRead();
    cout << "moduleCount:  " << moduleCount << endl;
    cout << "modulebaseAddress : " << moduleBaseAddress << endl;
    }
    vector<string>::iterator it;
    cout << "Symbol Table" << endl;
    for(it=symbols.begin() ; it < symbols.end(); it++) {
      cout << *it << " " <<symbolTable[*it] << endl;
    }
  }
}


int main(int argc, char* argv[]) {
  FILE_NAME = argv[1];
  //myReadFile(FILE_NAME);
  if ((argc <= 1) || (FILE_NAME == "")) {
    cout << "Expected argument after options";
    return 0;
  }
  first_pass();
  second_pass();
}

// ifstream myReadFile("/home/ajr619/himmi/os/lab1/labsamples/input-1");
/* char output[100];
   if (myReadFile.good()) {
   cout << "if passed"  << endl ;
   while (!myReadFile.eof()) {


   myReadFile >> output;
   cout<<output;


   }
   }else{
   cout << "in else" << endl;
   }
   myReadFile.close();
   return 0;
   }*/
