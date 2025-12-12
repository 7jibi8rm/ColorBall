// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SupplierComponent.h"
#include "FullSupplierComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLORBALL_API UFullSupplierComponent : public USupplierComponent
{
	GENERATED_BODY()

public:	

    // ŽŸ‚Ì”­ŽËŒûŒvŽZ
    virtual void NextFireMuzzle() override
	{
        FireMuzzleIndexes.Empty();
        for (int32 Index = 0; Index < MuzzleCount; ++Index) {
            FireMuzzleIndexes.Add(Index);
        }
    }
};
