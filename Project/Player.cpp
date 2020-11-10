#include "Player.h"

//MP回復ディレイ
#define		MPRECOV_WAIT		25

/**
 * コンストラクタ
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
 * デストラクタ
 *
 */
CPlayer::~CPlayer() {
}

/**
 * 読み込み
 * 利用するテクスチャを読み込む。
 */
bool CPlayer::Load(void) {

	//テクスチャの読み込み
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
	//アニメーションを作成
	SpriteAnimationCreate anim[] = {
		//待機
		{
			"待機",
			0,0,
			60,64,
			TRUE,{{5,0,0},{5,1,0},{5,2,0},{5,3,0},{5,4,0},{5,5,0},{5,6,0},{5,7,0}}
		},
		//移動
		{
			"移動",
			0,70,
			60,64,
			TRUE,{{5,0,0},{5,1,0},{5,2,0},{5,3,0},{5,4,0},{5,5,0}}
		},
		//ジャンプ
		{
			"ジャンプ開始",
			0,140,
			60,64,
			FALSE,{{5,0,0},{5,1,0},{5,2,0},{5,3,0}}
		},
		{
			"ジャンプ終了",
			240,140,
			60,64,
			FALSE,{{2,0,0},{2,1,0}}
		},
		//攻撃
		{
			"攻撃",
			0, 350,
			90, 64,
			FALSE, { { 2, 0, 0 }, { 2, 1, 0 }, { 2, 2, 0 }, { 2, 3, 0 }, { 2, 4, 0 }, { 2, 5, 0 }, { 2, 6, 0 } }
		},
		//攻撃2
		{
			"攻撃2",
			0, 420,
			90, 64,
			FALSE, { { 2, 0, 0 }, { 2, 1, 0 }, { 2, 2, 0 }, { 2, 3, 0 }, { 2, 4, 0 }, { 2, 5, 0 }, { 2, 6, 0 } }
		},
		//ダメージ
		{
			"ダメージ",
			480,0,
			60,64,
			FALSE,{{20,0,0}}
		},

	};
	m_Motion.Create(anim, MOTION_COUNT);
	return true;
}

/**
 * 初期化
 * パラメーターや座標を初期化する。
 * プレイヤーの位置など状態を初期化したいときに実行する。
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
 * 更新
 *
 */
