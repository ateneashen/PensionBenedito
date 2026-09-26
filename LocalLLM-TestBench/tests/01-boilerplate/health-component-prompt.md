# Test 01: Health Component (Boilerplate)

## Objetivo
Evaluar si el modelo local puede generar un ActorComponent de salud con las convenciones correctas de UE 4.27.2.

## Prompt para el modelo local

```
Create a C++ ActorComponent for Unreal Engine 4.27.2 called UHealthComponent with the following requirements:

1. Properties:
   - CurrentHealth (float, editable, clamped 0-MaxHealth)
   - MaxHealth (float, EditDefaultsOnly)
   - bIsDead (bool, BlueprintReadOnly)

2. Delegates:
   - FOnHealthChanged: broadcast when health changes (params: float CurrentHealth, float MaxHealth)
   - FOnDeath: broadcast when health reaches 0

3. Functions:
   - ApplyDamage(float DamageAmount)
   - Heal(float HealAmount)
   - ResetHealth()

4. Requirements:
   - Use DECLARE_DYNAMIC_MULTICAST_DELEGATE for Blueprint compatibility
   - Proper UPROPERTY specifiers (VisibleAnywhere for components, EditDefaultsOnly for config)
   - Category and tooltips for all properties
   - Bind/unbind delegates properly
   - Include logging category

Generate both .h and .cpp files with proper UE4.27 conventions.
```

## Criterios de evaluación

### Correctitud (0-10)
- [ ] Usa `DECLARE_DYNAMIC_MULTICAST_DELEGATE` (no nativo, para BP)
- [ ] `UPROPERTY(VisibleAnywhere, BlueprintReadOnly)` para componentes
- [ ] `UPROPERTY(EditDefaultsOnly)` para MaxHealth
- [ ] `meta=(ClampMin="0.0", ClampMax="MaxHealth")` para CurrentHealth
- [ ] `BlueprintAssignable` en delegates
- [ ] `Category="Health"` en todas las propiedades

### Seguridad (0-10)
- [ ] No usa `TObjectPtr` (es UE5)
- [ ] Verifica `IsValid()` antes de broadcast si es necesario
- [ ] `DECLARE_LOG_CATEGORY_EXTERN` + `DEFINE_LOG_CATEGORY`
- [ ] No hard references a Blueprints

### Eficiencia (0-10)
- [ ] Tick desactivado (no necesita tick)
- [ ] Sin IO o carga síncrona
- [ ] Código limpio y DRY

### Usabilidad (0-10)
- [ ] Compila sin warnings
- [ ] Se puede integrar directamente en un proyecto UE4.27
- [ ] Fácil de entender y mantener

## Resultado esperado

El modelo debería generar algo similar a la implementación estándar de un componente de salud con delegates, siguiendo las convenciones de Epic para 4.27.