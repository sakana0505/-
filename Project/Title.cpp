#include "Title.h"
#include "GameDefine.h"

//�ύX����V�[��(�O���Q�ƁA���̂�GameApp.cpp)
extern int						gChangeScene;
//���j���[���BGM
CSoundBuffer			gSound_Menu;

/**			
*�R���X�g���N�^
*
*/
CTitle::CTitle() :
	m_BackImage(),
    m_Alpha(255),
	m_bEnd(false){
}

/**
 * �f�X�g���N�^
 *
 */
CTitle::~CTitle() {
}

/**
 * �ǂݍ���
 * ���p����e�N�X�`����ǂݍ��ށB
 */
bool CTitle::Load(void) {
	//�e�N�X�`���̓ǂݍ���
	if (!m_BackImage.Load("Title.png"))
	{
		return false;
	}
	return true;
	
}
/**
 * ������
 * �p�����[�^�[����W������������B
 * ��Ԃ��������������Ƃ��Ɏ��s����B
 */
void CTitle::Initialize(void) {
	m_bEnd = false;
	m_Alpha = 0;

	//���f�ޓǂݍ���
	gSound_Menu.Load("Hometown.mp3");
	//���[�v�ݒ�
	gSound_Menu.SetLoop(TRUE);
	//BGM�Đ�
	//gSound_Menu.Play();					//�J�����͂��邳���̂ŃR�����g�A�E�g
}

/**
 * �X�V
 *
 */
void CTitle::Update(void) {
	//�I����ԂȂ�t�F�[�h�A�E�g
	if (m_bEnd)
	{
		m_Alpha -= 5;
		if (m_Alpha <= 0)
		{
			gChangeScene = SCENENO_GAME;
		}
	}
	else
	{
		if (m_Alpha < 255)
		{
			m_Alpha += 5;
		}
	}
	//Enter�L�[�ŃQ�[����ʂ�
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) && !m_bEnd)
	{
		m_bEnd = true;
		gSound_Menu.Stop();
	}


}

/**
 * �`��
 *
 */
void CTitle::Render(void) {
	m_BackImage.Render(0,0,MOF_ARGB(m_Alpha,255,255,255));
	CGraphicsUtilities::RenderString(400, 500, MOF_COLOR_BLACK, "Press Enter Key");
}

/**
 * �f�o�b�O�`��
 *
 */
void CTitle::RenderDebug(void) {
}

/**
 * ���
 *
 */
void CTitle::Release(void) {
	m_BackImage.Release();
	//���̉��
	gSound_Menu.Release();
}
