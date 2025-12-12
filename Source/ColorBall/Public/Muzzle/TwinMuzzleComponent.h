// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Muzzle/MuzzleComponent.h"
#include "TwinMuzzleComponent.generated.h"

/**
 * 
 */
UCLASS()
class COLORBALL_API UTwinMuzzleComponent : public UMuzzleComponent
{
	GENERATED_BODY()
	
public:

    /// <summary>
    /// コンストラクタ
	/// </summary>
	UTwinMuzzleComponent(){
	}

	/// <summary>
    /// デストラクタ
	/// </summary>
	virtual ~UTwinMuzzleComponent() {
	}

	/// <summary>
	/// 砲台座標計算
	/// ツイン砲台の砲台位置は固定なので、ここでは何もしない。
    /// </summary>	
	virtual void CalcMuzzleTransforms(float ElapsedTime) override{
		const FVector RightVec = GetOwner()->GetActorRightVector(); // プレイヤーの右方向ベクトル取得。
		const FVector LeftPosition = GetOwner()->GetActorLocation() - (RightVec * 75);
		const FVector RightPosition = GetOwner()->GetActorLocation() + (RightVec * 75);

		MuzzleTransforms.Empty();

		MuzzleTransforms.Add(FTransform(GetOwner()->GetActorRotation(), LeftPosition + GetOwner()->GetActorForwardVector() * 100.f));
		MuzzleTransforms.Add(FTransform(GetOwner()->GetActorRotation(), RightPosition + GetOwner()->GetActorForwardVector() * 100.f));
		return;
	}
};
