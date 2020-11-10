#pragma once
#include	"Mof.h"
#include	"GameDefine.h"


//ìñÇΩÇËîªíËå∏êäïù
#define		ENEMY_RECTDECREASE		10

//çıìGîÕàÕ
#define		ENEMY_SEARCHDIST		200

class CFire {
private:
	CTexture*				m_pTexture;
	CSpriteMotionController	m_Motion;
	int						m_Type;
	float					m_PosX;
	float					m_PosY;
	float					m_MoveX;
	float					m_MoveY;
	bool					m_bShow;
	bool					m_bReverse;
	CRectangle				m_SrcRect;

	int						m_HP;
	int						m_DamageWait;



	float					m_InitX, m_InitY;

public:
	CFire();
	~CFire();
	void Initialize(float px, float py, int type);
	void Update(void);
	void Damage(int dmg, bool bRev);
	void CollisionStage(float ox, float oy);
	void Render(float wx, float wy);
	void RenderDebug(float wx, float wy);
	void Release(void);
	void SetTexture(CTexture* pt) { m_pTexture = pt; }
	bool GetShow(void) { return m_bShow; }
	int GetDamageWait(void) { return m_DamageWait; }


	CRectangle GetSearchRect()
	{
		if (!m_bReverse)
		{
			return CRectangle(m_PosX, m_PosY + ENEMY_RECTDECREASE, m_PosX + m_SrcRect.GetWidth() + ENEMY_SEARCHDIST, m_PosY + m_SrcRect.GetHeight());
		}
		return CRectangle(m_PosX - ENEMY_SEARCHDIST, m_PosY + ENEMY_RECTDECREASE, m_PosX + m_SrcRect.GetWidth(), m_PosY + m_SrcRect.GetHeight());
	}

	void turn()
	{
		m_bReverse = !m_bReverse;
		m_PosX -= m_MoveX;
		m_MoveX = 5.0f * m_bReverse ? -1 : 1;
	}

	void SetMoveDir(bool isRight)
	{
		m_bReverse = !isRight;
		m_PosX -= m_MoveX;
		m_MoveX = 5.0f * m_bReverse ? -1 : 1;
	}

	int GetMoveX() { return m_MoveX; }

	CRectangle GetInitRect() {
		return CRectangle(m_InitX + ENEMY_RECTDECREASE, m_InitY + ENEMY_RECTDECREASE, m_InitX + m_SrcRect.GetWidth() - ENEMY_RECTDECREASE, m_InitY + m_SrcRect.GetHeight());
	}

	void Repop()
	{
		Initialize(m_InitX, m_InitY, m_Type);
	}
};
