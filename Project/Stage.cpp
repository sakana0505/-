#include "Stage.h"
#include	"Stage.h"

/**
 * �R���X�g���N�^
 *
 */
CStage::CStage() :
	m_ChipTexture(),
	m_BackTexture(),
	m_ChipSize(0),
	m_XCount(0),
	m_YCount(0),
	m_pChipData(NULL),
	m_ScrollX(0),
	m_ScrollY(0),
	m_EnemyTextureCount(0),
	m_pEnemyTexture(NULL),
	m_EnemyCount(0),
	m_WaterPlaceTextureCount(0),
	m_pWaterPlaceTexture(NULL),
	m_WaterPlaceCount(0) {
}

/**
 * �f�X�g���N�^
 *
 */
CStage::~CStage() {
}

/**
 * �ǂݍ���
 * ���p����e�N�X�`���ƃX�e�[�W�t�@�C����ǂݍ��ށB
 *
 * ����
 * [in]			pName				�X�e�[�W�t�@�C���̖��O
 */
bool CStage::Load(char* pName) {
	//�e�L�X�g�t�@�C�����J��
	FILE* fp = fopen(pName, "rt");
	if (fp == NULL)
	{
		return false;
	}
	//�t�@�C���̑S�e�ʂ𒲂ׂ�
	//��x�t�@�C���̏I�[�Ɉړ����āA���̈ʒu�𒲂ׂ邱�ƂŃT�C�Y���킩��
	//���׏I�������̓t�@�C���̐擪�Ɉړ����Ė߂��Ă���
	//���������̕��@�ł�����ȃT�C�Y��ǂݍ��ނ��Ƃ͂ł��Ȃ�
	fseek(fp, 0, SEEK_END);
	long fSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	//�t�@�C���T�C�Y�������̃��������m�ۂ���
	//�I�[���܂߂邽�߁{�P���Ă���
	char* pBuffer = (char*)malloc(fSize + 1);
	//�t�@�C����S�ăo�b�t�@�ɓǂݍ���
	//�����Ŏ��ۂɓǂݍ��߂��T�C�Y�𒲂ׂĂ��̏I�[��\0�����
	fSize = fread(pBuffer, 1, fSize, fp);
	pBuffer[fSize] = '\0';
	char* pstr;

	//�e�N�X�`���̓ǂݍ���
	pstr = strtok(pBuffer, ",");
	if (!m_BackTexture.Load(pstr))
	{
		return false;
	}
	pstr = strtok(NULL, ",");
	if (!m_ChipTexture.Load(pstr))
	{
		return false;
	}

	//�`�b�v�T�C�Y
	pstr = strtok(NULL, ",");
	m_ChipSize = atof(pstr);

	//�}�b�v�̃`�b�v��
	pstr = strtok(NULL, ",");
	m_XCount = atoi(pstr);
	pstr = strtok(NULL, ",");
	m_YCount = atoi(pstr);

	//�}�b�v�`�b�v�p�̃������m��
	m_pChipData = (char*)malloc(m_XCount * m_YCount);

	//�`�b�v�f�[�^�̓ǂݍ���
	for (int y = 0; y < m_YCount; y++)
	{
		for (int x = 0; x < m_XCount; x++)
		{
			pstr = strtok(NULL, ",");
			m_pChipData[y * m_XCount + x] = atoi(pstr);
		}
	}

	////�G�̃e�N�X�`���ǂݍ���
	//pstr = strtok(NULL, ",");
	//m_EnemyTextureCount = atoi(pstr);
	//m_pEnemyTexture = new CTexture[m_EnemyTextureCount];
	//for (int i = 0; i < m_EnemyTextureCount; i++)
	//{
	//	pstr = strtok(NULL, ",");
	//	if (!m_pEnemyTexture[i].Load(pstr))
	//	{
	//		return false;
	//	}
	//}

	////�z�u�f�[�^�̓ǂݍ���
	//m_pEnemyData = (char*)malloc(m_XCount * m_YCount);
	//m_EnemyCount = 0;
	//for (int y = 0; y < m_YCount; y++)
	//{
	//	for (int x = 0; x < m_XCount; x++)
	//	{
	//		pstr = strtok(NULL, ",");
	//		m_pEnemyData[y * m_XCount + x] = atoi(pstr);
	//		if (m_pEnemyData[y * m_XCount + x] > 0)
	//		{
	//			m_EnemyCount++;
	//		}
	//	}
	//}

	//�A�C�e���̃e�N�X�`���ǂݍ���
	pstr = strtok(NULL, ",");
	m_WaterPlaceTextureCount = atoi(pstr);
	m_pWaterPlaceTexture = new CTexture[m_WaterPlaceTextureCount];
	for (int i = 0; i < m_WaterPlaceTextureCount; i++)
	{
		pstr = strtok(NULL, ",");
		if (!m_pWaterPlaceTexture[i].Load(pstr))
		{
			return false;
		}
	}

	//�z�u�f�[�^�̓ǂݍ���
	m_pWaterPlaceData = (char*)malloc(m_XCount * m_YCount);
	m_WaterPlaceCount = 0;
	for (int y = 0; y < m_YCount; y++)
	{
		for (int x = 0; x < m_XCount; x++)
		{
			pstr = strtok(NULL, ",");
			m_pWaterPlaceData[y * m_XCount + x] = atoi(pstr);
			if (m_pWaterPlaceData[y * m_XCount + x] > 0)
			{
				m_WaterPlaceCount++;
			}
		}
	}

	//�t�@�C�������
	fclose(fp);
	free(pBuffer);
	return true;
}

