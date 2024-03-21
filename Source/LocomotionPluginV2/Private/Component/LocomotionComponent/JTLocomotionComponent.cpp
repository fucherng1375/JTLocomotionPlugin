// Fill out your copyright notice in the Description page of Project Settings.

#include "Component/LocomotionComponent/JTLocomotionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UJTLocomotionComponent::UJTLocomotionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UJTLocomotionComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ACharacter>(GetOwner());
	if (!Owner) return;
	MovementComp = Owner->GetCharacterMovement();
	DefaultRotationRate = MovementComp->RotationRate.Yaw;
	DefaultMaxAcceleration = MovementComp->MaxAcceleration;
	DefaultBrakingDeceleration = MovementComp->BrakingDecelerationWalking;
}


// Called every frame
void UJTLocomotionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!Owner) return;
	speed = FVector2D(Owner->GetVelocity()).Length();
	UpdateRotation();
	UpdateMovement();
}

void UJTLocomotionComponent::UpdateRotation()
{
	MovementRotation = Owner->GetLastMovementInputVector().GetUnsafeNormal2D().Rotation();
	RotationFake = MovementRotation;

	MovementInput = Owner->GetLastMovementInputVector().Length() > 0;

	float ConstTurnAngle = Owner->GetLastMovementInputVector().Length() == 0 ? 0 : Owner->GetActorTransform().InverseTransformRotation(FQuat(MovementRotation)).Rotator().Yaw;

	if (FMath::Abs(ConstTurnAngle) >= 45 && !InRotate)
	{
		TurnAngle = Owner->GetActorTransform().InverseTransformRotation(FQuat(MovementRotation)).Rotator().Yaw;
		RotateStrength = 1 + (1 - (FMath::Abs(ConstTurnAngle) / 180));
		PreMovementVector = MovementVector;
		InRotate = true;
	}
	if (FMath::Abs(ConstTurnAngle) < 45 && InRotate || Owner->GetLastMovementInputVector().Length() == 0.0f)
	{
		TurnAngle = 0;
		RotateStrength = 1;
		InRotate = false;
	}

	if (MovementComp)
	{
		MovementComp->RotationRate.Yaw = speed <= 0.0f ? DefaultRotationRate : MovementComp->RotationRate.Yaw;
		UAnimInstance* AnimInstant = Owner->GetMesh()->GetAnimInstance();
		TArray<FName> AllActivateCurve; AnimInstant->GetActiveCurveNames(EAnimCurveType::AttributeCurve, AllActivateCurve);
		MovementComp->RotationRate.Yaw = AllActivateCurve.Contains(AnimCurveRotationRate) ? (DefaultRotationRate * AnimInstant->GetCurveValue(AnimCurveRotationRate)) * RotateStrength: FMath::FInterpTo(MovementComp->RotationRate.Yaw, DefaultRotationRate, GetWorld()->GetDeltaSeconds(), 10);
		ForceDirection = AllActivateCurve.Contains(AnimCurveForceDirection);
		MovementComp->BrakingDecelerationWalking = AllActivateCurve.Contains(AnimCurveBrakingDeceleration) ? DefaultBrakingDeceleration * AnimInstant->GetCurveValue(AnimCurveBrakingDeceleration) : DefaultBrakingDeceleration;
		
		// Cache the direction while there is no any rotate, but require Force Direction
		if (ForceDirection && !ForceDirectionDoOnce && !InRotate)
		{
			ForceDirectionDoOnce = true;
			PreMovementVector = MovementVector;
		}
		else if (ForceDirection && ForceDirectionDoOnce && !InRotate || Owner->GetLastMovementInputVector().Length() == 0.0f)
		{
			ForceDirectionDoOnce = false;
		}


		switch (ActionMovement)
		{
		case EActionMovement::Walk: MovementComp->MaxWalkSpeed = AllActivateCurve.Contains(AnimCurveWalk) ? FMath::Lerp(PreviousWalkSpeed, DefaultWalkSpeed, AnimInstant->GetCurveValue(AnimCurveWalk)) : MovementComp->MaxWalkSpeed;
			break;
		case EActionMovement::Sprint:
			MovementComp->MaxWalkSpeed = AnimInstant->GetCurveValue(AnimCurveSprint) ? FMath::Lerp(DefaultWalkSpeed, DefaultSprintSpeed, AnimInstant->GetCurveValue(AnimCurveSprint)) : MovementComp->MaxWalkSpeed;
			PreviousWalkSpeed = MovementComp->MaxWalkSpeed;
			break;
		default:
			break;
		}
	}
}

void UJTLocomotionComponent::UpdateMovement()
{
	AController* Controller = Owner->GetController();
	if (Controller != nullptr)
	{
		if (!InRotate && !ForceDirection)
		{
			// find out which way is forward
			const FRotator ControllerRotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, ControllerRotation.Yaw, 0);

			// get forward vector
			ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			// get right vector 
			RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		}
	}
}

void UJTLocomotionComponent::Move(const FInputActionValue& Value)
{
	if (CurrentMovement != ECurrentMovement::StopTransition)
	{
		MovementVector = ForceDirection ? PreMovementVector : Value.Get<FVector2D>();
		Owner->AddMovementInput(ForwardDirection, MovementVector.Y);
		Owner->AddMovementInput(RightDirection, MovementVector.X);
	}
}