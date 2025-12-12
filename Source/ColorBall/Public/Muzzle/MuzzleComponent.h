// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MuzzleComponent.generated.h"

/**
 * <summary>
 * 砲台（マズル）管理用の基底コンポーネント。
 * 派生クラスで砲台配置や座標計算を実装します。
 * </summary>
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class COLORBALL_API UMuzzleComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	/// <summary>
	/// コンストラクタ。
	/// コンポーネントの初期化処理を行います。
	/// </summary>
	UMuzzleComponent();

	/// <summary>
	/// 砲台変換行列（座標・回転）配列を取得します。
	/// </summary>
	/// <returns>砲台のFTransform配列</returns>
	const TArray<FTransform>& GetMuzzleTransforms() const;

	/// <summary>
	/// 砲台情報を破棄します。
	/// 配列をクリアし、砲台をリセットします。
	/// </summary>
	void ClearMuzzles();

	/// <summary>
	/// 砲台座標計算処理。
	/// 経過時間などを元に砲台の配置・座標を計算します。
	/// 派生クラスでオーバーライド可能です。
	/// </summary>
	/// <param name="ElapsedTime">経過時間</param>
	virtual void CalcMuzzleTransforms(float ElapsedTime);

	/// <summary>
	/// 毎フレーム呼び出されるTick処理。
	/// 必要に応じて砲台の状態更新を行います。
	/// </summary>
	/// <param name="DeltaTime">前フレームからの経過時間</param>
	/// <param name="TickType">Tickの種類</param>
	/// <param name="ThisTickFunction">Tick関数情報</param>
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// 砲台変換行列配列（各砲台の座標・回転情報）
	TArray<FTransform> MuzzleTransforms;
};