// Fill out your copyright notice in the Description page of Project Settings.

#include "Repeater/RepeaterComponent.h"

/// <summary>
/// コンストラクタ。
/// コンポーネントのデフォルト値を設定し、毎フレームTickを有効化します。
/// </summary>
URepeaterComponent::URepeaterComponent()
{
	// ゲーム開始時に初期化し、毎フレームTickを有効化
	PrimaryComponentTick.bCanEverTick = true;
}

/// <summary>
/// リピーター開始処理。
/// 連射・バースト射撃を開始し、指定されたコールバック関数を射撃時に呼び出します。
/// </summary>
/// <param name="InFireCallback">射撃時に呼び出すコールバック関数</param>
void URepeaterComponent::StartRepeater(TFunction<void()> InFireCallback, int32 InMuzzle)
{
	FireCallback = InFireCallback; // 射撃時コールバック関数
    MuzzleCount = InMuzzle; // 砲台数
	verify(FireCallback);

	// バースト弾を考慮した連射間隔計算
	const float FixRapidRate = RapidRate + (BurstRate * static_cast<float>(BurstCount));

	// 子クラスの開始時処理呼び出し
	OnStartRepeater();

	// 連射弾初撃発射、その後タイマー起動
	HandleRapidFire();
	GetWorld()->GetTimerManager().SetTimer(RapidFireTimer, this, &URepeaterComponent::HandleRapidFire, FixRapidRate, true);
}

/// <summary>
/// リピーター停止処理。
/// 連射・バースト射撃を停止します。
/// </summary>
void URepeaterComponent::StopRepeater()
{
	// 連射用タイマー停止
	GetWorld()->GetTimerManager().ClearTimer(RapidFireTimer); // 連射用タイマー
}

/// <summary>
/// 連射発射処理。
/// RapidRateに従い、射撃コールバックを定期的に呼び出します。
/// バースト発射処理も開始します。
/// </summary>
void URepeaterComponent::HandleRapidFire()
{
	// バースト弾初撃発射、その後タイマー起動
	CurrentBurstCount = 0; // 現在のバースト発射数
	HandleBurstFire();
}

/// <summary>
/// バースト発射処理。
/// BurstCount/BurstRateに従い、複数回の射撃コールバックを制御します。
/// </summary>
void URepeaterComponent::HandleBurstFire()
{
	// 発射コールバック呼び出し
	if (FireCallback != nullptr) {
		FireCallback();
	}

	// 現在のバースト発射数インクリメント
	++CurrentBurstCount; // 現在のバースト発射数

	// バースト弾全弾発射完了後、タイマー停止
	if (CurrentBurstCount < BurstCount) {
		GetWorld()->GetTimerManager().SetTimer(BurstFireTimer, this, &URepeaterComponent::HandleBurstFire, BurstRate, true); // バースト用タイマー
	}
	// 全弾発射済みならタイマー停止
	else {
		GetWorld()->GetTimerManager().ClearTimer(BurstFireTimer); // バースト用タイマー
	}
}

/// <summary>
/// リピーター開始時の処理。
/// 派生クラスでオーバーライドし、開始時のカスタム処理を実装可能です。
/// </summary>
void URepeaterComponent::OnStartRepeater()
{
}
