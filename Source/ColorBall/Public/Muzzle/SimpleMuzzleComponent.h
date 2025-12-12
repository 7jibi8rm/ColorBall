// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Muzzle/MuzzleComponent.h"
#include "SimpleMuzzleComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLORBALL_API USimpleMuzzleComponent : public UMuzzleComponent
{
	GENERATED_BODY()

public:	
	USimpleMuzzleComponent(){
	}
	
};
