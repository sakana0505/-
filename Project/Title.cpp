#include "Title.h"
#include "GameDefine.h"

//変更するシーン(外部参照、実体はGameApp.cpp)
extern int						gChangeScene;
//メニュー画面BGM
CSoundBuffer			gSound_Menu;

/**			
*コンストラクタ
*
*/
CTitle::CTitle() :
	m_BackImage(),
    m_Alpha(255),
	m_bEnd(false){
}

/**
 * デストラクタ
 *
 */
CTitle::~CTitle() {
}

/**
 * 読み込み
 * 利用するテクスチャを読み込む。
 */
bool CTitle::Load(void) {
	//テクスチャの読み込み
	if (!m_BackImage.Load("Title.png"))
	{
		return false;
	}
	return true;
	
}
/**
 * 初期化
 * パラメーターや座標を初期化する。
 * 状態を初期化したいときに実行する。
 */
void CTitle::Initialize(void) {
	m_bEnd = false;
	m_Alpha = 0;

	//音素材読み込み
	gSound_Menu.Load("Hometown.mp3");
	//ループ設定
	gSound_Menu.SetLoop(TRUE);
	//BGM再生
	//gSound_Menu.Play();					//開発中はうるさいのでコメントアウト
}

/**
 * 更新
 *
 */
void CTitle::Update(void) {
	//終了状態ならフェードアウト
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
	//Enterキーでゲーム画面へ
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) && !m_bEnd)
	{
		m_bEnd = true;
		gSound_Menu.Stop();
	}


}

/**
 * 描画
 *
 */
void CTitle::Render(void) {
	m_BackImage.Render(0,0,MOF_ARGB(m_Alpha,255,255,255));
	CGraphicsUtilities::RenderString(400, 500, MOF_COLOR_BLACK, "Press Enter Key");
}

/**
 * デバッグ描画
 *
 */
void CTitle::RenderDebug(void) {
}

/**
 * 解放
 *
 */
void CTitle::Release(void) {
	m_BackImage.Release();
	//音の解放
	gSound_Menu.Release();
}
