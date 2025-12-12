// Fill out your copyright notice in the Description page of Project Settings.

#include "Muzzle/MuzzleComponent.h"

/// <summary>
/// コンストラクタ。
/// コンポーネントの初期化処理を行います。
/// 毎フレームTickを有効化します。
/// </summary>
UMuzzleComponent::UMuzzleComponent()
{
	// ゲーム開始時に初期化し、毎フレームTickを有効化
	PrimaryComponentTick.bCanEverTick = true;
}

/// <summary>
/// 砲台変換行列（座標・回転）配列を取得します。
/// </summary>
/// <returns>砲台のFTransform配列</returns>
const TArray<FTransform>& UMuzzleComponent::GetMuzzleTransforms() const
{
	return MuzzleTransforms; // 砲台変換行列配列
}

/// <summary>
/// 砲台情報を破棄します。
/// 配列をクリアし、砲台をリセットします。
/// </summary>
void UMuzzleComponent::ClearMuzzles()
{
	MuzzleTransforms.Empty(); // 砲台変換行列配列をクリア
}

/// <summary>
/// 砲台座標計算処理。
/// 経過時間などを元に砲台の配置・座標を計算します。
/// 派生クラスでオーバーライド可能です。
/// </summary>
/// <param name="ElapsedTime">経過時間</param>
void UMuzzleComponent::CalcMuzzleTransforms(float ElapsedTime)
{
	MuzzleTransforms.Empty(); // 砲台変換行列配列をクリア

	// デフォルト実装：オーナーの前方100cmに1つ砲台を配置
	MuzzleTransforms.Add(FTransform(GetOwner()->GetActorRotation(), GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.f));
}

/// <summary>
/// 毎フレーム呼び出されるTick処理。
/// 必要に応じて砲台の状態更新を行います。
/// </summary>
/// <param name="DeltaTime">前フレームからの経過時間</param>
/// <param name="TickType">Tickの種類</param>
/// <param name="ThisTickFunction">Tick関数情報</param>
void UMuzzleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
