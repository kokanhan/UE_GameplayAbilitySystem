// Copyright Moonlit Fantasy Nation


#include "Character/AuraEnemy.h"
#include "Aura/Aura.h"
#include "DrawDebugHelpers.h"
#include <AbilitySystem/AuraAbilitySystemComponent.h>
#include <AbilitySystem/AuraAttributeSet.h>
//#include "Kismet/GameplayStatics.h" // For UGameplayStatics
AAuraEnemy::AAuraEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    //use code to auto-set BPs collision presets, no manual hard work:)
    //但是代码的优先级是低于BP设置的，要确保这一点
    GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);

    AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

    AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSte");
}
void AAuraEnemy::HighLightActor()
{
    //bHighlighted = true; 不需要这个boolean了
    GetMesh()->SetRenderCustomDepth(true);
    GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);//设置一次就可以了 但是这里被设置很多次 因为操作简单暂时不影响performance

    Weapon->SetRenderCustomDepth(true);
    Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighLightActor()
{
    //bHighlighted = false;
    GetMesh()->SetRenderCustomDepth(false);

    Weapon->SetRenderCustomDepth(false);
}

void AAuraEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //use post-process highlight instead
    /*if (bHighlighted)
    {
        FVector SphereCenter = GetActorLocation();
        float SphereRadius = 50.0f;
        FColor SphereColor = FColor::Red;
        DrawDebugSphere(
            GetWorld(),
            SphereCenter,
            SphereRadius,
            12,
            SphereColor,
            false,
            0.0f,
            0,
            1.0f
        );
    }*/
}

void AAuraEnemy::BeginPlay()
{
    Super::BeginPlay();
    check(AbilitySystemComponent);//也可以不写 如果AbilitySystemComponent是nullprt，AAuraEnemy constructor里会报错
    AbilitySystemComponent->InitAbilityActorInfo(this, this);  // AI角色的所有者和表示者都是自己

}
