#include <iostream>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include "Webpage.cpp"
using namespace std;

void heapify(vector <Webpage>& arr, int n, int i);
void build_heap(vector <Webpage>& arr, int n);
void heap_sort(vector <Webpage>& arr, int n);
void inputNodesFromFile(vector <Webpage>& nodes);   //this funcion creates a vector of all webpages.
void inputEdgesFromFile(vector <pair <string, string>>& edges, unordered_map <string, int>& edgespernode);   //this function creates a vector for all edges.
void inputImpressionsFromFile(vector <Webpage>& nodes);     //this function reads the impressions from the file.
void pageRankFunction(vector <Webpage>& nodes, unordered_map <string, int>& edgespernode, vector <pair <string, string>>& edges, unordered_map <string, double>& pageRank);
void searchFunction(vector <Webpage>& nodes, string& search, vector <Webpage>& searchResults);
void score(vector <Webpage>& nodes);
void updateImpressionFile(vector <Webpage>& nodes);
void normalizePageRank(vector <Webpage>& nodes);

int main(){

    vector <Webpage> nodes;
    inputNodesFromFile(nodes);
    inputImpressionsFromFile(nodes);
    
    int numberOfNodes = nodes.size();

    unordered_map <string, int> edgespernode;   //how many nodes each point points to.

    vector <pair <string, string>> edges;
    inputEdgesFromFile(edges, edgespernode);
    int numberOfEdges = edges.size();

    unordered_map <string, double> pageRank;
    pageRankFunction(nodes, edgespernode, edges, pageRank);

    for(auto it = nodes.begin(); it != nodes.end(); it++){
        (*it).setPageRank(pageRank[(*it).getName()]);           //each node has its pagerank now.
    }

    normalizePageRank(nodes);   //normalized the pagerank for all the nodes.

    string choice;
    string choice1;
    string choice2;
    string search;
    bool flag = true;
    bool flag1 = false;
    vector <Webpage> searchResults;

    while(flag){
        bool flag1 = false;
        cout << "Welcome!"<<endl;
        cout << "What would you like to do?"<<endl;
        cout << "1. New Search"<<endl;
        cout << "2. Exit"<<endl;
        cout << endl;
        cout << "Enter your choice: ";
        getline(cin, choice);

        if(choice == "2"){
            flag = false;
        }

        if(flag == true && choice == "1"){

            cout << "Search: ";
            getline(cin, search);
            searchFunction(nodes, search, searchResults);
            cout << endl;

            cout << "Search Results:"<<endl;
            cout << endl;

            score(searchResults);
            heap_sort(searchResults, searchResults.size());

            for(int i = 0; i < searchResults.size(); i++){
                cout << i + 1 <<". "<<searchResults[i].getName()<<endl;
                searchResults[i].updateImpressions();

                for(int j = 0; j < numberOfNodes; j++){
                    if(searchResults[i].getName() == nodes[j].getName()){
                        nodes[j].updateImpressions();
                    }
                }

            }

            cout << endl;
            while(flag && flag1 == false){
                cout << "What would you like to do? " <<endl;
                cout << "1. Choose a webpage to open" <<endl;
                cout << "2. New Search" <<endl;
                cout << "3. Exit" << endl;
                cout << endl;
                cout << "Enter your choice: ";
                getline(cin, choice);

                if(choice == "2") flag1 = true;

                else if(choice == "1"){
                    cout << "Enter the number of the website you want to open: ";
                    getline(cin, choice1);
                    cout << endl;
                    cout << "You are now viewing: "<< searchResults[stoi(choice1) - 1].getName()<<endl;
                    searchResults[stoi(choice1) - 1].updateClicks();
                    for(int j = 0; j < numberOfNodes; j++){
                        if(searchResults[stoi(choice1) - 1].getName() == nodes[j].getName()){
                            nodes[j].updateClicks();
                        }
                    }
                    
                    cout << endl;
                    cout <<"Would you like to"<<endl;
                    cout <<"1. Back to search results"<<endl;
                    cout <<"2. New Search"<<endl;
                    cout <<"3. Exit"<<endl;
                    getline(cin, choice2);

                    if(choice2 == "1")  continue;
                    else if (choice2 == "2") flag1 = true;
                    else if (choice == "3") flag = false;
                }

                else if(choice == "3"){
                    flag = false;
                }
            }

        }
        

        searchResults.clear();
        
    }

    updateImpressionFile(nodes);
    system("pause");
    
}

void normalizePageRank(vector <Webpage>& nodes){

    double max = nodes[0].getPageRank();
    double temp = 0;

    for(int i = 1; i < nodes.size(); i++){
        if(nodes[i].getPageRank() > max)
            {max = nodes[i].getPageRank();}
    }

    for(int i = 0; i < nodes.size(); i++){
        temp = nodes[i].getPageRank();
        nodes[i].setPageRank(temp/max);
    }
}


