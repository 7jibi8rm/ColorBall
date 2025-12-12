// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Muzzle/MuzzleComponent.h"
#include "AroundMuzzleComponent.generated.h"

/**
 * 
 */
UCLASS()
class COLORBALL_API UAroundMuzzleComponent : public UMuzzleComponent
{
	GENERATED_BODY()
	
public:

	UAroundMuzzleComponent(){
		MuzzleTransforms.SetNum(MuzzleCount);
	}

	// 砲台座標計算
	virtual void CalcMuzzleTransforms(float ElapsedTime) override
	{
		const float AngleStep = 360.0f / MuzzleTransforms.Num();
		const float Radius = 70.0f; // 円の大きさ（必要に応じて調整）

		for (int32 i = 0; i < MuzzleTransforms.Num(); ++i)
		{
			// 各砲口の角度（度→ラジアン）
			const float AngleDeg = i * AngleStep;
			const float AngleRad = FMath::DegreesToRadians(AngleDeg);

			// 円周上の座標（X-Y平面上）
			const FVector LocalOffset = FVector(FMath::Cos(AngleRad), FMath::Sin(AngleRad), 0.0f) * Radius;

			// アクターの回転を考慮してワールド座標へ変換
			const FVector WorldOffset = GetOwner()->GetActorRotation().RotateVector(LocalOffset);
			const FVector MuzzleLocation = GetOwner()->GetActorLocation() + WorldOffset;

			// 外向き（円の中心から外側）を向く回転を計算
			const FVector ForwardVector = WorldOffset.GetSafeNormal();
			const FRotator MuzzleRotation = ForwardVector.Rotation();

			MuzzleTransforms[i] = FTransform(MuzzleRotation, MuzzleLocation);
		}
		return;
	}

private:

	const int32 MuzzleCount = 16;


};
