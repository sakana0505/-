#pragma once

#include	"Mof.h"
#include	"GameDefine.h"

class CWaterPlace {
private:
	CTexture*				m_pWPTexture;
	CSpriteMotionController	m_Motion;
	int						m_Type;
	float					m_PosX;
	float					m_PosY;
	bool					m_bShow;
	CRectangle				m_SrcRect;
public:
	CWaterPlace();
	~CWaterPlace();
	void Initialize(float px, float py, int type);
	void Update(void);
	void CollisionStage(float ox, float oy);
	void Render(float wx, float wy);
	void RenderDebug(float wx, float wy);
	void Release(void);
	void SetTexture(CTexture* pt) { m_pWPTexture = pt; }
	bool GetShow(void) { return m_bShow; }
	void SetShow(bool bs) { m_bShow = bs; }
	int GetType(void) { return m_Type; }
	CRectangle GetRect() { return CRectangle(m_PosX, m_PosY, m_PosX + m_SrcRect.GetWidth(), m_PosY + m_SrcRect.GetHeight()); }
};