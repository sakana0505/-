/*************************************************************************//*!
					
					@file	GameApp.cpp
					@brief	基本ゲームアプリ。

															@author	濱田　享
															@date	2014.05.14
*//**************************************************************************/

//INCLUDE
#include	"GameApp.h"
#include	"GameDefine.h"
#include	"Title.h"
#include	"Game.h"
#include	"GameOver.h"


//現在のシーン
int						gScene = SCENENO_TITLE;
//変更するシーン
int						gChangeScene = SCENENO_TITLE;

//各シーンクラス
CTitle					gTitleScene;
CGame					gGameScene;
CGameOver				gGameOverScene;

//デバッグ表示フラグ
bool					g_bDebug = false;

/*************************************************************************//*!
		@brief			アプリケーションの初期化
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Initialize(void){
	//リソース配置ディレクトリの設定
	CUtilities::SetCurrentDirectory("Resource");
	//シーンの必要リソースを全て読み込む
	gTitleScene.Load();
	gGameScene.Load();
	gGameOverScene.Load();

	//最初に実行されるシーンの初期化
	gTitleScene.Initialize();
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの更新
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Update(void){
	//キーの更新
	g_pInput->RefreshKey();
	//シーン番号によって更新
	switch (gScene)
	{
	case SCENENO_TITLE:
		gTitleScene.Update();
		break;
	case SCENENO_GAME:
		gGameScene.Update();
		break;
	case SCENENO_GAMEOVER:
		gGameOverScene.Update();
		break;
	}
	//シーン変更があった場合変更先シーンの初期化
	if (gChangeScene != gScene)
	{
		switch (gChangeScene)
		{
		case SCENENO_TITLE:
			gTitleScene.Initialize();
			break;
		case SCENENO_GAME:
			gGameScene.Initialize();
			break;
		case SCENENO_GAMEOVER:
			gGameOverScene.Initialize();
			break;
		}
		gScene = gChangeScene;
	}
	//デバッグ表示の切り替え
	if (g_pInput->IsKeyPush(MOFKEY_2))
	{
		g_bDebug = ((g_bDebug) ? false : true);
	}
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの描画
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Render(void){
	//描画開始
	g_pGraphics->RenderStart();
	//画面のクリア
	g_pGraphics->ClearTarget(0.0f,0.0f,1.0f,0.0f,1.0f,0);

	//シーン番号によって描画
	switch (gScene)
	{
	case SCENENO_TITLE:
		gTitleScene.Render();
		break;
	case SCENENO_GAME:
		gGameScene.Render();
		break;
	case SCENENO_GAMEOVER:
		gGameOverScene.Render();
		break;
	}
	//デバッグ表示をする場合
	if (g_bDebug)
	{
		//シーン番号によって描画
		switch (gScene)
		{
		case SCENENO_TITLE:
			gTitleScene.RenderDebug();
			break;
		case SCENENO_GAME:
			gGameScene.RenderDebug();
			break;
		case SCENENO_GAMEOVER:
			gGameOverScene.RenderDebug();
			break;
		}
	}
	//描画の終了
	g_pGraphics->RenderEnd();
	return TRUE;
}
/*************************************************************************//*!
		@brief			アプリケーションの解放
		@param			None
				
		@return			TRUE		成功<br>
						それ以外	失敗、エラーコードが戻り値となる
*//**************************************************************************/
MofBool CGameApp::Release(void){
	gTitleScene.Release();
	gGameScene.Release();
	gGameOverScene.Release();
	return TRUE;
}