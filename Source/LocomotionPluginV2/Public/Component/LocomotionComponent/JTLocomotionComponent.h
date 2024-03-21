// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "JTLocomotionComponent.generated.h"

UENUM(BlueprintType)
enum class EActionMovement : uint8
{
	Walk UMETA(DisplayName = "Walk"),
	Sprint   UMETA(DisplayName = "Sprint")
};

UENUM(BlueprintType)
enum class ECurrentMovement : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	StopTransition UMETA(DisplayName = "Stop Transition"),
	Walk UMETA(DisplayName = "Walk"),
	Sprint   UMETA(DisplayName = "Sprint")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOCOMOTIONPLUGINV2_API UJTLocomotionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJTLocomotionComponent();

	void Move(const FInputActionValue& Value);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region function
private:
	void UpdateRotation();
	void UpdateMovement();
#pragma endregion


#pragma region Variable
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName AnimCurveRotationRate = "JT_RotationRate";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName AnimCurveMaxAcceleration = "JT_StartWalkSpeed";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName AnimCurveWalkSpeed = "JT_WalkSpeed";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName AnimCurveForceDirection = "JT_ForceDirection";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName AnimCurveSprint = "JT_Sprint";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName AnimCurveWalk = "JT_Walk";

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FName AnimCurveBrakingDeceleration = "JT_BrakingDeceleration";

	UPROPERTY(BlueprintReadWrite)
	EActionMovement ActionMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECurrentMovement CurrentMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool MovementInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultWalkSpeed = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultSprintSpeed = 600;

	UPROPERTY(BlueprintReadOnly)
	float TurnAngle;

	UPROPERTY(BlueprintReadOnly)
	FRotator RotationFake;

private:
	UCharacterMovementComponent* MovementComp;
	ACharacter* Owner;
	float speed;
	float PreviousWalkSpeed;
	float DefaultRotationRate;
	float DefaultBrakingDeceleration;
	float DefaultMovementSpeed;
	float DefaultMaxAcceleration;
	float RotateStrength;
	FRotator MovementRotation;
	bool InRotate;
	bool ForceDirectionDoOnce;
	bool ForceDirection;
	FVector2D MovementVector;
	float PreviousSpeed;
	FVector2D PreMovementVector;
	FVector2D PreMovementVelocity;
	FVector RightDirection;
	FVector ForwardDirection;
#pragma endregion
};
