#pragma once

constexpr auto VAULT_MAGIC = 0x6E564C54;
#define KUR	0
constexpr auto VAULT_VERSION = 0x0200;

struct Compare_struct {
	size_t sizeXp;
	string szString;
  
} Entries[2];

int counter;

bool compare_xp(const Compare_struct& szFirst, const Compare_struct& szSecond)
{
	return (szFirst.sizeXp < szSecond.sizeXp);
}

bool Find_CaseNotSens(const char *szFirst, const  char* szDve)
{
	int iCounter = 0;
	do {
		//std::cout << *(szFirst + iCounter) % 32 << " || " << *(szDve + iCounter) % 32 << "\n";
		//std::cout << *(szFirst + iCounter)<< "\n";
		//std::cout << szFirst << "\n";
		//system("pause");
		if (*szFirst >= 'A' && *szFirst <= 'z' && *szDve >= 'A' && *szDve <= 'z')
		{
			if (*szFirst % 32 != *szDve % 32)
				return false;
		}
		else if( *szFirst != *szDve)
		{
			return false;
		}
		szFirst++;
		szDve++;
	} while (*szFirst != 0 && *szDve != 0);

	return true;
}

void CheckVersion(FILE *fp)
{
	uint16_t version;
	fread(&version, sizeof(uint16_t), 1, fp);
	if (version != VAULT_VERSION)
	{
		std::cout << "Wrong version " << std::hex << version << "!/n";
		system("pause");
		fclose(fp);
		//std::abort();
	}
}
void CheckMagic(FILE* fp)
{
	int32_t magic;
	fread(&magic, sizeof(uint32_t), 1, fp);
	if (magic != VAULT_MAGIC)
	{
		std::cout << "Wrong magic runes " << std::hex << magic << "!/n";
		system("pause");
		fclose(fp);
		//std::abort();
	}
}

void PrintStats(char * VaultName, int32_t entries)
{
	std::printf("\
File name: %s\n\
Entries: %d\n\n\
What do u want to do?\n\
1. Find 1 entry\n\
2. Find multiple entries\n\
3. Print All entries\n\
4. Sort by rank\n", VaultName, entries);
}

void Closing(FILE* &fp, char* &key, char* &val)
{
	if(!fclose(fp))
		std::cout << "Closing Vault Succesfully!\n";
	else
		std::cout << "Vault Didn't Close!\n";

	if (key)
	{
		delete[] key;
		key = NULL;
	}
	if (val)
	{
		delete[] val;
		val = NULL;
	}
}
