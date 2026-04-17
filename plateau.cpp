#include "plateau.h"
#include "math.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
plateau::plateau()
{
	init_Plateau();
	cp=0;
	//TODO
}

plateau::~plateau()
{
	
}

int plateau::fin_Partie(){
	return 0;
}

int plateau::get_Tour(){
	return tour;
}

void plateau::scanner_Plateau_IA()
{
	int rep;
	for(int i=0;i<8;i++)
	{
		for(int j=0;j<=7;j++)
		{
			if(ech[i][j]<0)
			{
				for(int k=0;k<=7;k++)
				{
					for(int l=0;l<=7;l++)
					{
						if(ech[k][l]>=0)
						{
							switch(ech[i][j])
							{
								case -1  : rep=eval_pion(i,j,k,l);     break;
								case -2  : rep=eval_tour(i,j,k,l);     break;
								case -3  : rep=eval_cavalier(i,j,k,l); break;
								case -4  : rep=eval_fou(i,j,k,l);      break;
								case -5  : rep=eval_dame(i,j,k,l);     break;
								case -6  : rep=eval_roi(i,j,k,l);      break;
								default  :                             break;
							}
							if(rep==1)
							{
								t[cp].ld=i;	
								t[cp].cd=j;	
								t[cp].la=k;	
								t[cp].ca=l;	
								t[cp].poid=0;	

								cp++;
							}							
						}
					}
				}
			}
		}
	}
}

void plateau::evalMovIA()
{
	int val,cpMp=0;
	predict_move tempcpMp;

	for(int i=0;i<cp;i++)
	{
		switch(ech[t[i].la][t[i].ca])
		{
			case 0 : break;
			case 1 : t[i].poid+=5; break;
			case 2 : t[i].poid+=15; break;
			case 3 : t[i].poid+=10; break;
			case 4 : t[i].poid+=20; break;
			case 5 : t[i].poid+=50; break;
			case 6 : t[i].poid+=1000; break; // Impossible Echec et mat !!
			default : break;
		}
	}
	for(int i=0;i<cp;i++)
	{
		for(int j=i+1;j<cp;j++)
		{
			if(t[i].poid < t[j].poid)
			{
				predict_move temp;
				temp=t[i];
				t[i]=t[j];
				t[j]=temp;
			}
		}
	}
	
	// si plusieurs poids similaires (meilleur mouv possible)
	for(int j=0;j<cp;j++)
	{
		if(t[0].poid == t[j].poid)
		{
			cpMp++;
		}
		else
		{
			j=cp;
		}
	}
	//..................
	srand(time(NULL));
	val=rand()%cpMp;
	tempcpMp=t[0];
	t[0]=t[val];
	t[val]=tempcpMp;	
}

void plateau::deplacer_IA()
{
	cp=0;
	scanner_Plateau_IA();
	evalMovIA();
	ech[t[0].la][t[0].ca]=ech[t[0].ld][t[0].cd];
	ech[t[0].ld][t[0].cd]=0;	
	tour++;		
}

int plateau::check_arival_space(int ld,int cd,int la,int ca)
{
	if((ech[ld][cd]>0 && ech[la][ca]<0) ||	(ech[ld][cd]<0 && ech[la][ca]>0) || ech[la][ca]==0)
	{
		return 1;
	}
	
	return 0;
}

int plateau::eval_pion(int ld,int cd,int la,int ca)
{
	if(ech[ld][cd]==1)
	{

		if(la==ld+1 && ca==cd && ech[la][ca]==0)
            return 1;

        if(ld==1 && la==ld+2 && ca==cd && ech[ld+1][cd]==0 && ech[la][ca]==0)
        	return 1;

        if(la==ld+1 && (ca==cd+1 || ca==cd-1) && ech[la][ca]<0)
        	return 1;
	}
	
	if(ech[ld][cd]==-1)
	{
		if(la==ld-1 && ca==cd && ech[la][ca]==0)
            return 1;

        if(ld==6 && la==ld-2 && ca==cd && ech[ld-1][cd]==0 && ech[la][ca]==0)
        	return 1;
        
        if(la==ld-1 && (ca==cd+1 || ca==cd-1) && ech[la][ca]>0)
        	return 1;
	}
	
	return 0;
}

int plateau::eval_cavalier(int ld,int cd,int la,int ca)
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

