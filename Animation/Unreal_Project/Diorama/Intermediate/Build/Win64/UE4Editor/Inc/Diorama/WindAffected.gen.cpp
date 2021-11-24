// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Diorama/WindAffected.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWindAffected() {}
// Cross Module References
	DIORAMA_API UClass* Z_Construct_UClass_UWindAffected_NoRegister();
	DIORAMA_API UClass* Z_Construct_UClass_UWindAffected();
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
	UPackage* Z_Construct_UPackage__Script_Diorama();
// End Cross Module References
	void UWindAffected::StaticRegisterNativesUWindAffected()
	{
	}
	UClass* Z_Construct_UClass_UWindAffected_NoRegister()
	{
		return UWindAffected::StaticClass();
	}
	struct Z_Construct_UClass_UWindAffected_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWindAffected_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_Diorama,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWindAffected_Statics::Class_MetaDataParams[] = {
		{ "BlueprintSpawnableComponent", "" },
		{ "ClassGroupNames", "Custom" },
		{ "IncludePath", "WindAffected.h" },
		{ "ModuleRelativePath", "WindAffected.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWindAffected_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWindAffected>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UWindAffected_Statics::ClassParams = {
		&UWindAffected::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x00B000A4u,
		METADATA_PARAMS(Z_Construct_UClass_UWindAffected_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UWindAffected_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UWindAffected()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UWindAffected_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UWindAffected, 1065337810);
	template<> DIORAMA_API UClass* StaticClass<UWindAffected>()
	{
		return UWindAffected::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UWindAffected(Z_Construct_UClass_UWindAffected, &UWindAffected::StaticClass, TEXT("/Script/Diorama"), TEXT("UWindAffected"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWindAffected);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
