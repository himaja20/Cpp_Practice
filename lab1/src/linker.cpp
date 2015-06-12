#include<iostream>
#include<fstream>
#include<string>
#include<string.h>
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
int lastLineOffset;
int lastLineNumber;
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
    getline(myReadFile,myLine);
   // lNumber++;
    //lastLineOffset = strlen(myLine.c_str()) + 1;
//    cout << myLine  << "   " << "lastLineOffset  : " << lastLineOffset << endl;
    //lastLineNumber = lNumber;
   // cout << ">>  " << strlen(myLine.c_str()) << endl;
   // if(myLine[0] != '\0') {
        lNumber++;
    //}

    char_separator<char> sep(" \n\t\0");
    tokenizer<char_separator<char> > toks(myLine,sep);

    for(tokenizer<char_separator<char> >::iterator it = toks.begin(); it != toks.end(); ++it)
    { 
      token t;
      t.value = *it;
      t.lineNumber = lNumber;
      t.lineOffset =(it.base() - myLine.begin() - it->size()) + 1;
      lastLineOffset = strlen(myLine.c_str()) + 1;
      cout << "lastLineOffset in for loop : " << lastLineOffset << endl;
      lastLineNumber = lNumber;
      tokens.insert(tokens.end(),t);
//      cout << "token :  " << t.value << "  lineNumber: " << t.lineNumber << "  lineOffset  : " << t.lineOffset << endl;
    }
   // if(myLine[0] != '\0') {
      lastLineOffset = strlen(myLine.c_str()) + 1;
      lastLineNumber = lNumber;
   // }
  }

  for(list<token>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
    cout << "token :  " << it->value << "  lineNumber: " << it->lineNumber << "  lineOffset  : " << it->lineOffset << endl;
  
  }
  cout << "lastLineOffset before exiting getNextTokens : " << lastLineOffset << endl;
  return;
}

void _parseerror(int errcode,token t) {
   static char* errstr[] = {
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

  cout << "in tokenExistenceCheck " << endl;
token t;
if(!tokens.empty()){
  t = tokens.front(); 
  cout << "token list not empty" << endl;
}
else{
  t.lineNumber = lastLineNumber;
  t.lineOffset = lastLineOffset;
  cout << "lastLineOffset before assiging : " <<lastLineOffset << endl;
  cout << "parseError in Token Existence check : " << t.lineOffset << endl;
  _parseerror(errCode,t);
}
return t;
}

int instructionTypeCheck(){

token t =  tokenExistenceCheck(2);
  cout << "instructiontype :  " << t.value << endl;
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

  cout << "in address check" << endl;
  token t;
  int returnVal;

  
  t = tokenExistenceCheck(0);
  string valToCheck = t.value;
  int addrLen = strlen(valToCheck.c_str());


  if (isdigit(valToCheck.c_str()[0])) {
    if (addrLen != 4){
      _parseerror(0,t);}
    else{
    returnVal = atoi(valToCheck.c_str());
    return returnVal;
     } 
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

void programTextRead(){

  int count = digitCheck("instr");
  string instructionType; 
  string addressValue;

  tokens.pop_front();

  for (int i=0; i<count; i++){
    //reading instruction type
  int instrType = instructionTypeCheck();
  
  tokens.pop_front();

  switch (instrType) {

    case 1 :
    //relative type
    break;

    case 2:
    //immediate type
    break;

    case 3:
    //absolute type
    break;

    case 4:
    //extern type
    break;

  }

  tokens.pop_front();

//    addressValue = tokens.front().value;
    if (addressCheck() > 0){
      cout << "address of the instruction : " <<  endl;
    }
    else { cout << "address of the instruction Error in Program Text" << endl; }
    tokens.pop_front();
  }
  moduleBaseAddress = moduleBaseAddress + count;
  moduleCount++;

}

void useListRead(){
  int count = digitCheck("use");
  tokens.pop_front();
  if (count == -1){
    cout << "count error" << endl;
  }
  else{
    cout << "count : " << count << endl; 
  }
  //reading symbols
  while(count != 0 ) {

    if (symbolCheck()){
      cout << "symbol: " + tokens.front().value << endl;
      tokens.pop_front();}
    else{
      cout << "symbol error in use list" << endl;
    }
    count--;
  }
}

void defListRead(){

  cout << "in if deflistread" << endl;
  int count;
  int address;

  count = digitCheck("def");
  tokens.pop_front();

    for (int i=0;i < count;i++){

      //checking symbols
      symbolCheck();
      string symbol = tokens.front().value;
      tokens.pop_front();

      //checking address
      addressCheck();
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
