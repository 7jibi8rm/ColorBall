// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SupplierComponent.h"
#include "OrderSupplierComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLORBALL_API UOrderSupplierComponent : public USupplierComponent
{
	GENERATED_BODY()

public:	

    // ŽŸ‚Ì”­ŽËŒûŒvŽZ
    virtual void NextFireMuzzle() override
	{
		FireMuzzleIndexes.Empty();
		FireMuzzleIndexes.Add(CurrentFireMuzzleIndex);

		++CurrentFireMuzzleIndex;
		CurrentFireMuzzleIndex %= MuzzleCount;
	}
};
