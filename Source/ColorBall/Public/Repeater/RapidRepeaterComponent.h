// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RepeaterComponent.h"
#include "RapidRepeaterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLORBALL_API URapidRepeaterComponent : public URepeaterComponent
{
	GENERATED_BODY()

public:

	URapidRepeaterComponent() {
		RapidRate = 0.1f; // 連射間隔（秒）
		BurstCount = 1; // バースト発射数
		BurstRate = 0.0f; // バースト間隔（秒）
	}

	virtual ~URapidRepeaterComponent() {
	}

};
