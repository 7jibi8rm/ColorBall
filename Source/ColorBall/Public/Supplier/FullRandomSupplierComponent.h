// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Math/RandomStream.h"
#include "SupplierComponent.h"
#include "FullRandomSupplierComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLORBALL_API UFullRandomSupplierComponent : public USupplierComponent
{
	GENERATED_BODY()

public:	

    // 次の発射口計算
    virtual void NextFireMuzzle() override
	{
		FireMuzzleIndexes.Empty();
		const int32 FireCount = FMath::RandRange(0, MuzzleCount-1);
		for(int32 i=0; i< FireCount; ++i)
		{
			const int32 FireMuzzle = FMath::RandRange(0, MuzzleCount-1);
			FireMuzzleIndexes.Add(FireMuzzle); // 重複を許容、変であれば後で修正。
		}
	}

};
