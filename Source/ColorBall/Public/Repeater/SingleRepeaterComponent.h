// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RepeaterComponent.h"
#include "SingleRepeaterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLORBALL_API USingleRepeaterComponent : public URepeaterComponent
{
	GENERATED_BODY()

public:

	USingleRepeaterComponent() {
		RapidRate = 9999.0f; // 連射間隔（秒）
		BurstCount = 1; // バースト発射数
		BurstRate = 0.0f; // バースト間隔（秒）
	}

	virtual ~USingleRepeaterComponent() {
	}

};
