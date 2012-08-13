// invariants.cpp : Defines the entry point for the console application.
//
#include "bubbleprocess.h"
//#include "stdafx.h"
#include <stdlib.h>    // exit(..), NULL
#include <stdio.h>     // FILE structure
//#include <conio.h>     // clrscr(), getch()
#include <iostream>  // cin, cout objects & methods
#include <fstream>  // cin, cout objects & methods
#include <cmath>
#include <ctime>
using namespace std;

#define PI 3.14
#define M 30
#define N 30

#define PLACES 63
#define STARTNO 0



using namespace std;

void bubbleProcess::calculateInvariants(vector<bubblePoint> bubble, QString path, int itemNo)
{
  int  tourNumber = 3;
//int main(void) {
	char invariantDosya[50],coeffDosya[50];
	int dizinSirasi,dosyaSirasi,yerSirasi,incelenenNokta=0;
	float a[M][N],b[M][N],c[M][N],d[M][N],I[M][N];

	yerSirasi=0;
	dizinSirasi=0;
	dosyaSirasi=0;

	for (yerSirasi=STARTNO;yerSirasi<=PLACES;yerSirasi++)
		for (dizinSirasi=0;dizinSirasi<1;dizinSirasi++)
			for (dosyaSirasi=0;dosyaSirasi<1;dosyaSirasi++) {
				//if (yerSirasi!=4 || dizinSirasi!=14) {
				//sprintf(coeffDosya,"c%i\\apes%i\\coeffs%i.m",yerSirasi,dizinSirasi,dosyaSirasi);
				sprintf(coeffDosya,"Tour%i\\c%i\\apes%i\\coeffs%i.m",tourNumber, yerSirasi,dizinSirasi,dosyaSirasi);
				cout<<coeffDosya<<endl;
				sprintf(invariantDosya,"Tour%i\\c%i\\apes%i\\invariants%i.m",tourNumber,yerSirasi,dizinSirasi,dosyaSirasi);
				cout<<invariantDosya<<endl;
				fstream file_in(coeffDosya,ios::in);
				if(file_in.is_open()){
					file_in>>incelenenNokta;
					for (int m=0;m<M;m++)
						for (int n=0;n<N;n++)
							file_in>>m>>n>>a[m][n]>>b[m][n]>>c[m][n]>>d[m][n];
					file_in.close();

					fstream file_Invariant(invariantDosya,ios::out);

					for (int m=0;m<M;m++)
						for (int n=0;n<N;n++) {
							if (m==0)
								I[m][n]=a[m][n]*a[m][n]+c[m][n]*c[m][n];
							else
								I[m][n]=a[m][n]*a[m][n]+b[m][n]*b[m][n]+c[m][n]*c[m][n]+d[m][n]*d[m][n];
							file_Invariant<<m<<" "<<n<<" "<<I[m][n]<<endl;
						}
						file_Invariant.close();
				}			
				//}
			}

        //	return 0;
//}

}
