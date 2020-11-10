#include "GameOver.h"
#include	"GameDefine.h"
#include	"GameApp.h"

//変更するシーン(外部参照、実体はGameApp.cpp)
extern int						gChangeScene;
//ゲームオーバーBGM
CSoundBuffer			gSound_GameOver;

/**
 * コンストラクタ
 *
 */
CGameOver::CGameOver() :
	m_bEnd(false) {
}

/**
 * デストラクタ
 *
 */
CGameOver::~CGameOver() {
}

/**
 * 読み込み
 * 利用するテクスチャを読み込む。
 */
bool CGameOver::Load(void) {
	return true;
}
/**
 * 初期化
 * パラメーターや座標を初期化する。
 * 状態を初期化したいときに実行する。
 */
void CGameOver::Initialize(void) {
	CUtilities::SetCurrentDirectory("Resource");
	//音素材読み込み
	gSound_GameOver.Load("in_dream.mp3");
	//ループ設定
	gSound_GameOver.SetLoop(TRUE);
	//BGM再生
	//gSound_GameOver.Play();						//開発中はうるさいのでコメントアウト
}

/**
 * 更新
 *
 */
void CGameOver::Update(void) {
	//Enterキーでタイトル画面へ
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) && !m_bEnd)
	{
		gChangeScene = SCENENO_TITLE;
		gSound_GameOver.Stop();
	}

}

/**
 * 描画
 *
 */
void CGameOver::Render(void) {
	CGraphicsUtilities::RenderString(400, 500, MOF_COLOR_WHITE, "ゲームオーバー\n次はがんばれ");
}

/**
 * デバッグ描画
 *
 */
void CGameOver::RenderDebug(void) {
}


/**
 * 解放
 *
 */
void CGameOver::Release(void) {
	//音の解放
	gSound_GameOver.Release();
	
}
