#include "GameDefine.h"
#include "Game.h"

//�ύX����V�[��(�O���Q�ƁA���̂�GameApp.cpp)
extern int						gChangeScene;
//�X�e�[�WBGM
CSoundBuffer			gSound_Stage;
/**
 * �R���X�g���N�^
 *
 */
CGame::CGame() :
	m_bEnd(false) {
}

/**
 * �f�X�g���N�^
 *
 */
CGame::~CGame() {
}

/**
 * �ǂݍ���
 * ���p����e�N�X�`����ǂݍ��ށB
 */
bool CGame::Load(void) {
	//�v���C���[�f�ޓǂݍ���
	m_Player.Load();
	//�X�e�[�W�̑f�ޓǂݍ���
	m_Stage.Load("Stage1.txt");

	//�A�C�e���������m��
	m_WaterPlaceArray = new CWaterPlace[m_Stage.GetWaterPlaceCount()];
	return true;
}
/**
 * ������
 * �p�����[�^�[����W������������B
 * ��Ԃ��������������Ƃ��Ɏ��s����B
 */
void CGame::Initialize(void) {
	//���\�[�X�z�u�f�B���N�g���̐ݒ�
	CUtilities::SetCurrentDirectory("Resource");
	//�v���C���[�̏�ԏ�����
	m_Player.Initialize();
	//�X�e�[�W�̏�ԏ�����
	m_Stage.Initialize(m_WaterPlaceArray);

	//���f�ޓǂݍ���
	gSound_Stage.Load("Harmless_3.mp3");
	//���[�v�ݒ�
	gSound_Stage.SetLoop(TRUE);
	//BGM�Đ�
	//gSound_Stage.Play();					//�J�����͂��邳���̂ŃR�����g�A�E�g
}

/**
 * �X�V
 *
 */
void CGame::Update(void) {
	//�v���C���[�̍X�V
	m_Player.Update();
	//�X�e�[�W�ƃv���C���[�̓����蔻��
	float ox = 0, oy = 0;
	if (m_Stage.Collision(m_Player.GetRect(), ox, oy))
	{
		m_Player.CollisionStage(ox, oy);
	}

	//�A�C�e���̍X�V
	for (int i = 0; i < m_Stage.GetWaterPlaceCount(); i++)
	{
		if (!m_WaterPlaceArray[i].GetShow())
		{
			continue;
		}
		m_WaterPlaceArray[i].Update();
		float ox = 0, oy = 0;
		if (m_Stage.Collision(m_WaterPlaceArray[i].GetRect(), ox, oy))
		{
			m_WaterPlaceArray[i].CollisionStage(ox, oy);
		}
	}
	//�����蔻��̎��s
	for (int i = 0; i < m_Stage.GetWaterPlaceCount(); i++)
	{
		m_Player.CollisionWaterPlace(m_WaterPlaceArray[i]);
	}

	//�X�e�[�W�̍X�V
	m_Stage.Update(m_Player);

	//Enter�L�[�ŃQ�[���I�[�o�[��ʂ�
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) && !m_bEnd)
	{
		gChangeScene = SCENENO_GAMEOVER;
		gSound_Stage.Stop();
	}

}

/**
 * �`��
 *
 */
void CGame::Render(void) {
	//�X�e�[�W�̕`��
	m_Stage.Render();
	CGraphicsUtilities::RenderString(400, 500, MOF_COLOR_WHITE, "�Q�[����ʂ��");
	//�A�C�e���̕`��
	for (int i = 0; i < m_Stage.GetWaterPlaceCount(); i++)
	{
		m_WaterPlaceArray[i].Render(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	}

	//�v���C���[�̕`��
	m_Player.Render(m_Stage.GetScrollX(), m_Stage.GetScrollY());

	

	//�v���C���[�̏�ԕ`��
	m_Player.RenderStatus();
}

/**
 * �f�o�b�O�`��
 *
 */
void CGame::RenderDebug(void) {
	//�X�e�[�W�̃f�o�b�O�`��
	m_Stage.RenderDebug();
	//�v���C���[�̃f�o�b�O�`��
	m_Player.RenderDebug(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	//�A�C�e���̃f�o�b�O�`��
	for (int i = 0; i < m_Stage.GetWaterPlaceCount(); i++)
	{
		m_WaterPlaceArray[i].RenderDebug(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	}
}



/**
 * ���
 *
 */
void CGame::Release(void) {
	//�X�e�[�W�̉��
	m_Stage.Release();
	//�v���C���[�̉��
	m_Player.Release();
	//���̉��
	gSound_Stage.Release();
	//�A�C�e���̉��
	if (m_WaterPlaceArray)
	{
		delete[] m_WaterPlaceArray;
		m_WaterPlaceArray = NULL;
	}
}