void CPlayer::Update(void) {
	//HPがなくなると終了
	if (m_HP <= 0)
	{
		m_bEnd = true;
		return;
	}



	//移動フラグ、このフレームでの移動があったかを保存
	m_bMove = false;

	//攻撃中、着地の場合の動作
	if (m_Motion.GetMotionNo() == MOTION_ATTACK2 || m_Motion.GetMotionNo() == MOTION_JUMPEND)
	{
		//終了で待機に戻す
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
		//終了で待機に戻す
		if (m_Motion.IsEndMotion() && !m_bAttack)
		{
			m_Motion.ChangeMotion(MOTION_WAIT);
		}
	}


	//ダメージ中の動作
	else if (m_Motion.GetMotionNo() == MOTION_DAMAGE)
	{
		//終了で待機に戻す
		if (m_Motion.IsEndMotion())
		{
			m_Motion.ChangeMotion(MOTION_WAIT);
		}
	}
	else
	{
		UpdateKey();
	}

	//移動更新
	UpdateMove();
	//実際に座標を移動させる
	m_PosX += m_MoveX;
	m_PosY += m_MoveY;

	//画面外で落下としてHPを０にする
	if (m_PosY >= g_pGraphics->GetTargetHeight() && m_HP > 0)
	{
		m_HP = 0;
	}
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
 * キー入力による動作更新
 *
 */
void CPlayer::UpdateKey(void) {
	//キーボードでの移動
	//入力で直接座標を動かすのではなく、速度を変化させる。
	//攻撃中は移動できないようにする

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
	//上キーでジャンプ
	if (g_pInput->IsKeyHold(MOFKEY_UP) && !m_bJump)
	{
		m_bJump = true;
		m_MoveY = PLAYER_JUMP;
		m_Motion.ChangeMotion(MOTION_JUMPSTART);
	}
	//SPACEキーで攻撃
	if (g_pInput->IsKeyPush(MOFKEY_SPACE))
	{
		//MPの個数管理
		//MPが０になると攻撃できなくなる
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
	//攻撃２
	if (m_Motion.GetMotionNo() == MOTION_ATTACK && m_bAttack)
	{
		if (m_Motion.IsEndMotion())
		{
			m_Motion.ChangeMotion(MOTION_ATTACK2);
		}
		return;
	}

	//MP回復
	//水場の判定内に居る時
	//Sキーで５回復
	if (m_WaterPlaceflg)
	{
		//ディレイ用処理
		if(m_MPRecovWait <= 0)
		{ 
			if (g_pInput->IsKeyHold(MOFKEY_S))
			{
				//レベル３の場合
				if (m_Level == 3)
				{
					//MP１０～２０
					if (m_MP >= 10 && m_MPPlus < 10)
					{
						m_MPPlus ++;
					}
					//MP５～１０
					else if (m_MP < 10)
					{
						m_MP ++;
					}
				}
				//レベル２の場合
				else if (m_Level == 2)
				{
					//MP１０～１５
					if (m_MP >= 10 && m_MPPlus < 5)
					{
						m_MPPlus++;
					}
					//MP５～１０
					else if (m_MP < 10)
					{
						m_MP++;
					}
				}
				//レベル１の場合
				else
				{
					//MP５～１０
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


	//デバッグ用プレイヤーレベル操作
	//Lキーでプレイヤーレベルを上昇させる（最大３）
	if (g_pInput->IsKeyPush(MOFKEY_L))
	{
		if (m_Level < 3)
		{
			//レベルを上げる
			m_Level += 1;

			//レベルの上昇に合わせてステータスを変化させる
			//MP量変化
			//MP１０～１５
			if (m_MP >= 10 && m_MPPlus <= 5)
			{
				m_MPPlus += 5;
			}
			//MP５～１０
			else if (m_MP < 10 && m_MP > 5)
			{
				int i;
				i = 10 - m_MP;
				m_MP = 10;
				m_MPPlus = 5 - i;
			}
			//MP５以下
			else if (m_MP <= 5)
			{
				m_MP += 5;
			}

		}
	}
}

/**
 * 移動更新
 *
 */
void CPlayer::UpdateMove(void) {
	//このフレームでの移動入力がなければ減速処理を実行する
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
	//重力により下に少しずつ下がる
	m_MoveY += GRAVITY;
	if (m_MoveY >= 20.0f)
	{
		m_MoveY = 20.0f;
	}
}

/**
 * ステージとの当たり
 *
 * 引数
 * [in]			ox					X埋まり量
 * [in]			oy					Y埋まり量
 */
void CPlayer::CollisionStage(float ox, float oy) {
	m_PosX += ox;
	m_PosY += oy;
	//落下中の下埋まり、ジャンプ中の上埋まりの場合は移動を初期化する。
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
	//左移動中の左埋まり、右移動中の右埋まりの場合は移動を初期化する。
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
 * 当たり判定
 * 引数の敵に対して矩形同士の当たり判定を実行する。
 * 当たっていた場合はプレイヤーの状態を変更する
 *
 * 引数
 * [in]			ene				判定を行う敵
 *
 * 戻り値
 * 当たっている場合はtrue、当たっていなければfalseを返す
 */
 //bool CPlayer::CollisionEnemy(CEnemy& ene) {
 //    if (!ene.GetShow())
 //    {
 //        return false;
 //    }
 //    HPがなくなると当たり判定しない
 //    if (m_HP <= 0)
 //    {
 //        return false;
 //    }
 //	敵のView矩形との判定
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
 //    ダメージ中のため当たり判定をおこなわない
 //    if (m_DamageWait > 0 || ene.GetDamageWait() > 0)
 //    {
 //        return false;
 //    }
 //    敵の矩形と自分の矩形でダメージ
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
 //            //爆発エフェクトを発生させる
 //            m_pEndEffect = m_pEffectManager->Start(m_PosX + m_SrcRect.GetWidth() * 0.5f, m_PosY + m_SrcRect.GetHeight() * 0.5f, EFC_EXPLOSION02);
 //        }
 //        else
 //        {
 //            //ダメージエフェクトを発生させる
 //            m_pEffectManager->Start(m_PosX + m_SrcRect.GetWidth() * 0.5f, m_PosY + m_SrcRect.GetHeight() * 0.5f, EFC_DAMAGE);
 //
 //            CPopupMsg::GetInstance()->Add("Damage!\n", GetRect().Left, GetRect().Top, 40, MOF_XRGB(255, 255, 255), MOF_XRGB(255, 0, 0));
 //        }
 //        return true;
 //    }
 //
 //
 //
 //    //敵の索敵範囲に居るかどうか
 //    CRectangle esrec = ene.GetSearchRect();
 //    if (prec.CollisionRect(esrec))
 //    {
 //        ene.SetBoost();
 //    }
 //
 //
 //
 //
 //    攻撃中の場合のみ攻撃との当たり判定を実行
 //    if (m_Motion.GetMotionNo() != MOTION_ATTACK && m_Motion.GetMotionNo() != MOTION_ATTACK2)
 //    {
 //        return false;
 //    }
 //    敵の矩形と自分の攻撃矩形で敵がダメージ
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
  * 当たり判定
  * 引数のアイテムに対して矩形同士の当たり判定を実行する。
  * 当たっていた場合はプレイヤーの状態を変更する
  *
  * 引数
  * [in]			itm				判定を行うアイテム
  *
  * 戻り値
  * 当たっている場合はtrue、当たっていなければfalseを返す
  */
  /*bool CPlayer::CollisionItem(CItem& itm) {
	  if (!itm.GetShow())
	  {
		  return false;
	  }
	  //アイテムの矩形と自分の矩形で当たり判定
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
	* 当たり判定
	* 引数の水場に対して矩形同士の当たり判定を実行する。
	* 当たっていた場合は設定してあるフラグをTRUEにする。
	*
	* 引数
	* [in]			wrp				判定を行う水場
	*
	* 戻り値
	* 当たっている場合はtrueを返す
	*/
	void CPlayer::CollisionWaterPlace(CWaterPlace& wrp) {

		//水場の矩形と自分の矩形で当たり判定
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
   * 描画
   *
   * 引数
   * [in]			wx					ワールドの変化
   * [in]			wy					ワールドの変化
   */
void CPlayer::Render(float wx, float wy) {
	//インターバル2フレームごとに描画をしない
	if (m_DamageWait % 4 >= 2)
	{
		return;
	}
	//HPがなくなると描画しない
	if (m_HP <= 0)
	{
		return;
	}
	//描画矩形
	CRectangle dr = m_SrcRect;
	//描画位置
	float px = m_PosX - wx;
	float py = m_PosY - wy;
	//反転フラグがONの場合描画矩形を反転させる
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
	//テクスチャの描画
	m_Texture.Render(px, py, dr);
}

/**
 * ステータス描画
 *
 */
void CPlayer::RenderStatus(void) {
	//HP表示
	for (int i = 0; i < m_HP; i++)
	{
		m_HPTexture.RenderScale(150 + 40 * i, 10, 0.2f);
	}

	//MP表示
	//空枠の表示
	for (int i = 0; i < 10; i++)
	{
		m_MPEmpTexture.RenderScale(145 + 30 * i, 40, 0.06f);
	}
	//追加量の空枠の表示
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
	
	//MP初期量（１０）の表示
	for (int i=0; i < m_MP; i++)
	{
		m_MPTexture.RenderScale(145 + 30 * i, 40, 0.06f);
	}
	//追加量（５～１０）の表示
	if (m_MPPlus > 0)
	{
		for (int i = 0; i < m_MPPlus; i++)
		{
			m_MPTexture.RenderScale(145 + 30 * i, 67, 0.06f);
		}
		
	}

	//フレームを上部に描画
	m_FrameTexture.RenderScale(0, 0,0.25f);
}

/**
 * デバッグ描画
 *
 * 引数
 * [in]			wx					ワールドの変化
 * [in]			wy					ワールドの変化
 */
void CPlayer::RenderDebug(float wx, float wy) {
	//位置の描画
	CGraphicsUtilities::RenderString(10, 70, "プレイヤー位置 X : %.0f , Y : %.0f", m_PosX, m_PosY);
	//プレイヤーステータスの表示
	CGraphicsUtilities::RenderString(10, 150, "プレイヤーレベル：%d",m_Level);
	CGraphicsUtilities::RenderString(10, 170, "プレイヤーHP：%d", m_HP);
	CGraphicsUtilities::RenderString(10, 190, "プレイヤーMP：%d", m_MP + m_MPPlus);

	//当たり判定の表示
	CRectangle hr = GetRect();
	CGraphicsUtilities::RenderRect(hr.Left - wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(0, 255, 0));
	//攻撃の当たり判定表示
	if (IsAttack())
	{
		CRectangle hr = GetAttackRect();
		CGraphicsUtilities::RenderRect(hr.Left - wx, hr.Top - wy, hr.Right - wx, hr.Bottom - wy, MOF_XRGB(255, 0, 0));
	}
}

/**
 * 解放
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
