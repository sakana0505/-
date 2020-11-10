#include "GameDefine.h"
#include "Game.h"

//変更するシーン(外部参照、実体はGameApp.cpp)
extern int						gChangeScene;
//ステージBGM
CSoundBuffer			gSound_Stage;
/**
 * コンストラクタ
 *
 */
CGame::CGame() :
	m_bEnd(false) {
}

/**
 * デストラクタ
 *
 */
CGame::~CGame() {
}

/**
 * 読み込み
 * 利用するテクスチャを読み込む。
 */
bool CGame::Load(void) {
	//プレイヤー素材読み込み
	m_Player.Load();
	//ステージの素材読み込み
	m_Stage.Load("Stage1.txt");

	//アイテムメモリ確保
	m_WaterPlaceArray = new CWaterPlace[m_Stage.GetWaterPlaceCount()];
	return true;
}
/**
 * 初期化
 * パラメーターや座標を初期化する。
 * 状態を初期化したいときに実行する。
 */
void CGame::Initialize(void) {
	//リソース配置ディレクトリの設定
	CUtilities::SetCurrentDirectory("Resource");
	//プレイヤーの状態初期化
	m_Player.Initialize();
	//ステージの状態初期化
	m_Stage.Initialize(m_WaterPlaceArray);

	//音素材読み込み
	gSound_Stage.Load("Harmless_3.mp3");
	//ループ設定
	gSound_Stage.SetLoop(TRUE);
	//BGM再生
	//gSound_Stage.Play();					//開発中はうるさいのでコメントアウト
}

/**
 * 更新
 *
 */
void CGame::Update(void) {
	//プレイヤーの更新
	m_Player.Update();
	//ステージとプレイヤーの当たり判定
	float ox = 0, oy = 0;
	if (m_Stage.Collision(m_Player.GetRect(), ox, oy))
	{
		m_Player.CollisionStage(ox, oy);
	}

	//アイテムの更新
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
	//当たり判定の実行
	for (int i = 0; i < m_Stage.GetWaterPlaceCount(); i++)
	{
		m_Player.CollisionWaterPlace(m_WaterPlaceArray[i]);
	}

	//ステージの更新
	m_Stage.Update(m_Player);

	//Enterキーでゲームオーバー画面へ
	if (g_pInput->IsKeyPush(MOFKEY_RETURN) && !m_bEnd)
	{
		gChangeScene = SCENENO_GAMEOVER;
		gSound_Stage.Stop();
	}

}

/**
 * 描画
 *
 */
void CGame::Render(void) {
	//ステージの描画
	m_Stage.Render();
	CGraphicsUtilities::RenderString(400, 500, MOF_COLOR_WHITE, "ゲーム画面やで");
	//アイテムの描画
	for (int i = 0; i < m_Stage.GetWaterPlaceCount(); i++)
	{
		m_WaterPlaceArray[i].Render(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	}

	//プレイヤーの描画
	m_Player.Render(m_Stage.GetScrollX(), m_Stage.GetScrollY());

	

	//プレイヤーの状態描画
	m_Player.RenderStatus();
}

/**
 * デバッグ描画
 *
 */
void CGame::RenderDebug(void) {
	//ステージのデバッグ描画
	m_Stage.RenderDebug();
	//プレイヤーのデバッグ描画
	m_Player.RenderDebug(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	//アイテムのデバッグ描画
	for (int i = 0; i < m_Stage.GetWaterPlaceCount(); i++)
	{
		m_WaterPlaceArray[i].RenderDebug(m_Stage.GetScrollX(), m_Stage.GetScrollY());
	}
}



/**
 * 解放
 *
 */
void CGame::Release(void) {
	//ステージの解放
	m_Stage.Release();
	//プレイヤーの解放
	m_Player.Release();
	//音の解放
	gSound_Stage.Release();
	//アイテムの解放
	if (m_WaterPlaceArray)
	{
		delete[] m_WaterPlaceArray;
		m_WaterPlaceArray = NULL;
	}
}
