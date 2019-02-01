#include <iostream>
#include <string>
#include <fstream>

using namespace std;

bool isDelim(char x){
    string delim = "\t\n +/-*;(){}[],<>=?:^%'\"";
    return delim.find(x) != string::npos;
}

bool isReservedKeyword(string token){
    if( token=="if" || token=="else"
    ||  token=="int" || token=="string"
    ||  token=="bool" || token=="void"
    ||  token=="return" || token=="while"
    ||  token=="for" || token=="do"){
        return true;
    }
    return false;
}

bool isInt(string token){
    for(int i=0;i<token.length();i++){
        if((int)token.at(i)<48 || (int)token.at(i)>57)
            return false;
    };

    return true;
}

bool isFloat(string token){
    int dot = 0;
    for(int i=0;i<token.length();i++){
        if((int)token.at(i)<48 || (int)token.at(i)>57){
            if(token.at(i) == '.'){
                dot++;
                if(dot>1)
                    return false;
            }
            else{
                return false;
            }
        }
    };

    return true;
}

bool startsWithValidChar(char token){
    string InvalidChars = "1234567890.";
    size_t pos = InvalidChars.find(token);
    return pos==string::npos;
}

void printType(string token){
    if(isReservedKeyword(token)){
        printf("'%s' is reserved keyword\n",&token[0]);
    }
    else if(isInt(token)){
        printf("'%s' is integer\n",&token[0]);
    }
    else if(isFloat(token)){
        printf("'%s' is float number\n",&token[0]);
    }
    else if(startsWithValidChar(token.at(0))){
        printf("'%s' is valid identifier\n",&token[0]);
    }
    else{
        printf("'%s' is UNKOWN\n",&token[0]);
    }
}

void parse(string code){
    size_t index = 0;

    string data = "";
    while(index < code.length()){
        if(!isDelim(code.at(index))){
            data.push_back(code.at(index));
        }else{
            // Check if current delim is comments
            if(code.substr(index, 2) == "//"){
                size_t newlinePos = code.find('\n',index);
                // Following code to stop infinite while loop
                if(newlinePos == string::npos){
                    break;
                }
                index = newlinePos; // After index++ it will point at new delim
            }

            if(data.length()>0)
                printType(data);
            data = "";
        }
        index++;
    }

    // Handle last data in unideal case
    // Ideal case always ends with a delim e.g. } or ;
    if(data.length()>0)
        printType(data);
}

string readFromFile(char* filename){
    ifstream inputFile(filename);
    
    if(inputFile.is_open()){
        string code = "";
        string line;
        while(getline(inputFile, line)){
            code.append(line);
            code.append("\n");
        }

        inputFile.close();
        return code;
    }
    else{
        exit(1);
    }
}

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Usage: %s <file-name>", argv[0]);
        return 1;
    }
    printf("Reading from file: %s\n", argv[1]);
    
    string code = readFromFile(argv[1]);
    printf("**************\n%s\n**************\n",&code[0]);
    parse(code);
    return 0;
}