#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
#include<list>
#include<boost/tokenizer.hpp>
#include<map>
#include<vector>
#include<utility>
#include<iomanip>

using namespace std;
using namespace boost;

fstream myReadFile;
char* FILE_NAME;
int moduleCount = 0;
int moduleBaseAddress = 0;
int lNumber = 0;
int lOffset = 0;
int lastLineOffset;
int lastLineNumber;
int memMapCounter = 0;
//Data Structures
typedef struct eachToken{
  string value;
  int lineNumber;
  int lineOffset;  
}token;
list<token> tokens;
list<string> symbolsList;
vector<string> useList;
//map<int,int> module;
map<string,pair<int,int> > symbolTable;
vector<string> symbols;
map<int,int> module;

void getNextTokens() {
  string myLine;
  while(!myReadFile.eof()){
    getline(myReadFile,myLine);
    if(!myReadFile.fail()) {
      lNumber++;
    }

    char_separator<char> sep(" \n\t\0");
    tokenizer<char_separator<char> > toks(myLine,sep);

    for(tokenizer<char_separator<char> >::iterator it = toks.begin(); it != toks.end(); ++it)
    { 
      token t;
      t.value = *it;
      t.lineNumber = lNumber;
      t.lineOffset =(it.base() - myLine.begin() - it->size()) + 1;
      lastLineOffset = strlen(myLine.c_str()) + 1;
      lastLineNumber = lNumber;
      tokens.insert(tokens.end(),t);
      //      cout << "token :  " << t.value << "  lineNumber: " << t.lineNumber << "  lineOffset  : " << t.lineOffset << endl;
    }
    if(!myReadFile.fail()) {
      lastLineOffset = strlen(myLine.c_str()) + 1;
      lastLineNumber = lNumber;
    }
  }

  for(list<token>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
  //  cout << "token :  " << it->value << "  lineNumber: " << it->lineNumber << "  lineOffset  : " << it->lineOffset << endl;
  }
}

void _printMemMap(int address){
  
      cout << setw(3) << setfill('0') << memMapCounter << ": " << address << endl;
}

void _parseerror(int errcode,token t) {
  static const char* errstr[] = {
    "NUM_EXPECTED", // Number expect
    "SYM_EXPECTED", // Symbol Expected
    "ADDR_EXPECTED", // Addressing Expected
    "SYM_TOLONG", // Symbol Name is to long
    "TO_MANY_DEF_IN_MODULE", // > 16
    "TO_MANY_USE_IN_MODULE", // > 16
    "TO_MANY_INSTR" // total num_instr exceeds memory size (512)
  };
  printf("Parse Error line %d offset %d: %s\n", t.lineNumber,t.lineOffset, errstr[errcode]);
  exit(0);
}


token tokenExistenceCheck(int errCode){

  token t;
  if(!tokens.empty()){
    t = tokens.front(); 
  }
  else{
    t.lineNumber = lastLineNumber;
    t.lineOffset = lastLineOffset;
    _parseerror(errCode,t);
  }
  return t;
}

int instructionTypeCheck(){

  token t =  tokenExistenceCheck(2);
  if ((t.value == "R")){
    return 1;}
  else if ((t.value == "I")){
    return 2;
  }
  else if ((t.value == "A")){
    return 3;
  }
  else if ((t.value == "E")){
    return 4;
  }
  else{
    _parseerror(2,t);
    return -1 ;}
}


int digitCheck(string listType){

  token t;
  int returnVal;
  int errCode = 0;

  t = tokenExistenceCheck(0);

  string valToCheck = t.value;

  if (isdigit(valToCheck.c_str()[0])){
    returnVal = atoi(valToCheck.c_str());

    if (returnVal > 16){
      if (listType == "def"){
        _parseerror(4,t);}
      else if (listType == "use") {
        _parseerror(5,t);}
    }

    if (listType == "instr"){
      if ((moduleBaseAddress + returnVal) > 512){
        _parseerror(6,t);}
    }
    return returnVal;
  } 
  else{
    _parseerror(errCode,t);
  }
}

int addressCheck(){

  token t;
  int returnVal;


  t = tokenExistenceCheck(0);
  string valToCheck = t.value;
  int addrLen = strlen(valToCheck.c_str());


  if (isdigit(valToCheck.c_str()[0])) {
      returnVal = atoi(valToCheck.c_str());
      return returnVal;
  }
  else{
    _parseerror(0,t);
  }
}

