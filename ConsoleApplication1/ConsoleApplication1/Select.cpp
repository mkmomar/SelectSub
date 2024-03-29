// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <sstream>
#include <string>
#include<vector>
#include<regex>
using namespace std;

string Select(string str, vector<pair<int, int>> ranges, bool bytes, bool complement)
{
	string Ostr="";
	int nCount = 1;
	int nLength = str.length();
	if (!nLength)
		return Ostr;
	bool* pbOut =new bool [nLength+1];
	for (int nx = 0;nx < nLength+1;nx++)
		pbOut[nx] = complement;
	int* pnIndex = new int[nLength+1];
	pnIndex[0] = 0;
	if (bytes)
	{
		for (int nx = 1;nx <= nLength;nx++)
			pnIndex[nx] = nx;
		nCount = nLength;
	}
	else
		for (int nx = 1; nx < nLength;nx++)
			if (str[nx] != ' '&&str[nx-1]==' ')
				pnIndex[nCount++] = nx;
	pnIndex[nCount] = nLength;
	for (unsigned int ni = 0;ni < ranges.size();ni++)
	{
		int nStart = ranges[ni].first > 0 ? ranges[ni].first : 0;
		int nEnd = ranges[ni].second > 0 && ranges[ni].second <nLength? ranges[ni].second : nLength;
		for (int nj = nStart;nj <= nEnd;nj++)
			pbOut[nj] = !complement;
	}
	int no = 0;
	while(no<nCount) {
		while (no<nCount&&!pbOut[no]) no++;
		if (no < nCount)
		{
			int nk = no + 1;
			while (nk < nCount&&pbOut[nk])nk++;
			Ostr.append(str, pnIndex[no], pnIndex[nk] - pnIndex[no]);
			no = nk;
		}
	};
	delete[] pbOut;
	delete[] pnIndex;
	return Ostr;
}
int TestSelectForErrors(void)
{
	int nRetVal = 0;
	vector<pair<int, int>> vRanges = { pair<int,int>(3,29),pair<int,int>(5,9),pair<int,int>(-1,7) };
	vector<pair<int, int>> vRanges2 = { pair<int,int>(7,-1),pair<int,int>(3,8),pair<int,int>(5,9) };
	vector<pair<int, int>> vRanges3 = { pair<int,int>(1,3),pair<int,int>(5,6),pair<int,int>(5,7) };
	// Test  White-Space String
	string sWhite("       ");
	for (int ni = 0;ni < 2;ni++)
		for (int nj = 0;nj < 2;nj++)
		{
			string OString = Select(sWhite, vRanges, (bool)ni, (bool)nj);
			if ((nj==1 && OString != "") || (nj == 0 && OString!=sWhite))
				nRetVal++;
		}

	for (int ni = 0;ni < 2;ni++)
		for (int nj = 0;nj < 2;nj++)
		{
			string OString = Select(sWhite, vRanges2, (bool)ni, (bool)nj);
			if ((ni==0&&nj==0&&OString != "")|| (ni == 0 && nj == 1 && OString != sWhite)||(ni == 1 && nj == 0 && OString != "    ")|| (ni == 1 && nj == 1 && OString != "   "))
					nRetVal++;
		}

	for (int ni = 0;ni < 2;ni++)
		for (int nj = 0;nj < 2;nj++)
		{
			string OString = Select(sWhite, vRanges3, (bool)ni, (bool)nj);
			if ((ni == 0 && nj == 0 && OString != "") || (ni == 0 && nj == 1 && OString != sWhite) || (ni == 1 && nj == 0 && OString != "     ") || (ni == 1 && nj == 1 && OString != "  "))
				nRetVal++;
		}
	// Test  Other String
	string sSec("ABCD EFGHI JKLMNO PQRST XYZ@#$ %^^&*   ");
	for (int ni = 0;ni < 2;ni++)
		for (int nj = 0;nj < 2;nj++)
		{
			string OString = Select(sSec, vRanges, (bool)ni, (bool)nj);
			if ((ni == 0 && nj == 0 && OString != sSec) || (ni == 0 && nj == 1 && OString != "")||(ni==1&&nj==0&&OString!="ABCD EFGHI JKLMNO PQRST XYZ@#$")||(ni==1&&nj==1&&OString!=" %^^&*   "))
				nRetVal++;
		}

	for (int ni = 0;ni < 2;ni++)
		for (int nj = 0;nj < 2;nj++)
		{
			string OString = Select(sSec, vRanges2, (bool)ni, (bool)nj);
			if ((ni == 0 && nj==0&&OString != "PQRST XYZ@#$ %^^&*   ") || (ni == 0 && nj == 1 && OString != "ABCD EFGHI JKLMNO ") || (ni == 1 && nj == 0 && OString != "D EFGHI JKLMNO PQRST XYZ@#$ %^^&*   ")||(ni==1&&nj==1&&OString!="ABC"))
				nRetVal++;
		}

	for (int ni = 0;ni < 2;ni++)
		for (int nj = 0;nj < 2;nj++)
		{
			string OString = Select(sSec, vRanges3, (bool)ni, (bool)nj);
			if ((ni == 0 && nj==0 && OString != "EFGHI JKLMNO PQRST %^^&*   ") || (ni == 0 && nj == 1 && OString != "ABCD XYZ@#$ ") || (ni == 1 && nj == 0 && OString != "BCDEFG")||(ni==1 && nj==1 && OString != "A HI JKLMNO PQRST XYZ@#$ %^^&*   "))
				nRetVal++;
		}
	return nRetVal;
}
int main(int argc, char *argv[])
{
	if (argc != 6)
	{
		cout << "Incorrect commandline" << endl;
		cout << "Usage: Select string Ranges bytes complement Test" << endl;
		cout << "Example: Select \"I have no problem\" {(0,2),(3,-)} 0 0 0" << endl;
		return 1;
	}
	string str(argv[1]);
	string sRanges(argv[2]);
	bool bytes = (bool)atoi(argv[3]);
	bool complement = (bool)atoi(argv[4]);
	bool bTest = (bool)atoi(argv[5]);
	int ni = 0;
	sRanges = regex_replace(sRanges, regex{ R"([|{|(|)|}|,])" }, " ");
	istringstream ssRanges{ regex_replace(sRanges, regex{ R"(-)" }, "-1") };
	vector<int> v{ istream_iterator<int>{ssRanges}, istream_iterator<int>{} };
	vector<pair<int, int> > vpRanges;
	for (unsigned int nx = 0;nx < v.size();nx += 2)
	{
		int nStart = v[nx];
		int nEnd = v[nx + 1];
		vpRanges.push_back(pair<int, int>(nStart, nEnd));
	}
	int nValue = 0;
	if (!bTest)
	{
		string Fstr = Select(str, vpRanges, bytes, complement);
		cout << Fstr << endl;
	}
	else nValue=TestSelectForErrors();
	return nValue;
}

