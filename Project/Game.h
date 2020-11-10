#pragma once
#include "Mof.h"
#include	"Stage.h"
#include	"Player.h"
#include	"WaterPlace.h"

	class CGame {
	private:
		//�w�i�摜
		CTexture				m_BackImage;

		int						m_Alpha;
		bool					m_bEnd;

		//�X�e�[�W
		CStage		m_Stage;
		//�v���C���[
		CPlayer		m_Player;
		//����
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

