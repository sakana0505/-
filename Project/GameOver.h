#pragma once
#include "Mof.h"

class CGameOver
{
private:
	CTexture				m_BackImage;
	int						m_Alpha;
	bool					m_bEnd;
public:
	CGameOver();
	~CGameOver();
	bool Load(void);
	void Initialize(void);
	void Update(void);
	void Render(void);
	void RenderDebug(void);
	void Release(void);
};