/**
 * ������
 * �p�����[�^�[����W������������B
 *
 * ����
 * [in]			pEnemy				�G�̔z��
 * [in]			pItem				�A�C�e���z��
 */
void CStage::Initialize(CWaterPlace* pWaterPlace) {
	m_ScrollX = 0;
	m_ScrollY = 0;
	int n = 0;
	//for (int y = 0; y < m_YCount; y++)
	//{
	//	for (int x = 0; x < m_XCount; x++)
	//	{
	//		//�z�u�ԍ�
	//		//�ԍ��O�͔z�u���Ȃ�
	//		/*char on = m_pEnemyData[y * m_XCount + x] - 1;
	//		if (on < 0)
	//		{
	//			continue;
	//		}*/
	//	}
	//}
	/*n = 0;*/
	for (int y = 0; y < m_YCount; y++)
	{
		for (int x = 0; x < m_XCount; x++)
		{
			//�z�u�ԍ�
			//�ԍ��O�͔z�u���Ȃ�
			char on = m_pWaterPlaceData[y * m_XCount + x] - 1;
			if (on < 0)
			{
				continue;
			}
			pWaterPlace[n].SetTexture(&m_pWaterPlaceTexture[on]);
			pWaterPlace[n++].Initialize(x * m_ChipSize, y * m_ChipSize, on);
		}
	}
}

/**
 * �X�V
 *
 * ����
 * [in]			pl					�v���C���[�A�X�N���[���̔���Ɏg�p
 */
void CStage::Update(CPlayer& pl) {
	//�v���C���[�̋�`�擾
	CRectangle prec = pl.GetRect();
	//�X�N���[���̕�
	float sw = CGraphicsUtilities::GetGraphics()->GetTargetWidth();
	//�X�e�[�W�S�̂̕�
	float stgw = m_ChipSize * m_XCount;
	//���W����ʒ[�ɂ���Ă���i�e�[����200pixel�j�ꍇ�X�N���[�����s���ĕ␳����
	if (prec.Left - m_ScrollX < 200)
	{
		m_ScrollX -= 200 - (prec.Left - m_ScrollX);
		if (m_ScrollX <= 0)
		{
			m_ScrollX = 0;
		}
	}
	else if (prec.Right - m_ScrollX > sw - 200)
	{
		m_ScrollX += (prec.Right - m_ScrollX) - (sw - 200);
		if (m_ScrollX >= stgw - sw)
		{
			m_ScrollX = stgw - sw;
		}
	}
}

/**
 * �����蔻��
 *
 * ����
 * [in]			r					�����`
 * [out]		ox					X���܂�
 * [out]		oy					Y���܂�
 */
