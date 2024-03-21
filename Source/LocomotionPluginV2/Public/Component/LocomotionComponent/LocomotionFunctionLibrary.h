// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Component/LocomotionComponent/JTLocomotionComponent.h"
#include "LocomotionFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class LOCOMOTIONPLUGINV2_API ULocomotionFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Locomotion")
	static float GetTurnAngle(AActor* Actor)
	{
		UJTLocomotionComponent* LocomotionComp = Actor->GetComponentByClass<UJTLocomotionComponent>();
		return LocomotionComp ? LocomotionComp->TurnAngle : -1;
	};

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Locomotion")
	static EActionMovement GetActionMovement(AActor* Actor)
	{
		UJTLocomotionComponent* LocomotionComp = Actor->GetComponentByClass<UJTLocomotionComponent>();
		return LocomotionComp ? LocomotionComp->ActionMovement : EActionMovement::Walk;
	};
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Locomotion")
	static bool IsMoving(AActor* Actor)
	{
		UJTLocomotionComponent* LocomotionComp = Actor->GetComponentByClass<UJTLocomotionComponent>();
		return LocomotionComp ? LocomotionComp->MovementInput : false;
	};
};
