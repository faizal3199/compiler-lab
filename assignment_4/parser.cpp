#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctype.h>

#define vector_string vector<string>

using namespace std;

extern void yylex();
extern FILE* yyin;

// State	ACTION	                                                                    GOTO
//          while	begin	end	lparn	rop	rparn	semi	id	aop	mop	sub	num	$	W	C	S	P	E	I
string parseTable[39][21] = {
    {"0", "s2", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "1", " ", " ", " ", " ", " "},
    {"1", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "acc", " ", " ", " ", " ", " ", " ", " "},
    {"2", " ", " ", " ", "s4", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "3", " ", " ", " ", " "},
    {"3", " ", "s5", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"4", " ", " ", " ", " ", " ", " ", " ", "s7", " ", " ", "s9", "s8", " ", " ", " ", " ", " ", " ", " ", "6"},
    {"5", " ", " ", "r4", " ", " ", " ", " ", "s12", " ", " ", " ", " ", " ", " ", " ", " ", "10", "11", " ", " "},
    {"6", " ", " ", " ", " ", "s13", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"7", " ", " ", " ", " ", "r9", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"8", " ", " ", " ", " ", "r10", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"9", " ", " ", " ", " ", " ", " ", " ", "s7", " ", " ", "s9", "s8", " ", " ", " ", " ", " ", " ", " ", "14"},
    {"10", " ", " ", "s15", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"11", " ", " ", " ", " ", " ", " ", "s16", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"12", " ", " ", " ", " ", " ", " ", " ", " ", "s17", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"13", " ", " ", " ", " ", " ", " ", " ", "s19", " ", " ", "s21", "s20", " ", " ", " ", " ", " ", " ", " ", "18"},
    {"14", " ", " ", " ", " ", "r11", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"15", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "r1", " ", " ", " ", " ", " ", " ", " "},
    {"16", " ", " ", "r4", " ", " ", " ", " ", "s12", " ", " ", " ", " ", " ", " ", " ", " ", "22", "11", " ", " "},
    {"17", " ", " ", " ", " ", " ", " ", " ", "s25", " ", " ", "s27", "s26", " ", " ", " ", " ", " ", " ", "23", "24"},
    {"18", " ", " ", " ", " ", " ", "s28", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"19", " ", " ", " ", " ", " ", "r9", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"20", " ", " ", " ", " ", " ", "r10", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"21", " ", " ", " ", " ", " ", " ", " ", "s19", " ", " ", "s21", "s20", " ", " ", " ", " ", " ", " ", " ", "29"},
    {"22", " ", " ", "r3", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"23", " ", " ", " ", " ", " ", " ", "r5", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"24", " ", " ", " ", " ", " ", " ", "r8", " ", " ", "s30", "s31", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"25", " ", " ", " ", " ", " ", " ", "r9", " ", " ", "r9", "r9", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"26", " ", " ", " ", " ", " ", " ", "r10", " ", " ", "r10", "r10", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"27", " ", " ", " ", " ", " ", " ", " ", "s25", " ", " ", "s27", "s26", " ", " ", " ", " ", " ", " ", " ", "32"},
    {"28", " ", "r2", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"29", " ", " ", " ", " ", " ", "r11", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"30", " ", " ", " ", " ", " ", " ", " ", "s34", " ", " ", "s36", "s35", " ", " ", " ", " ", " ", " ", " ", "33"},
    {"31", " ", " ", " ", " ", " ", " ", " ", "s34", " ", " ", "s36", "s35", " ", " ", " ", " ", " ", " ", " ", "37"},
    {"32", " ", " ", " ", " ", " ", " ", "r11", " ", " ", "r11", "r11", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"33", " ", " ", " ", " ", " ", " ", "r6", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"34", " ", " ", " ", " ", " ", " ", "r9", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"35", " ", " ", " ", " ", " ", " ", "r10", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"36", " ", " ", " ", " ", " ", " ", " ", "s34", " ", " ", "s36", "s35", " ", " ", " ", " ", " ", " ", " ", "38"},
    {"37", " ", " ", " ", " ", " ", " ", "r7", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "},
    {"38", " ", " ", " ", " ", " ", " ", "r11", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "}
};

map<int, vector<int>> reducingRules;
vector<int> stateStack;
vector<int> symbolsStack;
map<string, int> numToken;

bool debugStatus = (getenv("DEBUG")?true:false);

void dumpVector(vector<int> &x){
    for(int i=0;i<x.size();i++){
        printf("%d ",x.at(i));
    }
    printf("\n");
}

void debugLog(char* x){
    if(debugStatus){
        printf("[+] %s\n",x);
    }
}

void handleInput(char*);

bool checkAllLower(string token){
    for(int i=0;i<token.length();i++){
        if(!islower(token.at(i))){
            return false;
        }
    }
    return true;
}

void generateNumsFromRules(map<int, vector<string>> &rules){
    debugLog("Generating nums for token in rules");
    int index = 1;
    bool epsilonPresent = false;

    // Insert all terminals
    for(const auto &pair: rules){
        for(int i=0;i<pair.second.size();i++){
            if (pair.second.at(i) == ""){
                epsilonPresent = true;
                continue;
            }
            if(checkAllLower(pair.second.at(i))){
                auto temp = numToken.insert({pair.second.at(i), index});

                if(temp.second){
                    string tmp = "Inserting `"+pair.second.at(i) + "` at index: " + to_string(index);
                    debugLog((char *)tmp.c_str());
                    index++;
                }
            }
        }
    }

    numToken.insert({"$", index});
    index++;

    // Insert all non terminals
    // Consider only the left term of the production rule
    for(const auto &pair: rules){
        auto temp = numToken.insert({pair.second.at(0), index});

        if(temp.second){
            string tmp = "Inserting `"+pair.second.at(0) + "` at index: " + to_string(index);
            debugLog((char *)tmp.c_str());
            index++;
        }
    }

    if(epsilonPresent){
        numToken.insert({"", index});
    }
    debugLog("Nums generated for tokens");
}

int getTokenNum(char* inputToken){
    string token(inputToken);

    auto index = numToken.find(inputToken);
    if (index != numToken.end()){
        return index->second;
    }
    
    printf("Invalid Token: `%s`\n",inputToken);
    exit(1);
}

void reduceState(){
    if(debugStatus){
        printf("[+] stateStack: ");
        dumpVector(stateStack);

        printf("[+] symbolsStack: ");
        dumpVector(symbolsStack);
    }

    int nextState = atoi(parseTable[stateStack.back()][symbolsStack.back()].c_str());
    stateStack.push_back(nextState);
}

void applyReduceRule(int ruleNumber, char* value){
    debugLog("Starting reduction");
    // Copy vector in case assignment is by address
    vector<int> currRule(reducingRules[ruleNumber]);
    debugLog("Vector copy created");

    int reduceCount = currRule.size() - 1;

    if(debugStatus){
        printf("[+] Popping %d states and input.\n", reduceCount);
    }

    while(reduceCount--){
        int symbolTop, ruleTop, lastState;
        // Pop states
        lastState = stateStack.back();
        stateStack.pop_back();

        // Pop symbols and match with reduction rule        
        symbolTop = symbolsStack.back();
        symbolsStack.pop_back();

        ruleTop = currRule.back();
        currRule.pop_back();

        if(ruleTop == getTokenNum("")){ // Nullable symbol
            // Put the symbol back
            // And append nullable symbol at top of symbols stack
            // ^ done after while rule
            symbolsStack.push_back(symbolTop);
            // Also put the state back
            stateStack.push_back(lastState);
            break;
        }
        if(symbolTop != ruleTop){
            printf("Error in reducing.");
            exit(1);
        }
    }
    debugLog("Popping done");

    symbolsStack.push_back(currRule[0]);
    debugLog("Reduced symbol inserted");
    

    reduceState();
    debugLog("States updated");

    // Call hanldeInpupt again with same value. Because yylex() will shift to next input
    // And in reducing input is not consumed;
    // NOTE: Possibility of infinite loop . Check here if TLE occurs
    debugLog("Calling handleInput again");
    handleInput(value);
}

void changeState(int tokenNum, int newState){
    stateStack.push_back(newState);
    symbolsStack.push_back(tokenNum);
}

void handleInput(char* value){// At index 1
    debugLog("Handle input called with :");
    debugLog(value);

    int currState = stateStack.back();
    int tokenNum = getTokenNum(value);
    string temp = parseTable[currState][tokenNum];
    char* nextAction = (char *)temp.c_str();

    debugLog("Next Action:");
    debugLog(nextAction);

    if(nextAction[0] == 'r'){
        debugLog("Reducing...");
        nextAction++;
        int reductionRule = atoi(nextAction);
        
        if(debugStatus){
            printf("[+] Reducing using rule %d.\n", reductionRule);
        }
        
        applyReduceRule(reductionRule, value); // Pass 'value' for passback
    }
    else if(nextAction[0] == 's'){
        debugLog("Changing state");
        nextAction++;
        changeState(tokenNum, atoi(nextAction));
    }
    else if(string(nextAction) == "acc"){
        printf("Accepted\n");
        exit(0);
    }
    else{
        printf("Error: No action found\n");
        exit(1);
    }

    debugLog("handleInput done!");
}

void convertRulesToNums(map<int, vector<string>> &rules){
    debugLog("Converting rules...");
    for(const auto &pair: rules){
        vector<int> tmp;
        for(int i=0;i<pair.second.size();i++)
            tmp.push_back(getTokenNum((char*)pair.second.at(i).c_str()));
        
        reducingRules[pair.first] = vector<int>(tmp);
    }
    debugLog("Rules converted");
}

void prepareRules(){
    debugLog("Preparing rules");

    map<int, vector_string> tempReducingRules;
    // W' -> W
    tempReducingRules[0] = vector_string{"W'", "W"};
    // W -> while C begin S end
    tempReducingRules[1] = vector_string{"W", "while", "C", "begin", "S", "end"};
    // C -> lparn I rop I rparn
    tempReducingRules[2] = vector_string{"C", "lparn", "I", "rop", "I", "rparn"};
    // S -> P semi S
    tempReducingRules[3] = vector_string{"S", "P", "semi", "S"};
    // S -> ''
    tempReducingRules[4] = vector_string{"S", ""};
    // P -> id aop E
    tempReducingRules[5] = vector_string{"P", "id", "aop", "E"};
    // E -> I mop I
    tempReducingRules[6] = vector_string{"E", "I", "mop", "I"};
    // E -> I sub I
    tempReducingRules[7] = vector_string{"E", "I", "sub", "I"};
    // E -> I
    tempReducingRules[8] = vector_string{"E", "I"};
    // I -> id
    tempReducingRules[9] = vector_string{"I", "id"};
    // I -> num
    tempReducingRules[10] = vector_string{"I", "num"};
    // I -> sub I
    tempReducingRules[11] = vector_string{"I", "sub", "I"};

    generateNumsFromRules(tempReducingRules);
    convertRulesToNums(tempReducingRules);
    debugLog("Rules prepared");
}

/* The trick is to define yywrap in lex file itself */
// int yywrap(){
//     return 1;
// }

int main(int argc, char** argv) {
    if(argc != 2){
        printf("Usage: %s <file-path>\n",argv[0]);
        exit(1);
    }

    prepareRules();
    stateStack.push_back(0); // Push intial state;

    debugLog("Opening file");
    yyin = fopen(argv[1], "r");
    debugLog("File opened");

    debugLog("Parsing started");
    yylex();
    debugLog("Parsing done");
}