bool CStage::Collision(CRectangle r, float& ox, float& oy) {
	bool re = false;

	//�����蔻�肷���`�̍���ƉE���̃`�b�v�ʒu�����߂�
	int lc = r.Left / m_ChipSize;
	int rc = r.Right / m_ChipSize;
	int tc = r.Top / m_ChipSize;
	int bc = r.Bottom / m_ChipSize;
	//�X�e�[�W�͈̔͊O�ɂ͂Ȃ�Ȃ��悤�ɂ���
	if (lc < 0)
	{
		lc = 0;
	}
	if (tc < 0)
	{
		tc = 0;
	}
	if (rc >= m_XCount)
	{
		rc = m_XCount - 1;
	}
	if (bc >= m_YCount)
	{
		bc = m_YCount - 1;
	}

	//�����蔻��������`�̍��ォ��E���͈̔͂̂ݓ����蔻��������Ȃ�
	//����ȊO�̔ԍ��͓����邱�Ƃ͂Ȃ��̂Ŕ��肪�K�v�Ȃ�
	for (int y = tc; y <= bc; y++)
	{
		for (int x = lc; x <= rc; x++)
		{
			//�`�悷��`�b�v�ԍ�
			//�`�b�v�ԍ��O�͓����蔻�肵�Ȃ�
			char cn = m_pChipData[y * m_XCount + x] - 1;
			if (cn < 0)
			{
				continue;
			}
			//�}�b�v�`�b�v�̋�`
			CRectangle cr(x * m_ChipSize, y * m_ChipSize, x * m_ChipSize + m_ChipSize, y * m_ChipSize + m_ChipSize);
			//�����蔻��p�̃L�����N�^��`
			//���Ŕ͈͂����肵����p�̋�`���쐬����B
			CRectangle brec = r;
			brec.Top = brec.Bottom - 1;	//���̋�`�͏㑤�����Ɠ����l�ɂ���
			brec.Expansion(-6, 0);		//���͈̔͂��������߂�
			//���Ɠ����蔻��
			if (cr.CollisionRect(brec))
			{
				re = true;
				//�`�b�v���Ζʂ̏ꍇ�����̗����ʒu���獂���̊��������߂�
				if (cn == LEFTSLOPE)
				{
					float sp = (brec.Right - cr.Left) / cr.GetWidth();
					if (sp < 0.0f)
					{
						sp = 0.0f;
					}
					else if (sp > 1.0f)
					{
						sp = 1.0f;
					}
					//�Ζʂ̏�̈ʒu�����߂�
					float cTop = cr.Bottom - cr.GetHeight() * sp;
					if (brec.Bottom < cTop)
					{
						continue;
					}
					oy += cTop - brec.Bottom;
					r.Top += cTop - brec.Bottom;
					r.Bottom += cTop - brec.Bottom;
				}
				else
				{
					//���̖��܂�Ȃ̂Ń`�b�v�̏�[�����`�̉��[�̒l���������l�����܂�̒l�ɂȂ�
					oy += cr.Top - brec.Bottom;
					r.Top += cr.Top - brec.Bottom;
					r.Bottom += cr.Top - brec.Bottom;
				}
			}
			//�`�b�v���Ζʂ̏ꍇ�ǁE�V��Ƃ��Ă̓����蔻��������Ȃ�Ȃ�
			if (cn != LEFTSLOPE)
			{
				//�����蔻��p�̃L�����N�^��`
				//���A�E���ꂼ��Ŕ͈͂����肵����p�̋�`���쐬����B
				CRectangle lrec = r;
				lrec.Right = lrec.Left + 1;	//���̋�`�͉E�������Ɠ����l�ɂ���
				lrec.Expansion(0, -6);		//�c�͈̔͂��������߂�
				CRectangle rrec = r;
				rrec.Left = rrec.Right - 1;	//�E�̋�`�͍������E�Ɠ����l�ɂ���
				rrec.Expansion(0, -6);		//�c�͈̔͂��������߂�
				//���Ɠ����蔻��
				if (cr.CollisionRect(lrec))
				{
					re = true;
					//���̖��܂�Ȃ̂Ń`�b�v�̉E�[�����`�̍��[�̒l���������l�����܂�̒l�ɂȂ�
					ox += cr.Right - lrec.Left;
					r.Left += cr.Right - lrec.Left;
					r.Right += cr.Right - lrec.Left;
				}
				//�E�Ɠ����蔻��
				else if (cr.CollisionRect(rrec))
				{
					re = true;
					//�E�̖��܂�Ȃ̂Ń`�b�v�̍��[�����`�̉E�[�̒l���������l�����܂�̒l�ɂȂ�
					ox += cr.Left - rrec.Right;
					r.Left += cr.Left - rrec.Right;
					r.Right += cr.Left - rrec.Right;
				}
				//�����蔻��p�̃L�����N�^��`
				//��Ŕ͈͂����肵����p�̋�`���쐬����B
				CRectangle trec = r;
				trec.Bottom = trec.Top + 1;	//��̋�`�͉�������Ɠ����l�ɂ���
				trec.Expansion(-6, 0);		//���͈̔͂��������߂�
				//��Ɠ����蔻��
				if (cr.CollisionRect(trec))
				{
					re = true;
					//��̖��܂�Ȃ̂Ń`�b�v�̉��[�����`�̏�[�̒l���������l�����܂�̒l�ɂȂ�
					oy += cr.Bottom - trec.Top;
					r.Top += cr.Bottom - trec.Top;
					r.Bottom += cr.Bottom - trec.Top;
				}
			}
		}
	}

	return re;
}





