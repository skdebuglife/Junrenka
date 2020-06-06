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
	///<summary>CPerson�@�R���X�g���N�^</summary>
	///<param name='Name'>�l�̖��O</param>
	///<param name='Favourite'>���ӗ����̕�����|�C���^</param>
	///<param name='TastLevel'>���ӗ����̂�������</param>
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

	///<summary>CPerson�@�f�X�g���N�^</summary>
	~CPerson()
	{
		
	}

	///<summary>CPerson�@�f�t�H���g�R���X�g���N�^</summary>
	CPerson()
	{
		CommonInitialize((PWCHAR)NAME_DEFAULT);

		m_TastLevel = TASTELEVEL::Normal;
		m_pMenu = NULL;
		
	}

	///<summary>���l��ݒ�</summary>
	///<param name='pNewPartner'>���l�ɂȂ�l�ւ̃|�C���^</param>
	///<remarks>���łɗ��l������ꍇ�͗�O�𓊂��܂�</remarks>
	virtual void SetPartner(CPerson* pNewPartner)
	{
		if (m_pPartner)
		{
			throw L"Futamata!";
		}
		
		m_pPartner = pNewPartner;
	}

	///<summary>���l�ƕʂ�܂�</summary>
	///<remarks>���l�����Ȃ��Ƃ��͗�O�𓊂��܂�</remarks>
	void Wakarebanashi()
	{
		if (!m_pPartner)
		{
			throw L"Mousou OTSU";
		}

		m_pPartner = NULL;
		
	}

	///<summary>�F�B��ǉ����܂�</summary>
	///<param name='pNewFriend'>�F�B�ɂȂ�l�ւ̃|�C���^</param>
	void AddFriend(CPerson* pNewFriend)
	{
		m_vpFriends.push_back(pNewFriend);
	}

	///<summary>���߂Ĉ�r�ɂȂ�</summary>
	///<remarks>��r�ɂȂ�̂����߂Ăł͂Ȃ��ꍇ�A��O�𓊂��܂�</remarks>
	void BecameFirstPure()
	{
		if (m_IsAlreadyPure) throw L"Already Pure";

		m_IsAlreadyPure = true;
	}

	///<summary>�т����</summary>
	///<param name='isDisplayMenuName'>�^����������\������F�U����������\�����Ȃ�</param>
	///<returns>�����x��</returns>
	TASTELEVEL CreateFood(bool isDisplayMenuName)
	{
		if (isDisplayMenuName && m_pMenu.get())
			printf("%S\r\n", m_pMenu.get());

		return m_TastLevel;
	}

	///<summary>�ڂ����</summary>
	void CloseMyEye() { m_IsEyeOpening = false; }
	///<summary>�ڂ��J��/summary>
	void OpenMyEye() { m_IsEyeOpening = false; }
	///<summary>�ڂ��J���Ă��邩�擾</summary>
	///<returns>�^���J���Ă�F�U�����Ă�</returns>
	bool IsOpeningEye() { return m_IsEyeOpening; }

	///<summary>���݂̊�����擾</summary>
	///<returns>���݂̊���</returns>
	EMOTION GetEmotion() { return m_Emotion; }

	///<summary>�Q�[�����ʂ�ʒm</summary>
	///<param name='Result'>�Q�[���̌���</param>
	///<remarks>�Q�[���̌��ʂɂ���Ċ�����X�V</remarks>
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

	///<summary>���l�̊����ʒm</summary>
	///<param name='Emotion'>���l�̊���</param>
	///<remarks>���l�̊���ɂ���Ċ�����X�V</remarks>
	virtual void NotifyOthersEmotion(EMOTION Emotion)
	{
		m_Emotion = Emotion;
	}

	///<summary>�����������Ƃ����߂Ă���</summary>
	///<param name='pWords'>������������</param>
	///<remarks>�������������������ƂȂ̂ŏ����͂��Ȃ�</remarks>
	void WantSayJob(PWCHAR pWords)
	{
		if (!pWords) throw "NULL Poiter";

		int NeedLength = wcslen(pWords) + 1;

		std::shared_ptr<WCHAR[]> pPointer(new WCHAR[NeedLength], [](WCHAR* pPointer) { delete[] pPointer; });

		RtlCopyMemory(pPointer.get(), pWords, NeedLength << 1);

		m_vpJob.push_back(pPointer);
	}

	///<summary>���炩�̃��b�N</summary>
	void EnterSC() { ::EnterCriticalSection(&csLock); }

	///<summary>���炩�̃��b�N������</summary>
	void LeaveSC() { ::LeaveCriticalSection(&csLock); }
};