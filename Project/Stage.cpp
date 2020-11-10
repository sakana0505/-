#include "Stage.h"
#include	"Stage.h"

/**
 * コンストラクタ
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
 * デストラクタ
 *
 */
CStage::~CStage() {
}

/**
 * 読み込み
 * 利用するテクスチャとステージファイルを読み込む。
 *
 * 引数
 * [in]			pName				ステージファイルの名前
 */
bool CStage::Load(char* pName) {
	//テキストファイルを開く
	FILE* fp = fopen(pName, "rt");
	if (fp == NULL)
	{
		return false;
	}
	//ファイルの全容量を調べる
	//一度ファイルの終端に移動して、その位置を調べることでサイズがわかる
	//調べ終わった後はファイルの先頭に移動して戻しておく
	//ただしこの方法でも正常なサイズを読み込むことはできない
	fseek(fp, 0, SEEK_END);
	long fSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	//ファイルサイズ分だけのメモリを確保する
	//終端を含めるため＋１しておく
	char* pBuffer = (char*)malloc(fSize + 1);
	//ファイルを全てバッファに読み込む
	//ここで実際に読み込めたサイズを調べてその終端に\0を入る
	fSize = fread(pBuffer, 1, fSize, fp);
	pBuffer[fSize] = '\0';
	char* pstr;

	//テクスチャの読み込み
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

	//チップサイズ
	pstr = strtok(NULL, ",");
	m_ChipSize = atof(pstr);

	//マップのチップ数
	pstr = strtok(NULL, ",");
	m_XCount = atoi(pstr);
	pstr = strtok(NULL, ",");
	m_YCount = atoi(pstr);

	//マップチップ用のメモリ確保
	m_pChipData = (char*)malloc(m_XCount * m_YCount);

	//チップデータの読み込み
	for (int y = 0; y < m_YCount; y++)
	{
		for (int x = 0; x < m_XCount; x++)
		{
			pstr = strtok(NULL, ",");
			m_pChipData[y * m_XCount + x] = atoi(pstr);
		}
	}

	////敵のテクスチャ読み込み
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

	////配置データの読み込み
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

	//アイテムのテクスチャ読み込み
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

	//配置データの読み込み
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

	//ファイルを閉じる
	fclose(fp);
	free(pBuffer);
	return true;
}

/**
 * 初期化
 * パラメーターや座標を初期化する。
 *
 * 引数
 * [in]			pEnemy				敵の配列
 * [in]			pItem				アイテム配列
 */