bool CStage::IsGround(float px, float py)
{
	bool re = false;

	//�����蔻�肷���`�̍���ƉE���̃`�b�v�ʒu�����߂�
	int x = px / m_ChipSize;
	int y = py / m_ChipSize;

	//�X�e�[�W�͈̔͊O�ɂ͂Ȃ�Ȃ��悤�ɂ���
	if (x < 0 || x >= m_XCount || y < 0 || y >= m_YCount)
	{
		return false;
	}

	//�`�悷��`�b�v�ԍ�
	//�`�b�v�ԍ��O�͓����蔻�肵�Ȃ�
	char cn = m_pChipData[y * m_XCount + x] - 1;
	if (cn < 0)
	{
		return false;
	}

	return true;
}


/**
 * �`��
 *
 */
void CStage::Render(void) {
	//���i�̕`��
	int scw = g_pGraphics->GetTargetWidth();
	int sch = g_pGraphics->GetTargetHeight();
	int wn = m_BackTexture.GetWidth();
	int hn = m_BackTexture.GetHeight();
	for (float y = ((int)-m_ScrollY % hn) - hn; y < sch; y += hn)
	{
		for (float x = ((int)-m_ScrollX % wn) - wn; x < scw; x += wn)
		{
			m_BackTexture.Render(x, y);
		}
	}

	//�e�N�X�`���̉�������}�b�v�`�b�v�̏c�I�t�Z�b�g�����߂�
	int tcx = m_ChipTexture.GetWidth() / m_ChipSize;
	//�}�b�v�`�b�v�̕`��
	for (int y = 0; y < m_YCount; y++)
	{
		for (int x = 0; x < m_XCount; x++)
		{
			//�`�悷��`�b�v�ԍ�
			//�`�b�v�ԍ��O�͕`�悵�Ȃ�
			char cn = m_pChipData[y * m_XCount + x] - 1;
			if (cn < 0)
			{
				continue;
			}
			//�}�b�v�`�b�v�̋�`
			CRectangle cr(m_ChipSize * (cn % tcx), m_ChipSize * (cn / tcx), m_ChipSize * (cn % tcx + 1), m_ChipSize * (cn / tcx + 1));
			//�}�b�v�`�b�v�̕`��
			m_ChipTexture.Render(-m_ScrollX + x * m_ChipSize, -m_ScrollY + y * m_ChipSize, cr);
		}
	}
}

/**
 * �f�o�b�O�`��
 *
 */
void CStage::RenderDebug(void) {
	//�ʒu�̕`��
	CGraphicsUtilities::RenderString(10, 100, "�X�N���[�� X : %.0f , Y : %.0f", m_ScrollX, m_ScrollY);
}

/**
 * ���
 *
 */
void CStage::Release(void) {
	m_ChipTexture.Release();
	m_BackTexture.Release();
	if (m_pChipData)
	{
		free(m_pChipData);
		m_pChipData = NULL;
	}
	/*if (m_pEnemyData)
	{
		free(m_pEnemyData);
		m_pEnemyData = NULL;
	}
	if (m_pEnemyTexture)
	{
		delete[] m_pEnemyTexture;
		m_pEnemyTexture = NULL;
	}*/
	if (m_pWaterPlaceData)
	{
		free(m_pWaterPlaceData);
		m_pWaterPlaceData = NULL;
	}
	if (m_pWaterPlaceTexture)
	{
		delete[] m_pWaterPlaceTexture;
		m_pWaterPlaceTexture = NULL;
	}
}
