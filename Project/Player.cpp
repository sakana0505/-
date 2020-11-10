#include "Player.h"

//MP�񕜃f�B���C
#define		MPRECOV_WAIT		25

/**
 * �R���X�g���N�^
 *
 */
CPlayer::CPlayer() :
	m_Texture(),
	m_Motion(),
	m_PosX(0.0f),
	m_PosY(0.0f),
	m_bMove(false),
	m_MoveX(0.0f),
	m_MoveY(0.0f),
	m_bJump(false),
	m_bReverse(false),
	m_SrcRect(),
	m_HP(5),
	m_MP(10),
	m_MPPlus(0),
	m_MPRecovWait(),
	m_DamageWait(0),
	m_FrameTexture(),
	m_HPTexture(),
	m_bEnd(false),
	m_bGoal(false),
	m_AttackPhase(0) {
}

/**
 * �f�X�g���N�^
 *
 */
CPlayer::~CPlayer() {
}

/**
 * �ǂݍ���
 * ���p����e�N�X�`����ǂݍ��ށB
 */
bool CPlayer::Load(void) {

	//�e�N�X�`���̓ǂݍ���
	if (!m_Texture.Load("player.png"))
	{
		return false;
	}
	if (!m_FrameTexture.Load("UI.png"))
	{
		return false;
	}
	if (!m_HPTexture.Load("HPHeart.png"))
	{
		return false;
	}
	if (!m_MPTexture.Load("waterFuel.png"))
	{
		return false;
	}
	if (!m_MPEmpTexture.Load("waterEnpty_1.png"))
	{
		return false;
	}
	//�A�j���[�V�������쐬
	SpriteAnimationCreate anim[] = {
		//�ҋ@
		{
			"�ҋ@",
			0,0,
			60,64,
			TRUE,{{5,0,0},{5,1,0},{5,2,0},{5,3,0},{5,4,0},{5,5,0},{5,6,0},{5,7,0}}
		},
		//�ړ�
		{
			"�ړ�",
			0,70,
			60,64,
			TRUE,{{5,0,0},{5,1,0},{5,2,0},{5,3,0},{5,4,0},{5,5,0}}
		},
		//�W�����v
		{
			"�W�����v�J�n",
			0,140,
			60,64,
			FALSE,{{5,0,0},{5,1,0},{5,2,0},{5,3,0}}
		},
		{
			"�W�����v�I��",
			240,140,
			60,64,
			FALSE,{{2,0,0},{2,1,0}}
		},
		//�U��
		{
			"�U��",
			0, 350,
			90, 64,
			FALSE, { { 2, 0, 0 }, { 2, 1, 0 }, { 2, 2, 0 }, { 2, 3, 0 }, { 2, 4, 0 }, { 2, 5, 0 }, { 2, 6, 0 } }
		},
		//�U��2
		{
			"�U��2",
			0, 420,
			90, 64,
			FALSE, { { 2, 0, 0 }, { 2, 1, 0 }, { 2, 2, 0 }, { 2, 3, 0 }, { 2, 4, 0 }, { 2, 5, 0 }, { 2, 6, 0 } }
		},
		//�_���[�W
		{
			"�_���[�W",
			480,0,
			60,64,
			FALSE,{{20,0,0}}
		},

	};
	m_Motion.Create(anim, MOTION_COUNT);
	return true;
}

/**
 * ������
 * �p�����[�^�[����W������������B
 * �v���C���[�̈ʒu�ȂǏ�Ԃ��������������Ƃ��Ɏ��s����B
 */
void CPlayer::Initialize(void) {
	m_PosX = 200;
	m_PosY = 0;
	m_bMove = false;
	m_MoveX = 0.0f;
	m_MoveY = 0.0f;
	m_bReverse = false;
	m_bJump = false;
	m_HP = 5;
	m_MP = 10;
	m_MPPlus = 0;
	m_WaterPlaceflg = false;
	m_Level = 1;
	m_DamageWait = 0;
	m_bEnd = false;
	m_bGoal = false;
	m_bAttack = false;
	m_Motion.ChangeMotion(MOTION_WAIT);
}

