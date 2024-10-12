// Copyright Moonlit Fantasy Nation


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include <EnhancedInputComponent.h>
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	//随便记一下 就是服务器改变数据可以相应到不同客户端上 叫replication
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
	//EMouseLockMode::DoNotLock 这个值表示 不锁定鼠标，即鼠标可以自由移动到视口之外（通常是到其他应用程序或桌面）
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
	//Cast<IEnemyInterface>(CursorHit.GetActor());//但是已经不需要cast了因为pointer var已经有个wrapper来检查：TScriptInterface

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
