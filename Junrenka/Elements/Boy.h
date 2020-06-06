#pragma once

#include "CPerson.hpp"

class CBestFriend : public CPerson
{
	friend class CSelf;

public:
	CBestFriend(PWCHAR _Name):CPerson(_Name){}

};

class CSelf : public CPerson
{
	friend class BestFriend;
public:
	CSelf(PWCHAR _Name):CPerson(_Name) {}

	///<summary>�����ɂ���Đl�ɑ΂���D���x���X�V</summary>
	///<param name='pTarget'>�D���x��ݒ肷��Ώ�</param>
	///<remarks>���łɗ��l������ꍇ�͗�O�𓊂��܂�</remarks>
	void UpdateFavorableSensitibity(CPerson *pTarget,TASTELEVEL _FoodLevel)
	{
		EMOTION UpdateEmotion;
		switch (_FoodLevel)
		{
		case TASTELEVEL::Delicious:
			//��������������x�^����
			UpdateEmotion = EMOTION::Happy;
			break;
		case TASTELEVEL::Normal:
			UpdateEmotion = EMOTION::None;
			break;
		case TASTELEVEL::Uncomfortable:
			UpdateEmotion = EMOTION::Angry;
			break;
		default:
			UpdateEmotion = EMOTION::None;
			break;
		}

		if (m_vFavorableSensitibity.size() > 0)
		{
			auto it = std::find_if(m_vFavorableSensitibity.begin(), m_vFavorableSensitibity.end(),
				[pTarget](FAVORABLE_SENSITIBITY& fs) {
					return (fs.pPerson == pTarget);
				});
			if (it != m_vFavorableSensitibity.end())
			{
				it->Emotion = UpdateEmotion;
				return;
			}
		}

		m_vFavorableSensitibity.push_back(FAVORABLE_SENSITIBITY{ pTarget ,UpdateEmotion });

	}

	void NotifyGameResult(GAME_RESULT Result) override
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
			//�����ă}�W�M��
			m_Emotion = EMOTION::Majigire;
			break;
		default:
			m_Emotion = EMOTION::None;
			break;
		}
	}

	virtual void SetPartner(CPerson* pNewPartner) override
	{
		CPerson::SetPartner(pNewPartner);

		//�܂��߂Ȋ������
		m_Emotion = EMOTION::None;
	}
};