/**
 * �X�V
 *
 */
void CPlayer::Update(void) {
	//HP���Ȃ��Ȃ�ƏI��
	if (m_HP <= 0)
	{
		m_bEnd = true;
		return;
	}



	//�ړ��t���O�A���̃t���[���ł̈ړ�������������ۑ�
	m_bMove = false;

	//�U�����A���n�̏ꍇ�̓���
	if (m_Motion.GetMotionNo() == MOTION_ATTACK2 || m_Motion.GetMotionNo() == MOTION_JUMPEND)
	{
		//�I���őҋ@�ɖ߂�
		if (m_Motion.IsEndMotion())
		{
			m_bAttack = false;
			m_Motion.ChangeMotion(MOTION_WAIT);
		}
	}
	else if (m_Motion.GetMotionNo() == MOTION_ATTACK && !m_bAttack)
	{
		if (g_pInput->IsKeyPush(MOFKEY_SPACE))
		{
			m_bAttack = true;
		}
		//�I���őҋ@�ɖ߂�
		if (m_Motion.IsEndMotion() && !m_bAttack)
		{
			m_Motion.ChangeMotion(MOTION_WAIT);
		}
	}


	//�_���[�W���̓���
	else if (m_Motion.GetMotionNo() == MOTION_DAMAGE)
	{
		//�I���őҋ@�ɖ߂�
		if (m_Motion.IsEndMotion())
		{
			m_Motion.ChangeMotion(MOTION_WAIT);
		}
	}
	else
	{
		UpdateKey();
	}

	//�ړ��X�V
	UpdateMove();
	//���ۂɍ��W���ړ�������
	m_PosX += m_MoveX;
	m_PosY += m_MoveY;

	//��ʊO�ŗ����Ƃ���HP���O�ɂ���
	if (m_PosY >= g_pGraphics->GetTargetHeight() && m_HP > 0)
	{
		m_HP = 0;
	}
	//�A�j���[�V�����̍X�V
	m_Motion.AddTimer(CUtilities::GetFrameSecond());
	m_SrcRect = m_Motion.GetSrcRect();
	//�_���[�W�̃C���^�[�o�������炷
	if (m_DamageWait > 0)
	{
		m_DamageWait--;
	}
}

/**
 * �L�[���͂ɂ�铮��X�V
 *
 */