int plateau::eval_tour(int ld,int cd,int la,int ca)
{
	bool chemin_vide;
	int i;
	if(ld==la)
	{
		chemin_vide=true;
		if(cd<ca)
		{
			i=cd+1;
			while(i<ca && chemin_vide==true)
			{
				if(ech[ld][i]!=0)
				{
					chemin_vide=false;
				}
				i++;
			}
		}
		else if(cd>ca)
		{
			i=cd-1;
			while(i>ca && chemin_vide==true)
			{
				if(ech[ld][i]!=0)
				{
					chemin_vide=false;
				}
				i--;
			}
		}	
	}
	if(cd==ca)
	{
		chemin_vide=true;
		if(ld<la)
		{
			i=ld+1;
			while(i<la && chemin_vide==true)
			{
				if(ech[i][cd]!=0)
				{
					chemin_vide=false;
				}
				i++;
			}
		}
		if(ld>la)
		{
			i=ld-1;
			while(i>la && chemin_vide==true)
			{
				if(ech[i][cd]!=0)
				{
					chemin_vide=false;
				}
				i--;
			}
		}
	}
	
	if(chemin_vide==true)
	{
		return check_arival_space(ld,cd,la,ca);
	}
	else 
	{
		return 0;
	}
}

int plateau::eval_fou(int ld,int cd,int la,int ca)
{
    bool chemin_vide=false,strait_line=true;
    int i,j;
    if(abs(la-ld)==abs(ca-cd) && ld!=la && cd!=ca )
    {
		chemin_vide=true;
        if(la>ld && ca>cd)
        {  
			i=ld+1;
            j=cd+1;
            while(i<la && chemin_vide==true)
            {
                if(ech[i][j]!=0) 
				{
					chemin_vide=false;
				}
                i++; 
				j++;
            }
        }
        if(la>ld && ca<cd)
        {
            i=ld+1;
            j=cd-1;
            while(i<la && chemin_vide==true)
            {
                if(ech[i][j]!=0)
				{
					chemin_vide=false;
				}
                i++; 
				j--;
            }
        }
        if(la<ld && ca>cd)
        {
            i=ld-1;
            j=cd+1;
            while(i>la && chemin_vide==true)
            {
                if(ech[i][j]!=0) 
				{
					chemin_vide=false;
				}
                i--; 
				j++;
            }
        }
        if(la<ld && ca<cd)
        {
            i=ld-1;
            j=cd-1;
            while(i>la && chemin_vide==true)
            {
                if(ech[i][j]!=0) 
				{
					chemin_vide=false;
				}
                i--; 
				j--;
            }
        }
    }
   	if(chemin_vide==true)
	{
		return check_arival_space(ld,cd,la,ca);
	}
	else 
	{
		return 0;
	}
}

int plateau::eval_dame(int ld,int cd,int la,int ca)
{
	if(eval_tour(ld,cd,la,ca)==1)
	{
		return 1;
	}
	if(eval_fou(ld,cd,la,ca)==1)
	{
		return 1;
	}
	return 0;
}

int plateau::eval_roi(int ld,int cd,int la,int ca)
{
	if(abs(la-ld)<2 && abs(ca-cd)<2)
	{
		return check_arival_space(ld,cd,la,ca);
	}
	return 0;
}

int plateau::evaluation(int ld,int cd,int la,int ca)
{
	switch(ech[ld][cd])
	{
		case  1: return eval_pion(ld,cd,la,ca);		break;
		case -1: return eval_pion(ld,cd,la,ca);		break;
		case  2: return eval_tour(ld,cd,la,ca); 	break;
		case -2: return eval_tour(ld,cd,la,ca);		break;
		case  3: return eval_cavalier(ld,cd,la,ca); break;
		case -3: return eval_cavalier(ld,cd,la,ca);	break;
		case  4: return eval_fou(ld,cd,la,ca);		break;
		case -4: return eval_fou(ld,cd,la,ca);		break;
		case  5: return eval_dame(ld,cd,la,ca);		break;
		case -5: return eval_dame(ld,cd,la,ca);		break;
		case  6: return eval_roi(ld,cd,la,ca);		break;
		case -6: return eval_roi(ld,cd,la,ca);		break;
	}
}


plateau::mov_Piece(int ld,int cd,int la,int ca)
{
	if(evaluation(ld,cd,la,ca)==1)
	{
		ech[la][ca]=ech[ld][cd];
		ech[ld][cd]=0;
		tour++;
	}
	else
	{
		//TODO
	}
}


int plateau::get_Piece(int i,int j)
{
	return ech[i][j];
}

plateau::init_Plateau()
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