void updateImpressionFile(vector <Webpage>& nodes){
    
    fstream updateFiles;
    updateFiles.open("newnumofimpressions.csv");

    if(updateFiles.fail()){
        updateFiles.close();
        remove("numofimpressionsfile.csv"); 
        ofstream update;
        update.open("newnumofimpressions.csv");
        for(int i = 0; i < nodes.size(); i++){
            update << nodes[i].getName() <<","<<nodes[i].getImpressions()<<","<<nodes[i].getClicks()<<"\n";
        }
        update.close();
    }

    else{
        
        updateFiles.close();
        remove("newnumofimpressions.csv");
        ofstream update;
        update.open("newnumofimpressions.csv");
        for(int i = 0; i < nodes.size(); i++){
            update << nodes[i].getName() <<","<<nodes[i].getImpressions()<<","<<nodes[i].getClicks()<<"\n";
        }
        update.close();

    }

}


void score(vector <Webpage>& nodes){    //nodes here is not the main vector, but it is searchresults.

    double a;
    double b;
    double c;

    for(int i = 0; i < nodes.size(); i++){
        a = (0.1 * nodes[i].getImpressions()) / (1 +(0.1 * nodes[i].getImpressions()));
        a = (1 - a) * nodes[i].getPageRank();
        b = (0.1 * nodes[i].getImpressions()) / (1 +(0.1 * nodes[i].getImpressions()));
        b = b * nodes[i].getCtr(); 
        c = a + b;
        c = c * 0.6;
        c = c + (0.4 * nodes[i].getPageRank());
        nodes[i].setScore(c);
    }
}



void searchFunction(vector <Webpage>& nodes, string& search, vector <Webpage>& searchResults){

    vector <string> keywords;
    string x;
    string y;
    bool flag1;
    bool flag2;

    if(search.find("AND") != search.npos){

        x = search.substr(0, search.find(" AND"));
        y = search.substr(search.find("AND") + 4, search.length() - search.find("AND") + 4);


        for(int i = 0; i < nodes.size(); i++){

            keywords = nodes[i].getKeywords();
            flag1 = false;
            flag2 = false;

            for(int j = 0; j < keywords.size(); j++){
                if(x == keywords[j]) flag1 = true;
                if(y == keywords[j]) flag2 = true;
                if(flag1 && flag2) break;
            }

            if(flag1 && flag2){
                searchResults.push_back(nodes[i]);
            }
        }

    }
    else if (search.find("OR") != search.npos){
        x = search.substr(0, search.find(" OR"));
        y = search.substr(search.find("OR") + 3, search.length() - search.find("OR") + 3);


        for(int i = 0; i < nodes.size(); i++){

            keywords = nodes[i].getKeywords();
            flag1 = false;
            flag2 = false;

            for(int j = 0; j < keywords.size(); j++){
                if(x == keywords[j]) flag1 = true;
                if(y == keywords[j]) flag2 = true;
                if(flag1 || flag2) break;
            }

            if(flag1 || flag2){
                searchResults.push_back(nodes[i]);
            }
        }
    }
    else if(search[0] == '\"' && search[search.length() - 1] == '\"'){
        x = search.substr(1, search.length() - 2);

        for(int i = 0; i < nodes.size(); i++){ 

            keywords = nodes[i].getKeywords();

            for(int j = 0; j < keywords.size(); j++){
                if(x == keywords[j]) {
                    searchResults.push_back(nodes[i]);
                    break;
                }
            }

        }

        
        
    }
    else{

        if(search.find(" ") != search.npos){
            x = search.substr(0, search.find(" "));
            y = search.substr(search.find(" ") + 1, search.length() - search.find(" ") + 1);
            
            for(int i = 0; i < nodes.size(); i++){

                keywords = nodes[i].getKeywords();
                flag1 = false;
                flag2 = false;

                for(int j = 0; j < keywords.size(); j++){
                    if(x == keywords[j]) flag1 = true;
                    if(y == keywords[j]) flag2 = true;
                    if(flag1 || flag2) break;
                }

                if(flag1 || flag2){
                    searchResults.push_back(nodes[i]);
                }
            }
        }
        else{
            
            x = search;

            for(int i = 0; i < nodes.size(); i++){

                keywords = nodes[i].getKeywords();

                for(int j = 0; j < keywords.size(); j++){
                    if(x == keywords[j]) {
                        searchResults.push_back(nodes[i]);
                        break;
                    }
                }

            }
        }
    }

}


