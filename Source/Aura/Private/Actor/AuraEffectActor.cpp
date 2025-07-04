#include "Actor/AuraEffectActor.h"

#include "Components/SphereComponent.h"
#include <AbilitySystemInterface.h>
#include <AbilitySystem/AuraAttributeSet.h>

AAuraEffectActor::AAuraEffectActor() {
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComponent->SetupAttachment(GetRootComponent());
}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	if (auto* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor)) {
		const auto* const ASC = ASCInterface->GetAbilitySystemComponent();
		const auto* AttributeSet = Cast<UAuraAttributeSet>(ASC->GetAttributeSet(UAuraAttributeSet::StaticClass()));
		const_cast<UAuraAttributeSet*>(AttributeSet)->SetHealth(AttributeSet->GetHealth() + 25.f);
		Destroy();
	}
}

void AAuraEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex) {

}

void AAuraEffectActor::BeginPlay() {
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::EndOverlap);
}

