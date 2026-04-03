#ifndef PLATEAU_H
#define PLATEAU_H

class plateau
{
	public:
		plateau();
		~plateau();
		initPlateau();
		int getPiece(int i,int j);
		movPiece(int ld,int cd,int la,int ca);
		int check_arival_space(int ld,int cd,int la,int ca);
		int evaluation(int ld,int cd,int la,int ca);
		int evalpion(int ld,int cd,int la,int ca);
		int evalcavalier(int ld,int cd,int la,int ca);
		int evaltour(int ld,int cd,int la,int ca);
		int evalfou(int ld,int cd,int la,int ca);
		
	private:
		int ech[8][8];
};

#endif