void pageRankFunction(vector <Webpage>& nodes, unordered_map <string, int>& edgespernode, vector <pair <string, string>>& edges, unordered_map <string, double>& pageRank){
    
    unordered_map <string, double> temp;
    unordered_map <string, double> temp1;

    for(int i = 0; i < nodes.size(); i++){
        pageRank.insert({nodes[i].getName(), 1.0/nodes.size()});
        temp.insert({nodes[i].getName(), 0.0});
        temp1.insert({nodes[i].getName(), 0.0});
    }

    double temp_pageRank = 0.0;
    string pointer = "";
    double eshta = 0.0;
    int counter = 0;

    for(int i = 0; i <100; i++){

        for(int i = 0; i < nodes.size(); i++){
            
            Webpage x = nodes[i];
            counter = 0;

            for(auto it = edges.begin(); it != edges.end(); it++){
                
                if(it->second == x.getName()){
                    pointer = it->first;
                    temp_pageRank = pageRank[pointer];
                    temp_pageRank = temp_pageRank / edgespernode[pointer];
                    eshta = (0.15/nodes.size()) + 0.85*temp_pageRank;
                    temp[x.getName()] += eshta;
                }else{
                    counter ++;
                }
            }
            if(counter >= edges.size()){

                 //nothing points to this node, so we set its pagerank to be 0.15.
                 temp[x.getName()]+=(0.15 / nodes.size());

            }


        }

        pageRank = temp;
        temp = temp1;

    }
}

void inputNodesFromFile(vector <Webpage>& nodes){ 
   
    ifstream inputNodes;
    inputNodes.open("keywordfile.csv");
    if(inputNodes.fail()){cout << "a7a"<<endl;}
    string temp = "";
    string line = "";
    string word = "";
    int i = 0;

    while(getline(inputNodes, temp, ',')){

        nodes.push_back(Webpage(temp));
        getline(inputNodes, line, '\n');

        while(line.find_first_of(',') != -1){
            int index = line.find_first_of(',');
            word = line.substr(0, index);
            line = line.substr(index+1, line.length() - index);
            nodes[i].add_keyword(word);
        }

        nodes[i].add_keyword(line);
        i++;

    }

    inputNodes.close();

}

void inputEdgesFromFile(vector <pair <string, string>>& edges, unordered_map <string, int>& edgespernode){

    ifstream inputEdges;
    inputEdges.open("webgraphfile.csv");

    string x = "";

    while(getline(inputEdges, x, ',')){

        string y = ""; getline(inputEdges, y, '\n');

        edges.push_back(make_pair(x, y));
        
        auto it = edgespernode.find(x);

        if(it == edgespernode.end()){
            edgespernode.insert({x, 1});
        }
        else{
            it->second ++;
        }
    }

    inputEdges.close();
}

void inputImpressionsFromFile(vector <Webpage>& nodes){

    ifstream inputImpressions;
    inputImpressions.open("numofimpressionsfile.csv");
    string x = "";
    int y = 0;
    int a = 0;
    int z = 0;
    string eh = "";
    int i = 0;

    if(inputImpressions.fail()){
        inputImpressions.open("newnumofimpressions.csv");
        while(getline(inputImpressions, x, ',')){
            
            getline(inputImpressions, eh, '\n');

            while(eh.find_first_of(',') != -1){
                a = eh.find_first_of(',');
                y = stoi(eh.substr(0, a));
                eh = eh.substr(a+1, eh.length() - a);
                a = eh.find_first_of('\n');
                z = stoi(eh.substr(0, a));
                nodes.at(i).setImpressions(y);
                nodes.at(i).setClicks(z);
                if( i < nodes.size()) i++;
            }
            
        }
    }
    else{

        while(getline(inputImpressions, x, ',')){
            inputImpressions>>y;
            nodes.at(i).setImpressions(y);
            if( i < nodes.size()) i++;
        }

    }

    inputImpressions.close();

}

void heapify(vector <Webpage>& arr, int n, int i) 
{ 
    int largest = i; 
    int l = 2 * i + 1; 
    int r = 2 * i + 2;

    if (l < n && arr[l].getScore() < arr[largest].getScore()) 
        largest = l; 
  
    if (r < n && arr[r].getScore() < arr[largest].getScore()) 
        largest = r; 

    if (largest != i) { 
        swap(arr[i], arr[largest]); 
        heapify(arr, n, largest); 
    } 
}

void build_heap(vector <Webpage>& arr, int n)
{
    for (int i = n / 2 - 1; i >= 0; i--) 
        heapify(arr, n, i);
}
  
void heap_sort(vector <Webpage>& arr, int n) 
{
    build_heap(arr, n);
     
    for (int i = n - 1; i >= 0; i--) { 
        swap(arr[0], arr[i]); 
        heapify(arr, i, 0); 
    } 
} 