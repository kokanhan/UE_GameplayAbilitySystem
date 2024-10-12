// Copyright Moonlit Fantasy Nation


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include <EnhancedInputComponent.h>
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	//����һ�� ���Ƿ������ı����ݿ�����Ӧ����ͬ�ͻ����� ��replication
	bReplicates = true;

}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);//if check failed, the program will crash.

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	//check(Subsystem);
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	//EMouseLockMode::DoNotLock ���ֵ��ʾ ��������꣬�������������ƶ����ӿ�֮�⣨ͨ���ǵ�����Ӧ�ó�������棩
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComponent  = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	//const FRotator Rotation = GetControlRotation();
	const FRotator Rotation = FRotator::ZeroRotator;
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	// Check if CursorHit's actor has a interface or not
	// If not, the cast will auto return nullptr
	//Cast<IEnemyInterface>(CursorHit.GetActor());//�����Ѿ�����Ҫcast����Ϊpointer var�Ѿ��и�wrapper����飺TScriptInterface

	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	* Line trace from cursor. There are several scenarios:
	*  A. LastActor is null & ThisActor is null 
	*		- Do nothing
	*  B. Last Actor is null && ThisActor is valid
	*		- Hightlight ThisActor
	*  C. LastActor is valid && ThisActor is null
	*		-UnHighlight LastActor
	*  D. Both actors are valid, but LastActor != ThisActor
	*		- UnHighlight LastActor, and Highlight ThisActor
	*  E. Both actors are valid, and are the same actor
	*		- Do nothing 
	**/

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// Case B - Last Actor is null && ThisActor is valid
			ThisActor->HighLightActor();
		}
		else
		{
			// Case A - Both are null, do nothing
		}
	}
	else
	{
		// LastActor is valid
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighLightActor();
		}
		else
		{
			if (ThisActor != LastActor) 
			{
				// Case D - Both actors are valid, but LastActor != ThisActor
				ThisActor->HighLightActor();
				LastActor->UnHighLightActor();
			}
			else
			{
				// Case E - Both actors are valid, and are the same actor, then do nothing
			}
			
		}
	}
	
}
