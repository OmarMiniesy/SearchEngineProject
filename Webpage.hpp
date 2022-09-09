#ifndef WEBPAGE_H
#define WEBPAGE_H
using namespace std;

class Webpage  
{
	private:

		string name = "";
		int impressions = 0;	
		int clicks = 0;			
		double pageRank = 0.0;	 
		double ctr = 0.0;		
		double score = 0.0;		
		vector <string> keywords;

	public:

		Webpage(string Name);
		
		void updateClicks();
		void updateImpressions();

		void setClicks(int Clicks);
		void setImpressions(int Impressions);
		void setPageRank(double pageRank);
		void setCtr();
		void setScore(double Score);

		string getName();
		int getImpressions();
		int getClicks();
		double getPageRank();
		double getCtr();
		double getScore();
		vector <string> getKeywords();

		void add_keyword(string name);
		void print_keywords();
		~Webpage();

};

#endif