bool symbolCheck(){

  token t = tokenExistenceCheck(1);

  if (isdigit(t.value.c_str()[0])){
    _parseerror(1,t);
    return false;}
  else {
    if (strlen(t.value.c_str()) > 16) {
      _parseerror(3,t);
    }
    return true;
  }
}

void programTextRead(bool secondpass){
  int count = digitCheck("instr");
  string instructionType; 
  int address;

  tokens.pop_front();

  for (int i=0; i<count; i++){
    //reading instruction type
    int instrType = instructionTypeCheck();
    char instType = tokens.front().value[0];
    tokens.pop_front();
    
    address = addressCheck();
    tokens.pop_front();

    if(secondpass) {
     switch (instType) {
      case 'R' :
        //relative type
        _printMemMap(moduleBaseAddress + address);
        break;

      case 'I':
        //immediate type
        _printMemMap(address);
      break;

      case 'A':
        //absolute type
        _printMemMap(address);
        break;

      case 'E':
        //extern type
         int operand = address % 1000;
         string symbolUsed = useList[operand];
         int symbolTableAddress = symbolTable[symbolUsed].first;

         address = (address / 1000)*1000 + symbolTableAddress;
        _printMemMap(address);

        break;

      }
      memMapCounter++;
    }
    //    addressValue = tokens.front().value;
    }
  
  moduleBaseAddress = moduleBaseAddress + count;
  moduleCount++;
  module[moduleCount] = moduleBaseAddress;

  for (list<string>::iterator it = symbolsList.begin(); it != symbolsList.end(); ++it){
    if(symbolTable[*it].first >  moduleBaseAddress){
      cout << "Warning: Module " << moduleCount << ": " << *it << " to big " << symbolTable[*it].first << " (max=" << moduleBaseAddress - 1 << ") assume zero relative" << endl;
      symbolTable[*it].first = 0;
  }
  }  

  symbolsList.clear();
}

void useListRead(){
  int count = digitCheck("use");
  tokens.pop_front();
  
  //reading symbols
  while(count != 0 ) {

     symbolCheck();
     useList.push_back(tokens.front().value);
     tokens.pop_front();
     count--;
  }
}

void defListRead(){

  int count;
  int address;
  map<string,int> symbolCount;

  count = digitCheck("def");
  tokens.pop_front();

  for (int i=0;i < count;i++){

    //checking symbols
    symbolCheck();
    string symbol = tokens.front().value;
    symbolsList.push_back(symbol);
    


    tokens.pop_front();

    //checking address
    address = addressCheck();
    tokens.pop_front();
    if (symbolTable.count(symbol) > 0){
      symbolTable[symbol].second = 1;
    } else{
      symbolTable[symbol] = make_pair(moduleBaseAddress + address,0);
      symbols.push_back(symbol);
    }
  }// end for
}
void reset() {
  moduleCount = 0;
  moduleBaseAddress = 0;
  lNumber = 0;
  lOffset = 0;
  lastLineOffset = 0;
  lastLineNumber = 0;
  memMapCounter = 0;
}

void second_pass(){
    reset();
    myReadFile.open(FILE_NAME,ios::in | ios::out);
      if(myReadFile.good() && !myReadFile.eof()){
         getNextTokens();
         cout << "Memory Map" << endl;
         while (!tokens.empty()) {
           defListRead();
           useListRead();
           programTextRead(true);
           useList.clear();
         }
      }
      myReadFile.close();
}


void first_pass(){
  string multipleDefinitions = "Error: This variable is multiple times defined; first value used";
  myReadFile.open(FILE_NAME,ios::in | ios::out);
  if(myReadFile.good() && !myReadFile.eof()){
    getNextTokens();
    while (!tokens.empty()) {
      defListRead();
      useListRead();
      programTextRead(false);
    }
    vector<string>::iterator it;
    cout << "Symbol Table" << endl;
 //   cout << symbols.front() << endl;
  
    for(it=symbols.begin() ; it < symbols.end(); it++) {
      cout << *it << "=" <<symbolTable[*it].first; 
      if (symbolTable[*it].second == 1){
        cout <<" "<<  multipleDefinitions;
      }
      cout << endl; 
    }
    tokens.clear();
    cout << endl;
  }
  myReadFile.close();
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
