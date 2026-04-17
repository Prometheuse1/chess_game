#ifndef PLATEAU_H
#define PLATEAU_H
#include <vector>

using namespace std;

struct predict_move{
	int ld,cd,la,ca,poid;
};

class plateau
{
	public:
		plateau();
		~plateau();
		init_Plateau();
		int get_Tour();
		void scanner_Plateau_IA();
		void deplacer_IA();
		void evalMovIA();
		int fin_Partie();
		int get_Piece(int i,int j);
		mov_Piece(int ld,int cd,int la,int ca);
		int check_arival_space(int ld,int cd,int la,int ca);
		int evaluation(int ld,int cd,int la,int ca);
		int eval_pion(int ld,int cd,int la,int ca);
		int eval_cavalier(int ld,int cd,int la,int ca);
		int eval_tour(int ld,int cd,int la,int ca);
		int eval_fou(int ld,int cd,int la,int ca);
		int eval_dame(int ld,int cd,int la,int ca);
		int eval_roi(int ld,int cd,int la,int ca);
		
	private:
		int ech[8][8];
		int tour;
		predict_move t[10];
		int cp;
};

#endif
