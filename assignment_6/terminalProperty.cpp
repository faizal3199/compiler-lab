#include <iostream>
#include <string>
#include <map>

using namespace std;

class Base{};

template<typename T>
class Property: public Base{
    public:
    T value;

    Property(T x){
        value = x;
    }
};

class Variable{
    private:
    map<string, Base*> props;

    public:
    Variable(){};

    template<typename T>
    void add(string name,T value){
        props[name] = (Base*) new Property<T>(value);
    }

    template<typename T>
    T get(string name){
        auto index = props.find(name);

        if(index == props.end()){
            // throw error()
            throw invalid_argument("Parameter '"+name+"' is not defined.");
        }

        Property<T>* temp = (Property<T>*)index->second;

        return temp->value;
    }
};
