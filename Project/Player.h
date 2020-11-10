#pragma once

#include	"Mof.h"
#include	"GameDefine.h"
#include	"WaterPlace.h"


//�ړ����x
#define		PLAYER_SPEED			0.3f

//�ړ��ő呬�x
#define		PLAYER_MAXSPEED			10.0f

//�W�����v����
#define		PLAYER_JUMP				-10.0f

//�U����
#define		PLAYER_ATTACKWIDTH		30

//�����蔻�茸����
#define		PLAYER_RECTDECREASE		12

//MP
#define		MP		10

class CPlayer {
private:
	CTexture				m_Texture;
	CSpriteMotionController	m_Motion;
	float					m_PosX;
	float					m_PosY;
	bool					m_bMove;
	float					m_MoveX;
	float					m_MoveY;
	bool					m_bJump;
	bool					m_bReverse;
	CRectangle				m_SrcRect;

	int						m_HP;
	int                     m_MP;
	int						m_MPPlus;
	int						m_MPRecovWait;

	bool					m_WaterPlaceflg;

	int						m_Level;

	int						m_DamageWait;

	CTexture				m_FrameTexture;
	CTexture				m_HPTexture;
	CTexture                m_MPTexture;
	CTexture				m_MPEmpTexture;

	bool					m_bEnd;

	bool					m_bGoal;

	bool					m_bAttack;
	int						m_AttackPhase;

	//���[�V������ޒ�`
	enum tag_MOTION {
		MOTION_WAIT,
		MOTION_MOVE,
		MOTION_JUMPSTART,
		MOTION_JUMPEND,
		MOTION_ATTACK,
		MOTION_ATTACK2,
		MOTION_DAMAGE,


		MOTION_COUNT,
	};

public:
	CPlayer();
	~CPlayer();
	bool Load(void);
	void Initialize(void);
	void Update(void);
	void UpdateKey(void);
	void UpdateMove(void);
	void CollisionStage(float ox, float oy);
	void CollisionWaterPlace(CWaterPlace& wrp);
	void Render(float wx, float wy);
	void RenderStatus(void);
	void RenderDebug(float wx, float wy);
	void Release(void);
	bool IsAttack() { return m_Motion.GetMotionNo() == MOTION_ATTACK; }
	CRectangle GetRect() {
		if (IsAttack())
		{
			return CRectangle(m_PosX + PLAYER_RECTDECREASE, m_PosY + PLAYER_RECTDECREASE, m_PosX + m_SrcRect.GetWidth() - PLAYER_RECTDECREASE - PLAYER_ATTACKWIDTH, m_PosY + m_SrcRect.GetHeight());
		}
		return CRectangle(m_PosX + PLAYER_RECTDECREASE, m_PosY + PLAYER_RECTDECREASE, m_PosX + m_SrcRect.GetWidth() - PLAYER_RECTDECREASE, m_PosY + m_SrcRect.GetHeight());
	}
	CRectangle GetAttackRect() {
		//���]��
		if (m_bReverse)
		{
			return CRectangle(m_PosX - PLAYER_ATTACKWIDTH, m_PosY, m_PosX + PLAYER_RECTDECREASE, m_PosY + m_SrcRect.GetHeight());
		}
		return CRectangle(m_PosX + m_SrcRect.GetWidth() - PLAYER_RECTDECREASE - PLAYER_ATTACKWIDTH, m_PosY, m_PosX + m_SrcRect.GetWidth(), m_PosY + m_SrcRect.GetHeight());
	}
	bool IsEnd(void) { return m_bEnd; }
	bool IsGoal(void) { return m_bGoal; }
};