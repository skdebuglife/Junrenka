#include "CJunrenka.h"
#include <thread>
#include <vector>

#define ECHO printf

namespace
{
	///<summary>恋人を相互的に設定</summary>
	///<param name='pPerson1'>1人目のCPersonポインタ</param>
	///<param name='pPerson2'>2人目のCPersonポインタ</param>
	///<returns></returns>
	void SetPartner(CPerson* pPerson1, CPerson* pPerson2)
	{
		if (pPerson1 && pPerson2)
		{
			pPerson1->SetPartner(pPerson2);
			pPerson2->SetPartner(pPerson1);
		}
	}

	///<summary>友人関係を相互的に設定</summary>
	///<param name='pPerson1'>1人目のCPersonポインタ</param>
	///<param name='pPerson2'>2人目のCPersonポインタ</param>
	///<returns></returns>
	void SetFriend(CPerson* pPerson1, CPerson* pPerson2)
	{
		if (pPerson1 && pPerson2)
		{
			pPerson1->AddFriend(pPerson2);
			pPerson2->AddFriend(pPerson1);
		}
	}

	///<summary>月を見るため、10秒間CPersonのクリティカルセクションを取得するスレッド</summary>
	///<param name='pParam'>ロックを行うCPersonポインタ</param>
	///<returns>0</returns>
	DWORD WINAPI LookingMoonThread(PVOID pParam)
	{
		CPerson* pPerson= (CPerson*)pParam;

		__try
		{
			pPerson->EnterSC();

			::Sleep(10 * 1000);

		}
		__finally {
			pPerson->LeaveSC();
		}

		return 0;
	}

}

///<summary>CJunrenka　デフォルトコンストラクタ</summary>
CJunrenka::CJunrenka()
{
	//インスタンス化
	pOmae = new COmae((PWCHAR)L"お前", (PWCHAR)L"Pasta", TASTELEVEL::Delicious);
	pTsure = new CPerson((PWCHAR)L"ツレ");
	pBestfriend = new CBestFriend((PWCHAR)L"大親友");
	pSelf = new CSelf((PWCHAR)L"俺");

	//人間関係構築
	SetPartner(pTsure, pBestfriend);
	SetFriend(pOmae, pTsure);
	SetFriend(pSelf, pBestfriend);

	DWORD StarCount = GetTickCount() % STAR_MAX + STAR_MIN;
	for (DWORD i = 0; i < StarCount; i++)
	{
		std::shared_ptr<CStar> tempMemory(new CStar());

		MemoryOfStars.push_back(tempMemory);
		MultitudinousStars.push_back(tempMemory.get());
	}
	//億千の★の中にお前を追加
	MultitudinousStars.push_back(pOmae);
}

///<summary>CJunrenka　デストラクタ</summary>
CJunrenka::~CJunrenka()
{

	delete pOmae;
	delete pTsure;
	delete pBestfriend;
	delete pSelf;

}

///<summary>最も光度が高いCStarのポインタを取得する</summary>
///<param name='bShowStars'>真＝星の名前をすべて出力する、偽＝出力しない</param>
///<returns>最も高度が高いCStarのポインタ</returns>
CStar* CJunrenka::GetBrightestStar(bool bShowStars)
{
	CStar* pBrightestStar = NULL;
	pBrightestStar = MultitudinousStars[0];

	for (auto star : MultitudinousStars)
	{
		if (bShowStars) ECHO("%S\n", star->GetName());

		if (pBrightestStar->GetLuminosity() < star->GetLuminosity())
		{
			pBrightestStar = star;
		}
	}

	return pBrightestStar;
}

///<summary>サビ</summary>
void CJunrenka::Climax()
{
	pSelf->CloseMyEye();
	
	//目を閉じれば
	if (!pSelf->IsOpeningEye())
	{
		CStar* pBrightestStar;
		//億千の星
		pBrightestStar = GetBrightestStar(true);

		//一番光る
		ECHO("Brightest Star is %S\r\n", pBrightestStar->GetName());
	}

	try
	{
		//初めて一途になる
		pSelf->BecameFirstPure();
	}
	catch (PWCHAR *pMessage) {

	}

	//愛の歌を響かせる
	ECHO("Love Song\r\n");
}

///<summary>歌いだす</summary>
void CJunrenka::StartLyrics()
{
	//サビ
	Climax();

	//おいしい料理を作る
	TASTELEVEL FoodLevel = pOmae->CreateFood(true);

	pSelf->UpdateFavorableSensitibity(pOmae, FoodLevel);
	
	//大貧民
	PlayPoverty();

	pOmae->NotifyOthersEmotion(pSelf->GetEmotion());

	pSelf->NotifyOthersEmotion(pOmae->GetEmotion());


	EMOTION CurrentEmotion =  pSelf->GetEmotion();
	//うれしかったら
	if (CurrentEmotion == EMOTION::Happy)
	{
		//スキップ
		goto Skip_Label;
	}
	
	::Sleep(10 * 1000);
	
Skip_Label:

	//好きって言いたいのでジョブに追加
	//言いたいだけなので処理はしない
	pSelf->WantSayJob((PWCHAR)L"好き");

	DWORD Tid;
	//お前が月を見つめる
	HANDLE LookingMoon = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LookingMoonThread, pOmae, 0, &Tid);
	if (LookingMoon != NULL)
	{
		//月を見つめる君にくぎ付け
		WaitForSingleObject(LookingMoon, INFINITE);

		CloseHandle(LookingMoon);
	}

	__try
	{
		//守りたい女と初めて思う(相手はまだ了承していない)
		pSelf->SetPartner(pOmae);

		//ぎゅっと抱きしめる(ロックする)
		pOmae->EnterSC();
	}
	__finally {
		//(すぐ話す)
		pOmae->LeaveSC();
	}

	//サビ
	Climax();

}

///<summary>大貧民をプレイ</summary>
void CJunrenka::PlayPoverty()
{
	pOmae->NotifyGameResult(GAME_RESULT::Win);
	pTsure->NotifyGameResult(GAME_RESULT::Win);
	pBestfriend->NotifyGameResult(GAME_RESULT::Win);
	//俺だけ負け
	pSelf->NotifyGameResult(GAME_RESULT::Lose);
}