void CPlayer::UpdateKey(void) {
	//�L�[�{�[�h�ł̈ړ�
	//���͂Œ��ڍ��W�𓮂����̂ł͂Ȃ��A���x��ω�������B
	//�U�����͈ړ��ł��Ȃ��悤�ɂ���

	if (g_pInput->IsKeyHold(MOFKEY_LEFT))
	{
		m_MoveX -= PLAYER_SPEED;
		m_bReverse = true;
		m_bMove = true;
		if (m_MoveX < -PLAYER_MAXSPEED)
		{
			m_MoveX = -PLAYER_MAXSPEED;
		}
		if (m_Motion.GetMotionNo() == MOTION_WAIT)
		{
			m_Motion.ChangeMotion(MOTION_MOVE);
		}
	}
	else if (g_pInput->IsKeyHold(MOFKEY_RIGHT))
	{
		m_MoveX += PLAYER_SPEED;
		m_bReverse = false;
		m_bMove = true;
		if (m_MoveX > PLAYER_MAXSPEED)
		{
			m_MoveX = PLAYER_MAXSPEED;
		}
		if (m_Motion.GetMotionNo() == MOTION_WAIT)
		{
			m_Motion.ChangeMotion(MOTION_MOVE);
		}
	}
	//��L�[�ŃW�����v
	if (g_pInput->IsKeyHold(MOFKEY_UP) && !m_bJump)
	{
		m_bJump = true;
		m_MoveY = PLAYER_JUMP;
		m_Motion.ChangeMotion(MOTION_JUMPSTART);
	}
	//SPACE�L�[�ōU��
	if (g_pInput->IsKeyPush(MOFKEY_SPACE))
	{
		//MP�̌��Ǘ�
		//MP���O�ɂȂ�ƍU���ł��Ȃ��Ȃ�
		if (!m_MP <= 0)
		{
			if (m_MPPlus <= 0)
			{
				m_MP -= 1;
				m_Motion.ChangeMotion(MOTION_ATTACK);
				m_AttackPhase = 0;
			}
			else if(m_MPPlus > 0)
			{
				m_MPPlus -= 1;
				m_Motion.ChangeMotion(MOTION_ATTACK);
				m_AttackPhase = 0;
			}
		}

		return;
	}
	//�U���Q
	if (m_Motion.GetMotionNo() == MOTION_ATTACK && m_bAttack)
	{
		if (m_Motion.IsEndMotion())
		{
			m_Motion.ChangeMotion(MOTION_ATTACK2);
		}
		return;
	}

	//MP��
	//����̔�����ɋ��鎞
	//S�L�[�łT��
	if (m_WaterPlaceflg)
	{
		//�f�B���C�p����
		if(m_MPRecovWait <= 0)
		{ 
			if (g_pInput->IsKeyHold(MOFKEY_S))
			{
				//���x���R�̏ꍇ
				if (m_Level == 3)
				{
					//MP�P�O�`�Q�O
					if (m_MP >= 10 && m_MPPlus < 10)
					{
						m_MPPlus ++;
					}
					//MP�T�`�P�O
					else if (m_MP < 10)
					{
						m_MP ++;
					}
				}
				//���x���Q�̏ꍇ
				else if (m_Level == 2)
				{
					//MP�P�O�`�P�T
					if (m_MP >= 10 && m_MPPlus < 5)
					{
						m_MPPlus++;
					}
					//MP�T�`�P�O
					else if (m_MP < 10)
					{
						m_MP++;
					}
				}
				//���x���P�̏ꍇ
				else
				{
					//MP�T�`�P�O
					if (m_MP < 10)
					{
						m_MP ++;
					}
				}
				m_MPRecovWait = MPRECOV_WAIT;
			}
		}
		else
		{
			m_MPRecovWait--;
		}
	}


	//�f�o�b�O�p�v���C���[���x������
	//L�L�[�Ńv���C���[���x�����㏸������i�ő�R�j
	if (g_pInput->IsKeyPush(MOFKEY_L))
	{
		if (m_Level < 3)
		{
			//���x�����グ��
			m_Level += 1;

			//���x���̏㏸�ɍ��킹�ăX�e�[�^�X��ω�������
			//MP�ʕω�
			//MP�P�O�`�P�T
			if (m_MP >= 10 && m_MPPlus <= 5)
			{
				m_MPPlus += 5;
			}
			//MP�T�`�P�O
			else if (m_MP < 10 && m_MP > 5)
			{
				int i;
				i = 10 - m_MP;
				m_MP = 10;
				m_MPPlus = 5 - i;
			}
			//MP�T�ȉ�
			else if (m_MP <= 5)
			{
				m_MP += 5;
			}

		}
	}
}

/**
 * �ړ��X�V
 *
 */
