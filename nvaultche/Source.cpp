#include <cstdio>
#include <iostream>
#include <algorithm>
#include <istream>
#include <string>
#include <cstring>
#include <iomanip>
using namespace std;
#include "useless_shit.h"


int main(int argc, char* argv[])
{
	FILE* fp;
	char* VaultName = nullptr, *key = NULL, *val = NULL;
	Compare_struct CompList;
	vector<Compare_struct> szEntryName;
	char szFileName[200], szEntryContentTemp[200];
	string szTemp;
	int iDotChar = 0, iLastSlash = 0, iChoise = 0, iCharNum = KUR;
	bool stop = false, bRewind = false;
	uint8_t oldkeylen = 0, keylen;
	uint16_t oldvallen = 0, vallen;
	int32_t iTimeStamp, Total_Num_Ent;
	long iFilePosition;

	if (argc < 2)
	{
		#define DEFAULT_DIR "/home/gosho/Neshta Ot stariq Hard/kod/repos/nvaultche/SimpleRanks.vault"
		fp = fopen(DEFAULT_DIR, "rb");
		if (fp != nullptr)
		{
			VaultName = new char[sizeof "SimpleRanks"];
			strcpy(VaultName, "SimpleRanks");
			strcpy(szFileName, DEFAULT_DIR);
		}
		else
		{
			cout << "Please drag a file here and press ENTER.\n>> ";
			cin >> szFileName;
			cin.ignore(1000, '\n');
			goto hasfile;
		}
	}
	else
	{
	hasfile:;

		//cout << "sss " << argc << " + " << argv[0] << " + " << argv[1] << "\n";
		if(szFileName[0] == 0)
			strcpy(szFileName, argv[1]);

		while (!stop)
		{
			iCharNum++;
			switch (szFileName[iCharNum])
			{
			case '/': case '\\':
				iLastSlash = iCharNum;
				break;
			case '.':
				iDotChar = iCharNum;
				break;
			case '\0':
				//iLastSlash = iCharNum;
				stop = true;
				break;
			}
		}

		if (strcmp(szFileName + iDotChar, ".vault") != 0)
		{
			cout << "This not a .vault mate\n";
			//system("pause");
			return 0;
		}

		VaultName = new char[iCharNum - iLastSlash + 1];

		strcpy(VaultName, szFileName + iLastSlash + 1);
		if(iDotChar < iLastSlash)
			iDotChar = iLastSlash + 1;

		VaultName[iDotChar - iLastSlash - 1] = 0;
		//system("pause");
		if (!iLastSlash)
		{
			cout << "Sorry i am a dumb program and i can't read\n" << "Is this the vault name:\n\"" << VaultName << "\"";
			return 0;
		}

		fp = fopen(szFileName, "rb");
	}


	CheckMagic(fp);
	CheckVersion(fp);

	fread(&Total_Num_Ent, sizeof(int32_t), 1, fp);
	iFilePosition = ftell(fp);
rewind:;
	if (bRewind)
	{
		fseek(fp, iFilePosition, SEEK_SET);

		bRewind = false;
	}

	PrintStats(VaultName, Total_Num_Ent);
	cin >> iChoise;
	cin.ignore(1000, '\n');

	switch (iChoise)
	{
		case 1:
		{
			cout << "Give me txt :D\n";
			//cin.getline(szTemp, sizeof szTemp);
			getline(cin, szTemp);
			CompList.szString = szTemp;
			szEntryName.push_back(CompList);

			break;
		}
		case 2:
		{
			cout << "Write each entry u want found on a new row\n";
			cout << "And when ur finished, enter an empty line!\n\n";


			bool bItTimeToStop = false;

			while (!bItTimeToStop)
			{
				getline(cin, szTemp);
				//printf("chara e %d \n", szTemp[0]);

				if (szTemp[0] == 0)
					bItTimeToStop = true;
				else
				{
					CompList.szString = szTemp;
					szEntryName.push_back(CompList);
					//printf("Added \"%s\" succesfully\n", szTemp.c_str());
				}
			}
			//cout << szEntryName.size() << " Total_Num_Ent have been detected\n";
			break;
		}
		case 3: case 4: break;
		default:
		{
			cout << "_|_\n";
			abort();
		}
	}

	for (int32_t i = 0; i < Total_Num_Ent; i++)
	{

		fread(&iTimeStamp, sizeof(int32_t), 1, fp);
		fread(&keylen, sizeof(uint8_t), 1, fp);
		fread(&vallen, sizeof(uint16_t), 1, fp);

		if (!keylen || keylen > oldkeylen)
		{
			if (key) delete[] key;
			key = new char[keylen + 1];
			oldkeylen = keylen;
		}
		if (!vallen || vallen > oldvallen)
		{
			if (val) delete[] val;
			val = new char[vallen + 1];
			oldvallen = vallen;
		}

		fread(key, sizeof(char), keylen, fp);
		fread(val, sizeof(char), vallen, fp);

		key[keylen] = '\0', val[vallen] = '\0';

		switch (iChoise)
		{
			case 1:
			{
				if(Find_CaseNotSens(key, szEntryName.begin()->szString.c_str()))
				{
					printf("%d:%s[%s]\n", iTimeStamp, key, val);
					continue;
				}
				break;
			}
			case 2:
			{
				vector<Compare_struct>::iterator it;
				for (it = szEntryName.begin(); it != szEntryName.end(); ++it)
				{
					if (Find_CaseNotSens(key, it->szString.c_str()))
					{
						printf("%d:%s[%s]\n", iTimeStamp, key, val);
						continue;
					}
				}
				break;
			}
			case 3:
			{
				printf("%d:%s[%s]\n", iTimeStamp, key, val);
				break;
			}
			case 4:
			{
				char szFormatedLine[200];
				int UserXP = 0, cFoundNz = 0;

				do {
					if (val[cFoundNz] > '9' || val[cFoundNz] < '0')
					{
						if(UserXP)
							break;
						else
							continue;
					}
					UserXP = (UserXP * 10) + (val[cFoundNz] - '0');

					cFoundNz++;
				} while (cFoundNz < vallen);

				CompList.sizeXp = UserXP;
				snprintf(szFormatedLine, (sizeof szFormatedLine)-1, "%d:%s[%s]\n", iTimeStamp, key, val);
				CompList.szString = szFormatedLine;

				szEntryName.push_back(CompList);
				break;
			}
		}
	}

	if (iChoise == 4)
	{
		//cout << "pre sorting" << "\n";
		//szEntryName.sort(compare_xp);
		std::sort(szEntryName.begin(), szEntryName.end(), &compare_xp);
		vector<Compare_struct>::iterator it;
		for (it = szEntryName.begin(); it != szEntryName.end(); ++it)
		{
			//printf("%s\n", *it);

			cout << it->szString;
		}
	}
	printf("\nDisplay format:	Timestamp:Key[Key Value]\n\n");

	printf("What do u want to do?\n\
1. Restart Search\n\
0. Exit\n");

	cin >> iChoise;
	cin.ignore(1000, '\n');

	switch (iChoise)
	{
	case 1:
		bRewind = true;
		szEntryName.clear();
		system("clear");
		goto rewind;
	}

	//system("pause");
	Closing(fp, key, val);
}
