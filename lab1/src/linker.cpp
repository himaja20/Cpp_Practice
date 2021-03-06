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
list<string> symbolsList; //def list for symbols in current module.
vector<pair<string, int> > useList; // use list for symbols in current module.
map<string,pair<int,int> > symbolTable;
vector<string> symbols;//define list holding all the symbols in the input file
//map<int,int> module;
map<string,pair<int,int> > useListSymbols;
map<string,int> symDefModuleNum;
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

void _printMemMap(pair<int,int> address, string errMsg){
  
      cout << setw(3) << setfill('0') << memMapCounter << ": " << address.first << setw(3) << setfill('0') << address.second ;
      if (errMsg != ""){
        cout << " " << errMsg;
      }
      cout << endl;
      memMapCounter++;
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

string instructionTypeCheck(){

  token t =  tokenExistenceCheck(2);
  if ((t.value == "R") ||(t.value == "I") ||(t.value == "E") || (t.value == "A")){
    return t.value;
  }
  else{
    _parseerror(2,t);
}
}
int digitCheck(string listType){

  token t;
  int returnVal;
  int errCode = 0;

  t = tokenExistenceCheck(0);

  string valToCheck = t.value;

  char* pEnd;
  returnVal = (int)strtol(valToCheck.c_str(),&pEnd ,10);

    if((*pEnd == '\0') && (returnVal >= 0)){
//  if (isdigit(valToCheck.c_str()[0])){
   // returnVal = atoi(valToCheck.c_str());

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
  char* pEnd;

  t = tokenExistenceCheck(0);
  string valToCheck = t.value;
  int addrLen = strlen(valToCheck.c_str());

  returnVal = (int)strtol(valToCheck.c_str(),&pEnd,10);

  if((*pEnd == '\0') && (returnVal >= 0)) {
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
 
  int symbolTableAddress;
  int prevIsUsedValue = 0;
  string symbolUsed;
  int count = digitCheck("instr");
  string instructionType; 
  int address;
  pair<int,int> instrAddr;

  tokens.pop_front();

  for (int i=0; i<count; i++){
    //reading instruction type
    prevIsUsedValue = 0;
    string instrType = instructionTypeCheck();
    char instType =instrType[0];// tokens.front().value[0];
    string errorMessage;
    tokens.pop_front();
    
    address = addressCheck();
    
    // making a opcode,operand pair
    instrAddr = make_pair(address/1000,address%1000);

    tokens.pop_front();

    if(secondpass) {
     switch (instType) {
      case 'R' :
        //relative type

         if (instrAddr.first > 9){
            instrAddr.first = 9;
            instrAddr.second = 999;
            errorMessage = "Error: Illegal opcode; treated as 9999";
         }
         else if (instrAddr.second >= count ) {
            errorMessage = "Error: Relative address exceeds module size; zero used";
            instrAddr.second = moduleBaseAddress;
         }
         else{
           instrAddr.second = moduleBaseAddress + instrAddr.second;
         }
        _printMemMap(instrAddr,errorMessage);
        break;

      case 'I':
        //immediate type
        if (address> 9999){
           errorMessage =  "Error: Illegal immediate value; treated as 9999";
           instrAddr.first = 9;
           instrAddr.second = 999;
        }
        _printMemMap(instrAddr,errorMessage);
      break;

      case 'A':
        //absolute type
        if (instrAddr.second > 512){
          errorMessage = "Error: Absolute address exceeds machine size; zero used";
          instrAddr.second = 000;
        }
        _printMemMap(instrAddr,errorMessage);
        break;

      case 'E':
        //extern type
        if (instrAddr.second >  useList.size()-1){
          errorMessage = "Error: External address exceeds length of uselist; treated as immediate";
          _printMemMap (instrAddr,errorMessage);
         }
        else
        {
          symbolUsed = useList[instrAddr.second].first;

         
         // Update the isUsed flag.
         if (useListSymbols.count(symbolUsed) > 0 ) {
           useListSymbols[symbolUsed].second++;
         }
         useList[instrAddr.second].second++;

         if(symbolTable.count(symbolUsed) == 0) {
           errorMessage = "Error: " + symbolUsed + " is not defined; zero used";
           instrAddr.second = 000;
         }
     
         if (symbolTable.count(symbolUsed) > 0){
           symbolTableAddress = symbolTable[symbolUsed].first;
           instrAddr.second = symbolTableAddress;
         }
        _printMemMap(instrAddr,errorMessage);

        break;

      }
    }
   }// end of second pass logic
  } //end for 

  if(secondpass) {
    for(vector<pair<string,int> >::iterator it = useList.begin() ; it != useList.end() ; ++it){
  //    cout << it->second << endl;
          if (it->second == 0){
             cout << "Warning: Module " << moduleCount + 1<< ": " << it->first << " appeared in the uselist but was not actually used" << endl;
          }
    }
  }

 moduleCount++;
  for (list<string>::iterator it = symbolsList.begin(); it != symbolsList.end(); ++it){
    if((symbolTable[*it].first - moduleBaseAddress)> count ){
      cout << "Warning: Module " << moduleCount << ": " << *it << " to big " << symbolTable[*it].first - moduleBaseAddress << " (max="<< count - 1 << ") assume zero relative" << endl;
      symbolTable[*it].first = moduleBaseAddress;
  }
  }

 moduleBaseAddress = moduleBaseAddress + count;
 module[moduleCount] = moduleBaseAddress;

  symbolsList.clear();
}

void useListRead(bool secondpass){

  int count = digitCheck("use");
  tokens.pop_front();
  
  //reading symbols
  while(count > 0 ) {

     symbolCheck();
     useList.push_back(make_pair(tokens.front().value, 0));
    if (secondpass){
      if (!(useListSymbols.count(tokens.front().value) > 0)){
         useListSymbols[tokens.front().value] = make_pair(moduleCount + 1, 0);
      }
    }
     tokens.pop_front();
     count--;
  }
}

void defListRead(bool secondpass){

  int count;
  int address;
  map<string,int> symbolCount;

  count = digitCheck("def");
  tokens.pop_front();

  for (int i=0;i < count;i++){

    //checking symbols
    symbolCheck();
    string symbol = tokens.front().value;
    //symbolsList.push_back(symbol);

    tokens.pop_front();

    //checking address
    address = addressCheck();
    tokens.pop_front();
    if(!secondpass){
      if (symbolTable.count(symbol) > 0){
        symbolTable[symbol].second = 1;
      }
      else{
        symbolTable[symbol] = make_pair(moduleBaseAddress + address,0);
        symbols.push_back(symbol);
        symDefModuleNum[symbol] = moduleCount + 1;
      }
       symbolsList.push_back(symbol);
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
}

void second_pass(){
    reset();
    myReadFile.open(FILE_NAME,ios::in | ios::out);
      if(myReadFile.good() && !myReadFile.eof()){
         getNextTokens();
         cout << "Memory Map" << endl;
         while (!tokens.empty()) {
           defListRead(true);
           useListRead(true);
           programTextRead(true);
           useList.clear();
         }
      //Warning messages
     
      for(vector<string>::iterator it = symbols.begin() ; it != symbols.end() ; ++it){
        if ((useListSymbols.count(*it) > 0)){
          if (useListSymbols[*it].second == 0){
            cout << "Warning: Module " << symDefModuleNum[*it]<< ": " << *it << " was defined but never used " << endl;
          }
         }
        else{
            cout << "Warning: Module " << symDefModuleNum[*it]<< ": " << *it << " was defined but never used " << endl;
          }
      }

      myReadFile.close();
}
}

void first_pass(){
  string multipleDefinitions = "Error: This variable is multiple times defined; first value used";
  myReadFile.open(FILE_NAME,ios::in | ios::out);
  if(myReadFile.good() && !myReadFile.eof()){
    getNextTokens();
    while (!tokens.empty()) {
      defListRead(false);
      useListRead(false);
      programTextRead(false);
      useList.clear();
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