void CPlayer::UpdateMove(void) {
	//���̃t���[���ł̈ړ����͂��Ȃ���Ό������������s����
	if (!m_bMove)
	{
		if (m_MoveX > 0)
		{
			m_MoveX -= PLAYER_SPEED;
			if (m_MoveX <= 0)
			{
				m_MoveX = 0;
			}
		}
		else if (m_MoveX < 0)
		{
			m_MoveX += PLAYER_SPEED;
			if (m_MoveX >= 0)
			{
				m_MoveX = 0;
			}
		}
		else if (m_Motion.GetMotionNo() == MOTION_MOVE)
		{
			m_Motion.ChangeMotion(MOTION_WAIT);
		}
	}
	//�d�͂ɂ�艺�ɏ�����������
	m_MoveY += GRAVITY;
	if (m_MoveY >= 20.0f)
	{
		m_MoveY = 20.0f;
	}
}

/**
 * �X�e�[�W�Ƃ̓�����
 *
 * ����
 * [in]			ox					X���܂��
 * [in]			oy					Y���܂��
 */
void CPlayer::CollisionStage(float ox, float oy) {
	m_PosX += ox;
	m_PosY += oy;
	//�������̉����܂�A�W�����v���̏㖄�܂�̏ꍇ�͈ړ�������������B
	if (oy < 0 && m_MoveY > 0)
	{
		m_MoveY = 0;
		if (m_bJump)
		{
			m_bJump = false;
			m_Motion.ChangeMotion(MOTION_JUMPEND);

		}
	}
	else if (oy > 0 && m_MoveY < 0)
	{
		m_MoveY = 0;
	}
	//���ړ����̍����܂�A�E�ړ����̉E���܂�̏ꍇ�͈ړ�������������B
	if (ox < 0 && m_MoveX > 0)
	{
		m_MoveX = 0;
		//CPopupMsg::GetInstance()->Add("wall.", GetRect().Left, GetRect().Top, 40, MOF_XRGB(255, 255, 255), MOF_XRGB(0, 160, 0));
	}
	else if (ox > 0 && m_MoveX < 0)
	{
		m_MoveX = 0;
		//CPopupMsg::GetInstance()->Add("wall.", GetRect().Left, GetRect().Top, 40, MOF_XRGB(255, 255, 255), MOF_XRGB(0, 160, 0));
	}
}

