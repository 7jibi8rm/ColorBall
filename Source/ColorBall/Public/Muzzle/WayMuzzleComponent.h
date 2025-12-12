// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Muzzle/MuzzleComponent.h"
#include "WayMuzzleComponent.generated.h"

/**
 * 
 */
UCLASS()
class COLORBALL_API UWayMuzzleComponent : public UMuzzleComponent
{
	GENERATED_BODY()
	
public:

	UWayMuzzleComponent(){
	}

	// –C‘äÀ•WŒvŽZ
	virtual void CalcMuzzleTransforms(float ElapsedTime) override
	{
        const int32 WayCount = 7;   // 7•ûŒü‚É–C‘ä‚ð”z’u
        const int32 WayAngle = 120;  // L‚°‚½‚¢Šp“x
        const FRotator BaseRotator = GetOwner()->GetActorRotation();

		MuzzleTransforms.Empty();
		for(int i=0; i<WayCount; ++i)
		{
            const float AngleOffset = -WayAngle * 0.5f + (WayAngle / (WayCount - 1)) * i;
            const FRotator MuzzleRotator = BaseRotator + FRotator(0.f, AngleOffset, 0.f);
			MuzzleTransforms.Add(FTransform(MuzzleRotator, GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100.f));
		}
		return;
	}
};
