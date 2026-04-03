#include "plateau.h"

plateau::plateau()
{
	initPlateau();
	//TODO
}
plateau::~plateau()
{
	
}

int plateau::check_arival_space(int ld,int cd,int la,int ca)
{
	if((ech[ld][cd]>0 && ech[la][ca]<0) ||	(ech[ld][cd]<0 && ech[la][ca]>0) || ech[la][ca]==0)
	{
		return 1;
	}
	return 0;
}

int plateau::evalpion(int ld,int cd,int la,int ca)
{
	//TODO
}

int plateau::evalcavalier(int ld,int cd,int la,int ca)
{
	if( ((ld+2==la)&&(cd+1==ca)) || ((ld+2==la)&&(cd-1==ca)) || ((ld+1==la)&&(cd+2==ca)) || ((ld-1==la)&&(cd+2==ca)) 
	|| ((ld-2==la)&&(cd+1==ca)) || ((ld-2==la)&&(cd-1==ca)) || ((ld+1==la)&&(cd-2==ca)) || ((ld-1==la)&&(cd-2==ca)) )
	{
		return check_arival_space(ld,cd,la,ca);
	}
	else
	{
		return 0;
	}
}

int plateau::evaltour(int ld,int cd,int la,int ca)
{
	bool cheminvide;
	int i;
	if(ld==la)
	{
		cheminvide=true;
		if(cd<ca)
		{
			i=cd+1;
			while(i<ca && cheminvide==true)
			{
				if(ech[ld][i]!=0)
				{
					cheminvide=false;
				}
				i++;
			}
		}
		else if(cd>ca)
		{
			i=cd-1;
			while(i>ca && cheminvide==true)
			{
				if(ech[ld][i]!=0)
				{
					cheminvide=false;
				}
				i--;
			}
		}	
	}
	if(cd==ca)
	{
		cheminvide=true;
		if(ld<la)
		{
			i=ld+1;
			while(i<la && cheminvide==true)
			{
				if(ech[i][cd]!=0)
				{
					cheminvide=false;
				}
				i++;
			}
		}
		if(ld>la)
		{
			i=ld-1;
			while(i>la && cheminvide==true)
			{
				if(ech[i][cd]!=0)
				{
					cheminvide=false;
				}
				i--;
			}
		}
	}
	
	if(cheminvide==true)
	{
		return check_arival_space(ld,cd,la,ca);
	}
	else 
	{
		return 0;
	}
}

int plateau::evalfou(int ld,int cd,int la,int ca)
{
	bool cheminvide;
	int i;
	if(ld!=la)
	{
		cheminvide=true;
		if(cd<ca)
		{
			i=cd+1;
			while(i<ca && cheminvide==true)
			{
				if(ech[ld][i]!=0)
				{
					cheminvide=false;
				}
				i++;
			}
		}
		else if(cd>ca)
		{
			i=cd-1;
			while(i>ca && cheminvide==true)
			{
				if(ech[ld][i]!=0)
				{
					cheminvide=false;
				}
				i--;
			}
		}	
	}
	if(cd==ca)
	{
		cheminvide=true;
		if(ld<la)
		{
			i=ld+1;
			while(i<la && cheminvide==true)
			{
				if(ech[i][cd]!=0)
				{
					cheminvide=false;
				}
				i++;
			}
		}
		else if(cd>ca)
		{
			i=ld-1;
			while(i>la && cheminvide==true)
			{
				if(ech[i][cd]!=0)
				{
					cheminvide=false;
				}
				i--;
			}
		}
	}
	
	if(cheminvide==true)
	{
		return check_arival_space(ld,cd,la,ca);
	}
	else 
	{
		return 0;
	}
}

int plateau::evaluation(int ld,int cd,int la,int ca)
{
	switch(ech[ld][cd])
	{
		case 1: return evalpion(ld,cd,la,ca);
			break;
		case -1: return evalpion(ld,cd,la,ca);
			break;
		case 2: return evaltour(ld,cd,la,ca);
			break;
		case -2: return evaltour(ld,cd,la,ca);
			break;
		case 3: return evalcavalier(ld,cd,la,ca);
			break;
		case -3: return evalcavalier(ld,cd,la,ca);
			break;
		case 4: return evalfou(ld,cd,la,ca);
			break;
		case -4: return evalfou(ld,cd,la,ca);
			break;
		default:
			return 1;
			break;
	}
}


plateau::movPiece(int ld,int cd,int la,int ca)
{
	if(evaluation(ld,cd,la,ca)==1)
	{
		ech[la][ca]=ech[ld][cd];
		ech[ld][cd]=0;		
	}
	else
	{
		//TODO
	}
}


int plateau::getPiece(int i,int j)
{
	return ech[i][j];
}

plateau::initPlateau()
{
	for(int i=2;i<=5;i++)
	{
		for(int j=0;j<=7;j++)
		{
			ech[i][j]=0; // Position Vide
		}
	}
	
	for(int j=0;j<=7;j++)
	{
		ech[1][j]=1; // Pion Blanc
		ech[6][j]=-1;// Pion Noir
	}
	
	ech[0][0]=2; // Tour
	ech[0][1]=3; // Cavalier
	ech[0][2]=4; // Fou
	ech[0][3]=6; // Roi
	ech[0][4]=5; // Reine/Dame
	ech[0][5]=4; // Fou
	ech[0][6]=3; // Cavalier
	ech[0][7]=2; // Tour

	ech[7][0]=-2; // Tour
	ech[7][1]=-3; // Cavalier
	ech[7][2]=-4; // Fou
	ech[7][3]=-6; // Roi
	ech[7][4]=-5; // Reine/Dame
	ech[7][5]=-4; // Fou
	ech[7][6]=-3; // Cavalier
	ech[7][7]=-2; // Tour
		
	
}
