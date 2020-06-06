#pragma once

#include <Windows.h>
#include <vector>
#include <memory>

#define NAME_DEFAULT L"NoName"

class CPerson;

typedef enum class _Emotion {
	None,
	Happy,
	Angry,
	Majigire
}EMOTION;


typedef enum class _TasteLevel {
	Uncomfortable,
	Normal,
	Delicious
}TASTELEVEL;


typedef struct _FavorableSensitibity
{
	CPerson* pPerson;
	EMOTION Emotion;

}FAVORABLE_SENSITIBITY, *PFAVORABLE_SENSITIBITY;

typedef enum class _GameResult {
	Draw,
	Win,
	Lose,
}GAME_RESULT;

class CPerson {

private:
	bool m_IsEyeOpening;
	std::shared_ptr<WCHAR[]> m_pName;
	std::shared_ptr<WCHAR[]> m_pMenu;
	std::vector<CPerson*> m_vpFriends;
	TASTELEVEL m_TastLevel;
	bool m_IsAlreadyPure;
	std::vector <std::shared_ptr<WCHAR[]>> m_vpJob;
	CRITICAL_SECTION csLock;

	void CommonInitialize(PWCHAR Name)
	{
		m_IsEyeOpening = true;
		m_Emotion = EMOTION::Happy;
		m_pPartner = NULL;
		m_IsAlreadyPure = false;

		::InitializeCriticalSection(&csLock);

		DWORD NameLength = wcslen(Name) + 1;
		m_pName = std::shared_ptr<WCHAR[]>(new WCHAR[NameLength], [](WCHAR* pPointer) { delete[] pPointer; });

		RtlCopyMemory(m_pName.get(), Name, NameLength << 1);
	}

protected:
	std::vector<FAVORABLE_SENSITIBITY> m_vFavorableSensitibity;
	EMOTION m_Emotion;
	CPerson* m_pPartner;

public:
	///<summary>CPerson　コンストラクタ</summary>
	///<param name='Name'>人の名前</param>
	///<param name='Favourite'>得意料理の文字列ポインタ</param>
	///<param name='TastLevel'>得意料理のおいしさ</param>
	CPerson(PWCHAR Name, PWCHAR Favourite = NULL, TASTELEVEL TastLevel = TASTELEVEL::Normal)
	{
		CommonInitialize(Name);

		if (Favourite)
		{
			DWORD MenuLength = wcslen(Favourite) + 1;
			m_pMenu = std::shared_ptr<WCHAR[]>(new WCHAR[MenuLength], [](WCHAR* pPointer) { delete[] pPointer; });

			RtlCopyMemory(m_pMenu.get(), Favourite, MenuLength << 1);
			m_TastLevel = TastLevel;	
		}
		else
		{
			m_pMenu = NULL;
		}
	}

	///<summary>CPerson　デストラクタ</summary>
	~CPerson()
	{
		
	}

	///<summary>CPerson　デフォルトコンストラクタ</summary>
	CPerson()
	{
		CommonInitialize((PWCHAR)NAME_DEFAULT);

		m_TastLevel = TASTELEVEL::Normal;
		m_pMenu = NULL;
		
	}

	///<summary>恋人を設定</summary>
	///<param name='pNewPartner'>恋人になる人へのポインタ</param>
	///<remarks>すでに恋人がいる場合は例外を投げます</remarks>
	virtual void SetPartner(CPerson* pNewPartner)
	{
		if (m_pPartner)
		{
			throw L"Futamata!";
		}
		
		m_pPartner = pNewPartner;
	}

	///<summary>恋人と別れます</summary>
	///<remarks>恋人がいないときは例外を投げます</remarks>
	void Wakarebanashi()
	{
		if (!m_pPartner)
		{
			throw L"Mousou OTSU";
		}

		m_pPartner = NULL;
		
	}

	///<summary>友達を追加します</summary>
	///<param name='pNewFriend'>友達になる人へのポインタ</param>
	void AddFriend(CPerson* pNewFriend)
	{
		m_vpFriends.push_back(pNewFriend);
	}

	///<summary>初めて一途になる</summary>
	///<remarks>一途になるのが初めてではない場合、例外を投げます</remarks>
	void BecameFirstPure()
	{
		if (m_IsAlreadyPure) throw L"Already Pure";

		m_IsAlreadyPure = true;
	}

	///<summary>飯を作る</summary>
	///<param name='isDisplayMenuName'>真＝料理名を表示する：偽＝料理名を表示しない</param>
	///<returns>味レベル</returns>
	TASTELEVEL CreateFood(bool isDisplayMenuName)
	{
		if (isDisplayMenuName && m_pMenu.get())
			printf("%S\r\n", m_pMenu.get());

		return m_TastLevel;
	}

	///<summary>目を閉じる</summary>
	void CloseMyEye() { m_IsEyeOpening = false; }
	///<summary>目を開く/summary>
	void OpenMyEye() { m_IsEyeOpening = false; }
	///<summary>目を開いているか取得</summary>
	///<returns>真＝開いてる：偽＝閉じてる</returns>
	bool IsOpeningEye() { return m_IsEyeOpening; }

	///<summary>現在の感情を取得</summary>
	///<returns>現在の感情</returns>
	EMOTION GetEmotion() { return m_Emotion; }

	///<summary>ゲーム結果を通知</summary>
	///<param name='Result'>ゲームの結果</param>
	///<remarks>ゲームの結果によって感情を更新</remarks>
	virtual void NotifyGameResult(GAME_RESULT Result)
	{
		switch (Result)
		{
		case GAME_RESULT::Draw:
			m_Emotion = EMOTION::None;
			break;
		case GAME_RESULT::Win:
			m_Emotion = EMOTION::Happy;
			break;
		case GAME_RESULT::Lose:
			m_Emotion = EMOTION::Angry;
			break;
		default:
			m_Emotion = EMOTION::None;
			break;
		}
	}

	///<summary>他人の感情を通知</summary>
	///<param name='Emotion'>他人の感情</param>
	///<remarks>他人の感情によって感情を更新</remarks>
	virtual void NotifyOthersEmotion(EMOTION Emotion)
	{
		m_Emotion = Emotion;
	}

	///<summary>言いたいことをためておく</summary>
	///<param name='pWords'>言いたいこと</param>
	///<remarks>ただただ言いたいことなので処理はしない</remarks>
	void WantSayJob(PWCHAR pWords)
	{
		if (!pWords) throw "NULL Poiter";

		int NeedLength = wcslen(pWords) + 1;

		std::shared_ptr<WCHAR[]> pPointer(new WCHAR[NeedLength], [](WCHAR* pPointer) { delete[] pPointer; });

		RtlCopyMemory(pPointer.get(), pWords, NeedLength << 1);

		m_vpJob.push_back(pPointer);
	}

	///<summary>何らかのロック</summary>
	void EnterSC() { ::EnterCriticalSection(&csLock); }

	///<summary>何らかのロックを解除</summary>
	void LeaveSC() { ::LeaveCriticalSection(&csLock); }
};