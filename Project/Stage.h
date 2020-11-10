#pragma once
#include "Mof.h"
#include	"Player.h"
#include	"WaterPlace.h"

//ç∂åXéŒÉ`ÉbÉv
#define		LEFTSLOPE							2

class CStage {
private:
	CTexture				m_ChipTexture;
	CTexture				m_BackTexture;

	float					m_ChipSize;
	int						m_XCount;
	int						m_YCount;
	char*					m_pChipData;

	float					m_ScrollX;
	float					m_ScrollY;

	int						m_EnemyTextureCount;
	CTexture*				m_pEnemyTexture;
	char*					m_pEnemyData;
	int						m_EnemyCount;

	int						m_WaterPlaceTextureCount;
	CTexture*				m_pWaterPlaceTexture;
	char*					m_pWaterPlaceData;
	int						m_WaterPlaceCount;

public:
	CStage();
	~CStage();
	bool Load(char* pName);
	void Initialize(CWaterPlace* pWaterPlace);
	void Update(CPlayer& pl);
	bool Collision(CRectangle r, float& ox, float& oy);
	void Render(void);
	void RenderDebug(void);
	void Release(void);
	float GetScrollX() { return m_ScrollX; }
	float GetScrollY() { return m_ScrollY; }
	int GetEnemyCount() { return m_EnemyCount; }
	int GetWaterPlaceCount() { return m_WaterPlaceCount; }



	bool IsGround(float px, float py);
};