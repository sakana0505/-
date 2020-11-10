#pragma once
#include "Mof.h"
#include	"Stage.h"
#include	"Player.h"
#include	"WaterPlace.h"

	class CGame {
	private:
		//背景画像
		CTexture				m_BackImage;

		int						m_Alpha;
		bool					m_bEnd;

		//ステージ
		CStage		m_Stage;
		//プレイヤー
		CPlayer		m_Player;
		//水場
		CWaterPlace*		m_WaterPlaceArray;
	public:
		CGame();
		~CGame();
		bool Load(void);
		void Initialize(void);
		void Update(void);
		void Render(void);
		void RenderDebug(void);
		void Release(void);
};

