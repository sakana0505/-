#include	"WaterPlace.h"

/**
 * �R���X�g���N�^
 *
 */
CWaterPlace::CWaterPlace() :
	m_pWPTexture(NULL),
	m_Motion(),
	m_PosX(0.0f),
	m_PosY(0.0f),
	m_bShow(false),
	m_SrcRect() {
}

/**
 * �f�X�g���N�^
 *
 */
CWaterPlace::~CWaterPlace() {
	//delete[] �ŉ�������悤��Release���f�X�g���N�^�Ŏ��s����
	Release();
}

/**
 * ������
 * �p�����[�^�[����W������������B
 *
 * ����
 * [in]			px					X���W
 * [in]			py					Y���W
 * [in]			type				�G�^�C�v
 */
void CWaterPlace::Initialize(float px, float py, int type) {
	m_Type = type;
	m_PosX = px;
	m_PosY = py;
	m_bShow = true;
	//�A�j���[�V�������쐬
	SpriteAnimationCreate anim = {
		"����",
		0,0,
		96,96,
		TRUE,{{5,0,0}}
	};
	m_Motion.Create(anim);
}

/**
 * �X�V
 *
 */
void CWaterPlace::Update(void) {
	//��\��
	if (!m_bShow)
	{
		return;
	}

	//�A�j���[�V�����̍X�V
	m_Motion.AddTimer(CUtilities::GetFrameSecond());
	m_SrcRect = m_Motion.GetSrcRect();
}

/**
 * �X�e�[�W�Ƃ̓�����
 *
 * ����
 * [in]			ox					X���܂��
 * [in]			oy					Y���܂��
 */
void CWaterPlace::CollisionStage(float ox, float oy) {
	m_PosX += ox;
	m_PosY += oy;

}

/**
 * �`��
 *
 * ����
 * [in]			wx					���[���h�̕ω�
 * [in]			wy					���[���h�̕ω�
 */
void CWaterPlace::Render(float wx, float wy) {
	//��\��
	if (!m_bShow)
	{
		return;
	}
	//�e�N�X�`���̕`��
	m_pWPTexture->Render(m_PosX - wx, m_PosY - wy, m_SrcRect);
}

/**
 * �f�o�b�O�`��
 *
 * ����
 * [in]			wx					���[���h�̕ω�
 * [in]			wy					���[���h�̕ω�
 */
void CWaterPlace::RenderDebug(float wx, float wy) {
	//��\��
	if (!m_bShow)
	{
		return;
	}
	//�����蔻��̕\��
	CRectangle hr = GetRect();
	CGraphicsUtilities::RenderRect(hr.Left - wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(255, 0, 0));
}

/**
 * ���
 *
 */
void CWaterPlace::Release(void) {
	m_Motion.Release();
}
