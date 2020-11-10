#include "GameOver.h"
#include	"GameDefine.h"
#include	"GameApp.h"

//�ύX����V�[��(�O���Q�ƁA���̂�GameApp.cpp)
extern int						gChangeScene;
//�Q�[���I�[�o�[BGM
CSoundBuffer			gSound_GameOver;

/**
 * �R���X�g���N�^
 *
 */
CGameOver::CGameOver() :
	m_bEnd(false) {
}

/**
 * �f�X�g���N�^
 *
 */
CGameOver::~CGameOver() {
}

/**
 * �ǂݍ���
 * ���p����e�N�X�`����ǂݍ��ށB
 */
bool CGameOver::Load(void) {
	return true;
}
/**
 * ������
 * �p�����[�^�[����W������������B
 * ��Ԃ��������������Ƃ��Ɏ��s����B
 */
void CGameOver::Initialize(void) {
	CUtilities::SetCurrentDirectory("Resource");
	//���f�ޓǂݍ���
	gSound_GameOver.Load("in_dream.mp3");
	//���[�v�ݒ�
	gSound_GameOver.SetLoop(TRUE);
	//BGM�Đ�
	//gSound_GameOver.Play();						//�J�����͂��邳���̂ŃR�����g�A�E�g
}

/**
 * �X�V
 *
 */
void CGameOver::Update(void) {
	//Enter�L�[�Ń^�C�g����ʂ�
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) && !m_bEnd)
	{
		gChangeScene = SCENENO_TITLE;
		gSound_GameOver.Stop();
	}

}

/**
 * �`��
 *
 */
void CGameOver::Render(void) {
	CGraphicsUtilities::RenderString(400, 500, MOF_COLOR_WHITE, "�Q�[���I�[�o�[\n���͂���΂�");
}

/**
 * �f�o�b�O�`��
 *
 */
void CGameOver::RenderDebug(void) {
}


/**
 * ���
 *
 */
void CGameOver::Release(void) {
	//���̉��
	gSound_GameOver.Release();
	
}
