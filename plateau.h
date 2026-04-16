#ifndef PLATEAU_H
#define PLATEAU_H

class plateau
{
	public:
		plateau();
		~plateau();
		init_Plateau();
		int get_Piece(int i,int j);
		mov_Piece(int ld,int cd,int la,int ca);
		int check_arival_space(int ld,int cd,int la,int ca);
		int evaluation(int ld,int cd,int la,int ca);
		int eval_pion(int ld,int cd,int la,int ca);
		int eval_cavalier(int ld,int cd,int la,int ca);
		int eval_tour(int ld,int cd,int la,int ca);
		int eval_fou(int ld,int cd,int la,int ca);
		int eval_reine(int ld,int cd,int la,int ca);
		
	private:
		int ech[8][8];
};
 #endif