/**
 * �����蔻��
 * �����̓G�ɑ΂��ċ�`���m�̓����蔻������s����B
 * �������Ă����ꍇ�̓v���C���[�̏�Ԃ�ύX����
 *
 * ����
 * [in]			ene				������s���G
 *
 * �߂�l
 * �������Ă���ꍇ��true�A�������Ă��Ȃ����false��Ԃ�
 */
 //bool CPlayer::CollisionEnemy(CEnemy& ene) {
 //    if (!ene.GetShow())
 //    {
 //        return false;
 //    }
 //    HP���Ȃ��Ȃ�Ɠ����蔻�肵�Ȃ�
 //    if (m_HP <= 0)
 //    {
 //        return false;
 //    }
 //	�G��View��`�Ƃ̔���
 //	CRectangle prec = GetRect();
 //	CRectangle vrec = ene.GetViewRect();
 //	if (prec.CollisionRect(vrec))
 //	{
 //		ene.SetDash(true);
 //	}
 //	else
 //	{
 //		ene.SetDash(false);
 //	}
 //    �_���[�W���̂��ߓ����蔻��������Ȃ�Ȃ�
 //    if (m_DamageWait > 0 || ene.GetDamageWait() > 0)
 //    {
 //        return false;
 //    }
 //    �G�̋�`�Ǝ����̋�`�Ń_���[�W
 //    /*CRectangle prec = GetRect();*/
 //    CRectangle erec = ene.GetRect();
 //    if (prec.CollisionRect(erec))
 //    {
 //        m_HP -= 1;
 //        m_DamageWait = 60;
 //        if (prec.Left < erec.Left)
 //        {
 //            m_MoveX = -5.0f;
 //            m_bReverse = false;
 //        }
 //        else
 //        {
 //            m_MoveX = 5.0f;
 //            m_bReverse = true;
 //        }
 //        m_Motion.ChangeMotion(MOTION_DAMAGE);
 //        if (m_HP <= 0)
 //        {
 //            //�����G�t�F�N�g�𔭐�������
 //            m_pEndEffect = m_pEffectManager->Start(m_PosX + m_SrcRect.GetWidth() * 0.5f, m_PosY + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSION02);
 //        }
 //        else
 //        {
 //            //�_���[�W�G�t�F�N�g�𔭐�������
 //            m_pEffectManager->Start(m_PosX + m_SrcRect.GetWidth() * 0.5f, m_PosY + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
 //
 //            CPopupMsg::GetInstance()->Add("Damage!\n", GetRect().Left, GetRect().Top, 40, MOF_XRGB(255, 255, 255), MOF_XRGB(255, 0, 0));
 //        }
 //        return true;
 //    }
 //
 //
 //
 //    //�G�̍��G�͈͂ɋ��邩�ǂ���
 //    CRectangle esrec = ene.GetSearchRect();
 //    if (prec.CollisionRect(esrec))
 //    {
 //        ene.SetBoost();
 //    }
 //
 //
 //
 //
 //    �U�����̏ꍇ�̂ݍU���Ƃ̓����蔻������s
 //    if (m_Motion.GetMotionNo() != MOTION_ATTACK && m_Motion.GetMotionNo() != MOTION_ATTACK2)
 //    {
 //        return false;
 //    }
 //    �G�̋�`�Ǝ����̍U����`�œG���_���[�W
 //    prec = GetAttackRect();
 //    if (prec.CollisionRect(erec))
 //    {
 //        ene.Damage(5, m_bReverse);
 //        return true;
 //    }
 //
 //
 //    return false;
 //}

 /**
  * �����蔻��
  * �����̃A�C�e���ɑ΂��ċ�`���m�̓����蔻������s����B
  * �������Ă����ꍇ�̓v���C���[�̏�Ԃ�ύX����
  *
  * ����
  * [in]			itm				������s���A�C�e��
  *
  * �߂�l
  * �������Ă���ꍇ��true�A�������Ă��Ȃ����false��Ԃ�
  */
  /*bool CPlayer::CollisionItem(CItem& itm) {
	  if (!itm.GetShow())
	  {
		  return false;
	  }
	  //�A�C�e���̋�`�Ǝ����̋�`�œ����蔻��
	  CRectangle prec = GetRect();
	  CRectangle irec = itm.GetRect();
	  if (prec.CollisionRect(irec))
	  {
		  itm.SetShow(false);
		  switch (itm.GetType())
		  {
		  case ITEM_RECOVER:
			  m_HP += 30;
			  if (m_HP >= 100)
			  {
				  m_HP = 100;
			  }
			  break;
		  case ITEM_GOAL:
			  m_bGoal = true;
			  break;
		  }
		  return true;
	  }
	  return false;
  }*/

  /**
	* �����蔻��
	* �����̐���ɑ΂��ċ�`���m�̓����蔻������s����B
	* �������Ă����ꍇ�͐ݒ肵�Ă���t���O��TRUE�ɂ���B
	*
	* ����
	* [in]			wrp				������s������
	*
	* �߂�l
	* �������Ă���ꍇ��true��Ԃ�
	*/
	void CPlayer::CollisionWaterPlace(CWaterPlace& wrp) {

		//����̋�`�Ǝ����̋�`�œ����蔻��
		CRectangle prec = GetRect();
		CRectangle irec = wrp.GetRect();
		if (prec.CollisionRect(irec))
		{
			m_WaterPlaceflg = true;
		}
		else
		{
			m_WaterPlaceflg = false;
		}
	}

  /**
   * �`��
   *
   * ����
   * [in]			wx					���[���h�̕ω�
   * [in]			wy					���[���h�̕ω�
   */
