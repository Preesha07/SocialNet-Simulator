#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include <sstream>

#include "SocialNet.h"
#include "AVL_tree.h"
// file for the main program: a socialnet class instantiated, io parsed, functions called, ouptut display. 
using namespace std;

string normalize(std::string s) 
{
    transform(s.begin(), s.end(), s.begin(),[](unsigned char c) {return tolower(c);});
    return s;
}

int main()
{
    SocialNet net1;
    string line;
    time_t timestamp;
    cout<<"Welcome!\n";
    while(getline(cin,line))
    {
        stringstream parser(line);
        string op;
        parser>>op;
        
        if (op=="EXIT")
        {
            cout<<"Exiting program\n"<<"Bye!";
            break;
        }
        else if(op=="ADD_USER")
        {
            string username;
            parser>>username;
            username=normalize(username);
            {
                net1.ADD_USER(username);
            }       
        }
        else if(op=="ADD_FRIEND")
        {
            string name1;
            string name2;
            parser>>name1;
            parser>>name2;
            name1=normalize(name1);
            name2=normalize(name2);
            net1.ADD_FRIENDS(name1,name2);
        }
        else if(op=="LIST_FRIENDS")
        {
            string name;
            parser>>name;
            name=normalize(name);
            net1.LIST_FRIENDS(name);
        }
        else if(op=="SUGGEST_FRIENDS")
        {
            string name, N;
            parser>>name;
            parser>>N;
            name=normalize(name);
            net1.SUGGEST_FRIENDS(name, stoi(N));
        }
        else if(op=="DEGREES_OF_SEPARATION")
        {
            string name1;
            string name2;
            parser>>name1;
            parser>>name2;
            name1=normalize(name1);
            name2=normalize(name2);
            cout<<net1.DEGREES_OF_SEPARATION(name1, name2)<<"\n";
        }
        else if(op=="ADD_POST")
        {
            string name;
            string content;
            parser>>name;
            name=normalize(name);
            getline(parser,content);
            if(content==" ")
            {}
            else if (!content.empty() && content[0] == ' ') //empty posts unaltered.
            {
            content = content.substr(1);
            }
            content=normalize(content);
            timestamp=time(nullptr);
            net1.ADD_POST(name,content,timestamp);
        }
        else if(op=="OUTPUT_POSTS")
        {
            string name,N;
            parser>>name;
            parser>>N;
            net1.OUTPUT_POSTS(name,stoi(N));
        }
        else
        {
            if (!op.empty()) 
            {
            cout << "Error: Unknown command " << op << "\n";
            }  
    }
    }
    return 0;
}
