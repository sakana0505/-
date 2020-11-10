#include	"WaterPlace.h"

/**
 * コンストラクタ
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
 * デストラクタ
 *
 */
CWaterPlace::~CWaterPlace() {
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
void CWaterPlace::Initialize(float px, float py, int type) {
	m_Type = type;
	m_PosX = px;
	m_PosY = py;
	m_bShow = true;
	//アニメーションを作成
	SpriteAnimationCreate anim = {
		"水場",
		0,0,
		96,96,
		TRUE,{{5,0,0}}
	};
	m_Motion.Create(anim);
}

/**
 * 更新
 *
 */
void CWaterPlace::Update(void) {
	//非表示
	if (!m_bShow)
	{
		return;
	}

	//アニメーションの更新
	m_Motion.AddTimer(CUtilities::GetFrameSecond());
	m_SrcRect = m_Motion.GetSrcRect();
}

/**
 * ステージとの当たり
 *
 * 引数
 * [in]			ox					X埋まり量
 * [in]			oy					Y埋まり量
 */
void CWaterPlace::CollisionStage(float ox, float oy) {
	m_PosX += ox;
	m_PosY += oy;

}

/**
 * 描画
 *
 * 引数
 * [in]			wx					ワールドの変化
 * [in]			wy					ワールドの変化
 */
void CWaterPlace::Render(float wx, float wy) {
	//非表示
	if (!m_bShow)
	{
		return;
	}
	//テクスチャの描画
	m_pWPTexture->Render(m_PosX - wx, m_PosY - wy, m_SrcRect);
}

/**
 * デバッグ描画
 *
 * 引数
 * [in]			wx					ワールドの変化
 * [in]			wy					ワールドの変化
 */
void CWaterPlace::RenderDebug(float wx, float wy) {
	//非表示
	if (!m_bShow)
	{
		return;
	}
	//当たり判定の表示
	CRectangle hr = GetRect();
	CGraphicsUtilities::RenderRect(hr.Left - wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(255, 0, 0));
}

/**
 * 解放
 *
 */
void CWaterPlace::Release(void) {
	m_Motion.Release();
}
