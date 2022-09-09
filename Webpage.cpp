#include "Webpage.hpp"

Webpage :: Webpage(string Name){
    name = Name;
}

void Webpage :: updateClicks(){
    clicks++;
}

void Webpage :: updateImpressions(){
    impressions++;
}

void Webpage :: add_keyword(string name){
    keywords.push_back(name);
}

void Webpage :: setPageRank(double PageRank){
    pageRank = PageRank;
}

void Webpage :: setImpressions(int Impressions){
    impressions = Impressions;
}

void Webpage :: setClicks(int Clicks){
    clicks = Clicks;
}

void Webpage :: setCtr(){
    ctr = double(clicks) / impressions;
}

double Webpage :: getCtr(){
    setCtr();
    return ctr;
}

void Webpage :: setScore(double Score){
    score = Score;
}

void Webpage :: print_keywords(){
    for(int i = 0; i < keywords.size(); i++){
        cout << keywords[i] << " ";
    }
    cout << endl;
}

vector <string> Webpage :: getKeywords(){
    return keywords;
}

string Webpage ::  getName(){       return name; }
int Webpage :: getImpressions(){    return impressions;}
int Webpage :: getClicks(){         return clicks;}
double Webpage :: getPageRank(){    return pageRank;}
double Webpage :: getScore(){       return score;}

Webpage::~Webpage(){
	
}