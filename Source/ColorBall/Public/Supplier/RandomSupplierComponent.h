// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/RandomStream.h"
#include "SupplierComponent.h"
#include "RandomSupplierComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLORBALL_API URandomSupplierComponent : public USupplierComponent
{
	GENERATED_BODY()

public:	

    // ŽŸ‚Ì”­ŽËŒûŒvŽZ
    virtual void NextFireMuzzle() override
	{
		const int32 FireMuzzle = FMath::RandRange(0, MuzzleCount-1);
		FireMuzzleIndexes.Empty();
		FireMuzzleIndexes.Add(FireMuzzle);

	}

};
