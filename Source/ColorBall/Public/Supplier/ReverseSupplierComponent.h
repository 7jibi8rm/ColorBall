// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SupplierComponent.h"
#include "ReverseSupplierComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLORBALL_API UReverseSupplierComponent : public USupplierComponent
{
	GENERATED_BODY()

public:	

    // ŽŸ‚Ì”­ŽËŒûŒvŽZ
    virtual void NextFireMuzzle() override
	{
		FireMuzzleIndexes.Empty();
		FireMuzzleIndexes.Add(CurrentFireMuzzleIndex);

		--CurrentFireMuzzleIndex;
		if(CurrentFireMuzzleIndex<0){
            CurrentFireMuzzleIndex = MuzzleCount-1;
	    }
		CurrentFireMuzzleIndex %= MuzzleCount;
	}
};
