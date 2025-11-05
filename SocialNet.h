#pragma once

#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>

#include "AVL_tree.h"

struct comparator
{
    bool operator() (const std::pair<int,std:: string>&a, const std::pair<int, std::string>&b) const{
        if(a.first==b.first)
        return a.second>b.second;
        return a.first<b.first;
    }
};

int ninter (const int& id2, const int& id1, const std::vector <std::set<std::string>> &friends)
{
    auto i=friends[id2].begin();
    auto j=friends[id1].begin();
    int ans=0;
    while(i!=friends[id2].end() && j!=friends[id1].end())
    {
        if(*i==*j)
        {
            i++;
            j++;
            ans++;
        }
        else if(*i<*j)
        {
            i++;
        }
        else
        {
            j++;
        }
    }
    return ans;
}

class SocialNet
{
    private:
    std::vector <std::set<std::string>> friends;
    std::unordered_map <std::string, int> index;
    std::unordered_map <std::string, AVLTree> posts;
    int nusers=-1;
    public:
    SocialNet()
    {
    };

    ~SocialNet()=default;
    
    void ADD_USER(std::string name)
    {
        nusers+=1;
        auto [it, inserted] = index.insert({name,nusers});
        if (!inserted) {
            std::cout << "Error: User \"" << name << "\" already exists.\n";
            return;
        }
        friends.emplace_back();
        posts.insert({name,AVLTree()});
    };

    void ADD_FRIENDS(std::string name1, std::string name2)//name1 and name2 must exist
    {
        try
        {
            if(name1==name2)
            {
                std::cout<<"Illegal operation\n";
                return;
            }
            int id1=index.at(name1);
            int id2=index.at(name2);
            if(friends.at(id1).find(name2)!=friends.at(id1).end())
            {
                std::cout<<"Already friends!\n";
                return;
            }
            else
            {
                friends.at(id1).insert(name2);
                friends.at(id2).insert(name1);
                return;
            }
        }
        catch(const std::out_of_range& e)
        {
            std::cout<<"Provide existing usernames!" << '\n';
        }
        
        
    };

    void LIST_FRIENDS(std::string name)//name DNE handled
    {
        try
        {
            int id=index.at(name);
            for(auto it: friends.at(id))
            {
                std::cout<<it<<" ";
            }
            std::cout<<"\n";
        }
        catch (const std::out_of_range& e)
        {
            std::cout<<"User does NOT exist!\n";
        }
    };

    void SUGGEST_FRIENDS( std:: string name, int N)//name DNE handled
    {
        if(N==0)
        {
            std::cout<<"\n";
            return;
        }
        try
        {
            int id=index.at(name), id2, id3;
            std:: unordered_map <std::string, bool> seen; 
            std::priority_queue <std::pair<int, std:: string>, std::vector<std::pair<int, std:: string>>, comparator> all;
            {
                for(auto i:friends.at(id))
                {
                    id2=index.at(i);
                    for(auto jt: friends.at(id2))
                    {
                        if(!seen[jt])
                        {
                            seen[jt]=true;
                            if(jt==name)//dont recommend user to themselves.
                            continue;
                            if(friends.at(id).find(jt)==friends.at(id).end())
                            {
                                id3=index.at(jt);
                                all.push({ninter(id3,id,friends),jt});//cardinality of intersection
                            }
                            else//is a friend of user, don't recommend.
                            {
                            }
                        }
                        else//already processed this guy
                        {
                            continue;
                        }
                    }

                }
            }
            // std::priority_queue <pair<int, std:: string>> findN;//currently using the klogN solution.//does the priority queue break tie
            for(int i=0;i<N;i++)
            {
                if(all.size()==0)
                break;
                std::cout<<all.top().second<<" ";
                all.pop();
            }
            std::cout<<"\n";
        }
        catch (const std::out_of_range& e)
        {
            std::cout<<"User does NOT exist!\n";
        }
    }

    int DEGREES_OF_SEPARATION(std::string name1, std::string name2)//shortest path lenth bw 2 ppl
    {
        //implement a bfs.
        int n, id, d;
        if(index.find(name1)==index.end()||index.find(name2)==index.end())
        {
            return -1;
        }
        else if(name1==name2)
        {
            return 0;
        }
        
        std::queue <std::pair<std::string, int>> bfs;

        bfs.push({name1,0});
        std::vector <bool> vis (nusers+1,false);
        vis[index.at(bfs.front().first)]=true;
        id=index.at(bfs.front().first);
        vis[id]=true;

        while(!bfs.empty())
        {
            id=index.at(bfs.front().first);
            vis[id]=true;
            d=bfs.front().second;
            n=friends.at(id).size();
            bfs.pop();

            for(auto it: friends.at(id))
            {
                if(vis[index.at(it)]==false)
                {
                    vis[index.at(it)]=true;
                    if(it==name2)
                    {
                        return d+1;
                    }
                    else
                    {
                        bfs.push({it,d+1});
                    }
                }
            }
        }
        return -1;
    }

    void ADD_POST(std::string name, std::string content, time_t t)
    {
        try
        {
            posts.at(name).insert({t, content});
            std::cout<<"POSTED SUCCESSFULLY\n";
            return;
        }
        catch(const std::out_of_range& e)
        {
            std::cout<<"User does NOT exist!\n";
        }
    }

    void OUTPUT_POSTS(std::string name, int N)
    {
        try
        {
            std::vector<std::string> rec_posts=posts.at(name).Ngreatest(N);
            for(auto it: rec_posts)
            {
                std::cout<<it<<" ";
            }
            std::cout<<"\n";
            return;
        }
        catch(const std::out_of_range& e)
        {
            std::cout<<"User does NOT exist!\n";
        }
    }
};

