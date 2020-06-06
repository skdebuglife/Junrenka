#pragma once

#include "Elements/Boy.h"
#include "Elements/CPerson.hpp"
#include "Elements/COmae.h"

#include <vector>
#include <memory>

#define STAR_MIN 1000
#define STAR_MAX 10000

class CJunrenka
{
private:
	void Climax();

	CBestFriend *pBestfriend;
	CSelf *pSelf;
	CPerson *pTsure;
	COmae *pOmae;

	std::vector<std::shared_ptr<CStar>> MemoryOfStars;
	std::vector<CStar*> MultitudinousStars;

	
	CStar* GetBrightestStar(bool bShowStars);

	void PlayPoverty();

public:

	CJunrenka();
	~CJunrenka();

	void StartLyrics();
};