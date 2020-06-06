#include "CJunrenka.h"
#include <thread>
#include <vector>

#define ECHO printf

namespace
{
	///<summary>���l�𑊌ݓI�ɐݒ�</summary>
	///<param name='pPerson1'>1�l�ڂ�CPerson�|�C���^</param>
	///<param name='pPerson2'>2�l�ڂ�CPerson�|�C���^</param>
	///<returns></returns>
	void SetPartner(CPerson* pPerson1, CPerson* pPerson2)
	{
		if (pPerson1 && pPerson2)
		{
			pPerson1->SetPartner(pPerson2);
			pPerson2->SetPartner(pPerson1);
		}
	}

	///<summary>�F�l�֌W�𑊌ݓI�ɐݒ�</summary>
	///<param name='pPerson1'>1�l�ڂ�CPerson�|�C���^</param>
	///<param name='pPerson2'>2�l�ڂ�CPerson�|�C���^</param>
	///<returns></returns>
	void SetFriend(CPerson* pPerson1, CPerson* pPerson2)
	{
		if (pPerson1 && pPerson2)
		{
			pPerson1->AddFriend(pPerson2);
			pPerson2->AddFriend(pPerson1);
		}
	}

	///<summary>�������邽�߁A10�b��CPerson�̃N���e�B�J���Z�N�V�������擾����X���b�h</summary>
	///<param name='pParam'>���b�N���s��CPerson�|�C���^</param>
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

///<summary>CJunrenka�@�f�t�H���g�R���X�g���N�^</summary>
CJunrenka::CJunrenka()
{
	//�C���X�^���X��
	pOmae = new COmae((PWCHAR)L"���O", (PWCHAR)L"Pasta", TASTELEVEL::Delicious);
	pTsure = new CPerson((PWCHAR)L"�c��");
	pBestfriend = new CBestFriend((PWCHAR)L"��e�F");
	pSelf = new CSelf((PWCHAR)L"��");

	//�l�Ԋ֌W�\�z
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
	//����́��̒��ɂ��O��ǉ�
	MultitudinousStars.push_back(pOmae);
}

///<summary>CJunrenka�@�f�X�g���N�^</summary>
CJunrenka::~CJunrenka()
{

	delete pOmae;
	delete pTsure;
	delete pBestfriend;
	delete pSelf;

}

///<summary>�ł����x������CStar�̃|�C���^���擾����</summary>
///<param name='bShowStars'>�^�����̖��O�����ׂďo�͂���A�U���o�͂��Ȃ�</param>
///<returns>�ł����x������CStar�̃|�C���^</returns>
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

///<summary>�T�r</summary>
void CJunrenka::Climax()
{
	pSelf->CloseMyEye();
	
	//�ڂ�����
	if (!pSelf->IsOpeningEye())
	{
		CStar* pBrightestStar;
		//����̐�
		pBrightestStar = GetBrightestStar(true);

		//��Ԍ���
		ECHO("Brightest Star is %S\r\n", pBrightestStar->GetName());
	}

	try
	{
		//���߂Ĉ�r�ɂȂ�
		pSelf->BecameFirstPure();
	}
	catch (PWCHAR *pMessage) {

	}

	//���̉̂���������
	ECHO("Love Song\r\n");
}

///<summary>�̂�����</summary>
void CJunrenka::StartLyrics()
{
	//�T�r
	Climax();

	//�����������������
	TASTELEVEL FoodLevel = pOmae->CreateFood(true);

	pSelf->UpdateFavorableSensitibity(pOmae, FoodLevel);
	
	//��n��
	PlayPoverty();

	pOmae->NotifyOthersEmotion(pSelf->GetEmotion());

	pSelf->NotifyOthersEmotion(pOmae->GetEmotion());


	EMOTION CurrentEmotion =  pSelf->GetEmotion();
	//���ꂵ��������
	if (CurrentEmotion == EMOTION::Happy)
	{
		//�X�L�b�v
		goto Skip_Label;
	}
	
	::Sleep(10 * 1000);
	
Skip_Label:

	//�D�����Č��������̂ŃW���u�ɒǉ�
	//�������������Ȃ̂ŏ����͂��Ȃ�
	pSelf->WantSayJob((PWCHAR)L"�D��");

	DWORD Tid;
	//���O���������߂�
	HANDLE LookingMoon = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LookingMoonThread, pOmae, 0, &Tid);
	if (LookingMoon != NULL)
	{
		//�������߂�N�ɂ����t��
		WaitForSingleObject(LookingMoon, INFINITE);

		CloseHandle(LookingMoon);
	}

	__try
	{
		//��肽�����Ə��߂Ďv��(����͂܂��������Ă��Ȃ�)
		pSelf->SetPartner(pOmae);

		//������ƕ������߂�(���b�N����)
		pOmae->EnterSC();
	}
	__finally {
		//(�����b��)
		pOmae->LeaveSC();
	}

	//�T�r
	Climax();

}

///<summary>��n�����v���C</summary>
void CJunrenka::PlayPoverty()
{
	pOmae->NotifyGameResult(GAME_RESULT::Win);
	pTsure->NotifyGameResult(GAME_RESULT::Win);
	pBestfriend->NotifyGameResult(GAME_RESULT::Win);
	//����������
	pSelf->NotifyGameResult(GAME_RESULT::Lose);
}