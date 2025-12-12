// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Muzzle/MuzzleComponent.h"
#include "SpreaderMuzzleComponent.generated.h"

/**
 * 
 */
UCLASS()
class COLORBALL_API USpreaderMuzzleComponent : public UMuzzleComponent
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	USpreaderMuzzleComponent(){
        // スプレッダーの砲台数分、配列を確保
        MuzzleTransforms.SetNum(SpreaderCount);
	}

    /// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~USpreaderMuzzleComponent() {
    }

    /// <summary>
	/// 砲台座標計算
	/// スプレッダー砲台の砲台位置を計算します。
	/// </summary>
	virtual void CalcMuzzleTransforms(float ElapsedTime) override
	{
		const int32 SpreadAngle = 10;  // スプレッド角度

		// 1つだけ砲台があるので、オーナーの前方に配置
		for (int32 i = 0; i < MuzzleTransforms.Num(); ++i)
		{
			const int32 SpreadYaw = FMath::RandRange(-SpreadAngle, SpreadAngle);
			const int32 SpreadPitch = FMath::RandRange(-SpreadAngle, SpreadAngle);
			const FRotator SpreadRotator = FRotator(SpreadPitch, SpreadYaw, 0.f);
            MuzzleTransforms[i] = FTransform(GetOwner()->GetActorRotation() + SpreadRotator, GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.f);
		}
		return;
	}

private:

	const int32 SpreaderCount = 12; // スプレッダーの砲台数


};
