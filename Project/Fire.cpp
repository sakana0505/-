#include "Fire.h"
/**
 * コンストラクタ
 *
 */
CFire::CFire() :
	m_pTexture(NULL),
	m_Motion(),
	m_PosX(0.0f),
	m_PosY(0.0f),
	m_MoveX(0.0f),
	m_MoveY(0.0f),
	m_bShow(false),
	m_bReverse(false),
	m_SrcRect(),
	m_HP(0),
	m_DamageWait(0) {
}

/**
 * デストラクタ
 *
 */
CFire::~CFire() {
	//delete[] で解放されるようにReleaseをデストラクタで実行する
	Release();
}

/**
 * 初期化
 * パラメーターや座標を初期化する。
 *
 * 引数
 * [in]			px					X座標
 * [in]			py					Y座標
 * [in]			type				敵タイプ
 */
void CFire::Initialize(float px, float py, int type) {
	m_Type = type;
	m_PosX = px;
	m_PosY = py;
	m_MoveX = -3.0f;
	m_MoveY = 0.0f;
	m_bReverse = true;
	m_bShow = true;
	m_HP = 3;
	m_DamageWait = 0;

	m_InitX = px;
	m_InitY = py;

	//アニメーションを作成
	SpriteAnimationCreate anim[] = {
		//移動
		{
			"移動",
			0,0,
			60,64,
			TRUE,{{5,0,0},{5,1,0},{5,2,0},{5,3,0}}
		},
		//ダメージ
		{
			"ダメージ",
			0,210,
			60,64,
			FALSE,{{20,0,0}}
		},
	};

}

/**
 * 更新
 *
 */
void CFire::Update(void) {
	//非表示
	if (!m_bShow)
	{
		return;
	}


	//重力により下に少しずつ下がる
	m_MoveY += GRAVITY;
	if (m_MoveY >= 20.0f)
	{
		m_MoveY = 20.0f;
	}
	//実際に座標を移動させる
	m_PosX += m_MoveX;
	m_PosY += m_MoveY;


	//アニメーションの更新
	m_Motion.AddTimer(CUtilities::GetFrameSecond());
	m_SrcRect = m_Motion.GetSrcRect();
	//ダメージのインターバルを減らす
	if (m_DamageWait > 0)
	{
		m_DamageWait--;
	}
}

/**
 * ダメージ処理
 * 引数分のＨＰを減らしＨＰが０以下になれば敵を消去する
 *
 * 引数
 * [in]			dmg				ダメージ
 * [in]			bRev			反転フラグ
 */
void CFire::Damage(int dmg, bool bRev) {
	m_HP -= dmg;
	m_DamageWait = 60;
	if (bRev)
	{
		m_MoveX = -5.0f;
		m_bReverse = false;
	}
	else
	{
		m_MoveX = 5.0f;
		m_bReverse = true;
	}
}

/**
 * ステージとの当たり
 *
 * 引数
 * [in]			ox					X埋まり量
 * [in]			oy					Y埋まり量
 */
void CFire::CollisionStage(float ox, float oy) {
	m_PosX += ox;
	m_PosY += oy;
	//落下中の下埋まり、ジャンプ中の上埋まりの場合は移動を初期化する。
	if (oy < 0 && m_MoveY > 0)
	{
		m_MoveY = 0;
	}
	else if (oy > 0 && m_MoveY < 0)
	{
		m_MoveY = 0;
	}
}

/**
 * 描画
 *
 * 引数
 * [in]			wx					ワールドの変化
 * [in]			wy					ワールドの変化
 */
void CFire::Render(float wx, float wy) {
	//非表示
	if (!m_bShow)
	{
		return;
	}
	//インターバル2フレームごとに描画をしない
	if (m_DamageWait % 4 >= 2)
	{
		return;
	}
	//描画矩形
	CRectangle dr = m_SrcRect;
	//反転フラグがONの場合描画矩形を反転させる
	if (m_bReverse)
	{
		float tmp = dr.Right;
		dr.Right = dr.Left;
		dr.Left = tmp;
	}
	//テクスチャの描画
	m_pTexture->Render(m_PosX - wx, m_PosY - wy, dr);
}

/**
 * デバッグ描画
 *
 * 引数
 * [in]			wx					ワールドの変化
 * [in]			wy					ワールドの変化
 */
void CFire::RenderDebug(float wx, float wy) {
	//非表示
	if (!m_bShow)
	{
		return;
	}
}

/**
 * 解放
 *
 */
void CFire::Release(void) {
	m_Motion.Release();
}