void CPlayer::Render(float wx, float wy) {
	//�C���^�[�o��2�t���[�����Ƃɕ`������Ȃ�
	if (m_DamageWait % 4 >= 2)
	{
		return;
	}
	//HP���Ȃ��Ȃ�ƕ`�悵�Ȃ�
	if (m_HP <= 0)
	{
		return;
	}
	//�`���`
	CRectangle dr = m_SrcRect;
	//�`��ʒu
	float px = m_PosX - wx;
	float py = m_PosY - wy;
	//���]�t���O��ON�̏ꍇ�`���`�𔽓]������
	if (m_bReverse)
	{
		float tmp = dr.Right;
		dr.Right = dr.Left;
		dr.Left = tmp;
		if (m_Motion.GetMotionNo() == MOTION_ATTACK || m_Motion.GetMotionNo() == MOTION_ATTACK2)
		{
			px -= PLAYER_ATTACKWIDTH;
		}
	}
	//�e�N�X�`���̕`��
	m_Texture.Render(px, py, dr);
}

/**
 * �X�e�[�^�X�`��
 *
 */
void CPlayer::RenderStatus(void) {
	//HP�\��
	for (int i = 0; i < m_HP; i++)
	{
		m_HPTexture.RenderScale(150 + 40 * i, 10, 0.2f);
	}

	//MP�\��
	//��g�̕\��
	for (int i = 0; i < 10; i++)
	{
		m_MPEmpTexture.RenderScale(145 + 30 * i, 40, 0.06f);
	}
	//�ǉ��ʂ̋�g�̕\��
	if (m_Level == 2)
	{
		for (int i = 0; i < 5; i++)
		{
			m_MPEmpTexture.RenderScale(145 + 30 * i, 67, 0.06f);
		}
	}
	else if (m_Level == 3)
	{
		for (int i = 0; i < 10; i++)
		{
			m_MPEmpTexture.RenderScale(145 + 30 * i, 67, 0.06f);
		}
	}
	
	//MP�����ʁi�P�O�j�̕\��
	for (int i=0; i < m_MP; i++)
	{
		m_MPTexture.RenderScale(145 + 30 * i, 40, 0.06f);
	}
	//�ǉ��ʁi�T�`�P�O�j�̕\��
	if (m_MPPlus > 0)
	{
		for (int i = 0; i < m_MPPlus; i++)
		{
			m_MPTexture.RenderScale(145 + 30 * i, 67, 0.06f);
		}
		
	}

	//�t���[�����㕔�ɕ`��
	m_FrameTexture.RenderScale(0, 0,0.25f);
}

/**
 * �f�o�b�O�`��
 *
 * ����
 * [in]			wx					���[���h�̕ω�
 * [in]			wy					���[���h�̕ω�
 */
void CPlayer::RenderDebug(float wx, float wy) {
	//�ʒu�̕`��
	CGraphicsUtilities::RenderString(10, 70, "�v���C���[�ʒu X : %.0f , Y : %.0f", m_PosX, m_PosY);
	//�v���C���[�X�e�[�^�X�̕\��
	CGraphicsUtilities::RenderString(10, 150, "�v���C���[���x���F%d",m_Level);
	CGraphicsUtilities::RenderString(10, 170, "�v���C���[HP�F%d", m_HP);
	CGraphicsUtilities::RenderString(10, 190, "�v���C���[MP�F%d", m_MP + m_MPPlus);

	//�����蔻��̕\��
	CRectangle hr = GetRect();
	CGraphicsUtilities::RenderRect(hr.Left - wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(0, 255, 0));
	//�U���̓����蔻��\��
	if (IsAttack())
	{
		CRectangle hr = GetAttackRect();
		CGraphicsUtilities::RenderRect(hr.Left - wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(255, 0, 0));
	}
}

/**
 * ���
 *
 */
void CPlayer::Release(void) {
	m_Texture.Release();
	m_Motion.Release();
	m_FrameTexture.Release();
	m_HPTexture.Release();
	m_MPTexture.Release();
	m_MPEmpTexture.Release();
}