void CStage::Initialize(CWaterPlace* pWaterPlace) {
	m_ScrollX = 0;
	m_ScrollY = 0;
	int n = 0;
	//for (int y = 0; y < m_YCount; y++)
	//{
	//	for (int x = 0; x < m_XCount; x++)
	//	{
	//		//配置番号
	//		//番号０は配置しない
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
			//配置番号
			//番号０は配置しない
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
 * 更新
 *
 * 引数
 * [in]			pl					プレイヤー、スクロールの判定に使用
 */
void CStage::Update(CPlayer& pl) {
	//プレイヤーの矩形取得
	CRectangle prec = pl.GetRect();
	//スクリーンの幅
	float sw = CGraphicsUtilities::GetGraphics()->GetTargetWidth();
	//ステージ全体の幅
	float stgw = m_ChipSize * m_XCount;
	//座標が画面端によっている（各端から200pixel）場合スクロールを行って補正する
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
 * 当たり判定
 *
 * 引数
 * [in]			r					判定矩形
 * [out]		ox					X埋まり
 * [out]		oy					Y埋まり
 */
bool CStage::Collision(CRectangle r, float& ox, float& oy) {
	bool re = false;

	//当たり判定する矩形の左上と右下のチップ位置を求める
	int lc = r.Left / m_ChipSize;
	int rc = r.Right / m_ChipSize;
	int tc = r.Top / m_ChipSize;
	int bc = r.Bottom / m_ChipSize;
	//ステージの範囲外にはならないようにする
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

	//当たり判定をする矩形の左上から右下の範囲のみ当たり判定をおこなう
	//それ以外の番号は当たることはないので判定が必要ない
	for (int y = tc; y <= bc; y++)
	{
		for (int x = lc; x <= rc; x++)
		{
			//描画するチップ番号
			//チップ番号０は当たり判定しない
			char cn = m_pChipData[y * m_XCount + x] - 1;
			if (cn < 0)
			{
				continue;
			}
			//マップチップの矩形
			CRectangle cr(x * m_ChipSize, y * m_ChipSize, x * m_ChipSize + m_ChipSize, y * m_ChipSize + m_ChipSize);
			//当たり判定用のキャラクタ矩形
			//下で範囲を限定した専用の矩形を作成する。
			CRectangle brec = r;
			brec.Top = brec.Bottom - 1;	//下の矩形は上側を下と同じ値にする
			brec.Expansion(-6, 0);		//横の範囲を少し狭める
			//下と当たり判定
			if (cr.CollisionRect(brec))
			{
				re = true;
				//チップが斜面の場合自分の立ち位置から高さの割合を求める
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
					//斜面の上の位置を求める
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
					//下の埋まりなのでチップの上端から矩形の下端の値を引いた値が埋まりの値になる
					oy += cr.Top - brec.Bottom;
					r.Top += cr.Top - brec.Bottom;
					r.Bottom += cr.Top - brec.Bottom;
				}
			}
			//チップが斜面の場合壁・天井としての当たり判定をおこなわない
			if (cn != LEFTSLOPE)
			{
				//当たり判定用のキャラクタ矩形
				//左、右それぞれで範囲を限定した専用の矩形を作成する。
				CRectangle lrec = r;
				lrec.Right = lrec.Left + 1;	//左の矩形は右側を左と同じ値にする
				lrec.Expansion(0, -6);		//縦の範囲を少し狭める
				CRectangle rrec = r;
				rrec.Left = rrec.Right - 1;	//右の矩形は左側を右と同じ値にする
				rrec.Expansion(0, -6);		//縦の範囲を少し狭める
				//左と当たり判定
				if (cr.CollisionRect(lrec))
				{
					re = true;
					//左の埋まりなのでチップの右端から矩形の左端の値を引いた値が埋まりの値になる
					ox += cr.Right - lrec.Left;
					r.Left += cr.Right - lrec.Left;
					r.Right += cr.Right - lrec.Left;
				}
				//右と当たり判定
				else if (cr.CollisionRect(rrec))
				{
					re = true;
					//右の埋まりなのでチップの左端から矩形の右端の値を引いた値が埋まりの値になる
					ox += cr.Left - rrec.Right;
					r.Left += cr.Left - rrec.Right;
					r.Right += cr.Left - rrec.Right;
				}
				//当たり判定用のキャラクタ矩形
				//上で範囲を限定した専用の矩形を作成する。
				CRectangle trec = r;
				trec.Bottom = trec.Top + 1;	//上の矩形は下側を上と同じ値にする
				trec.Expansion(-6, 0);		//横の範囲を少し狭める
				//上と当たり判定
				if (cr.CollisionRect(trec))
				{
					re = true;
					//上の埋まりなのでチップの下端から矩形の上端の値を引いた値が埋まりの値になる
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

	//当たり判定する矩形の左上と右下のチップ位置を求める
	int x = px / m_ChipSize;
	int y = py / m_ChipSize;

	//ステージの範囲外にはならないようにする
	if (x < 0 || x >= m_XCount || y < 0 || y >= m_YCount)
	{
		return false;
	}

	//描画するチップ番号
	//チップ番号０は当たり判定しない
	char cn = m_pChipData[y * m_XCount + x] - 1;
	if (cn < 0)
	{
		return false;
	}

	return true;
}


/**
 * 描画
 *
 */
void CStage::Render(void) {
	//遠景の描画
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

	//テクスチャの横幅からマップチップの縦オフセットを求める
	int tcx = m_ChipTexture.GetWidth() / m_ChipSize;
	//マップチップの描画
	for (int y = 0; y < m_YCount; y++)
	{
		for (int x = 0; x < m_XCount; x++)
		{
			//描画するチップ番号
			//チップ番号０は描画しない
			char cn = m_pChipData[y * m_XCount + x] - 1;
			if (cn < 0)
			{
				continue;
			}
			//マップチップの矩形
			CRectangle cr(m_ChipSize * (cn % tcx), m_ChipSize * (cn / tcx), m_ChipSize * (cn % tcx + 1), m_ChipSize * (cn / tcx + 1));
			//マップチップの描画
			m_ChipTexture.Render(-m_ScrollX + x * m_ChipSize, -m_ScrollY + y * m_ChipSize, cr);
		}
	}
}

/**
 * デバッグ描画
 *
 */
void CStage::RenderDebug(void) {
	//位置の描画
	CGraphicsUtilities::RenderString(10, 100, "スクロール X : %.0f , Y : %.0f", m_ScrollX, m_ScrollY);
}

/**
 * 解放
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
