#pragma once

// Fortnite (1.11) SDK

// Generated with <3 by Jacobb626

#include <set>
#include <string>
#include <locale>
#include <Windows.h>
#include <vector>

#include "SDK/FN_Basic.hpp"

#include "SDK/FN_Vivox_structs.hpp"
#include "SDK/FN_Vivox_classes.hpp"
#include "SDK/FN_Vivox_parameters.hpp"
#include "SDK/FN_CoreUObject_structs.hpp"
#include "SDK/FN_CoreUObject_classes.hpp"
#include "SDK/FN_CoreUObject_parameters.hpp"
#include "SDK/FN_SlateCore_structs.hpp"
#include "SDK/FN_SlateCore_classes.hpp"
#include "SDK/FN_SlateCore_parameters.hpp"
#include "SDK/FN_WebBrowser_structs.hpp"
#include "SDK/FN_WebBrowser_classes.hpp"
#include "SDK/FN_WebBrowser_parameters.hpp"
#include "SDK/FN_JsonUtilities_structs.hpp"
#include "SDK/FN_JsonUtilities_classes.hpp"
#include "SDK/FN_JsonUtilities_parameters.hpp"
#include "SDK/FN_SocialTypes_structs.hpp"
#include "SDK/FN_SocialTypes_classes.hpp"
#include "SDK/FN_SocialTypes_parameters.hpp"
#include "SDK/FN_SessionMessages_structs.hpp"
#include "SDK/FN_SessionMessages_classes.hpp"
#include "SDK/FN_SessionMessages_parameters.hpp"
#include "SDK/FN_Serialization_structs.hpp"
#include "SDK/FN_Serialization_classes.hpp"
#include "SDK/FN_Serialization_parameters.hpp"
#include "SDK/FN_Renderer_structs.hpp"
#include "SDK/FN_Renderer_classes.hpp"
#include "SDK/FN_Renderer_parameters.hpp"
#include "SDK/FN_EngineMessages_structs.hpp"
#include "SDK/FN_EngineMessages_classes.hpp"
#include "SDK/FN_EngineMessages_parameters.hpp"
#include "SDK/FN_ScheduledEvents_structs.hpp"
#include "SDK/FN_ScheduledEvents_classes.hpp"
#include "SDK/FN_ScheduledEvents_parameters.hpp"
#include "SDK/FN_NVIDIAGfeSDK_structs.hpp"
#include "SDK/FN_NVIDIAGfeSDK_classes.hpp"
#include "SDK/FN_NVIDIAGfeSDK_parameters.hpp"
#include "SDK/FN_WidgetCarousel_structs.hpp"
#include "SDK/FN_WidgetCarousel_classes.hpp"
#include "SDK/FN_WidgetCarousel_parameters.hpp"
#include "SDK/FN_AnimationCore_structs.hpp"
#include "SDK/FN_AnimationCore_classes.hpp"
#include "SDK/FN_AnimationCore_parameters.hpp"
#include "SDK/FN_InputCore_structs.hpp"
#include "SDK/FN_InputCore_classes.hpp"
#include "SDK/FN_InputCore_parameters.hpp"
#include "SDK/FN_TcpMessaging_structs.hpp"
#include "SDK/FN_TcpMessaging_classes.hpp"
#include "SDK/FN_TcpMessaging_parameters.hpp"
#include "SDK/FN_WmfMediaFactory_structs.hpp"
#include "SDK/FN_WmfMediaFactory_classes.hpp"
#include "SDK/FN_WmfMediaFactory_parameters.hpp"
#include "SDK/FN_AvfMediaFactory_structs.hpp"
#include "SDK/FN_AvfMediaFactory_classes.hpp"
#include "SDK/FN_AvfMediaFactory_parameters.hpp"
#include "SDK/FN_PacketHandler_structs.hpp"
#include "SDK/FN_PacketHandler_classes.hpp"
#include "SDK/FN_PacketHandler_parameters.hpp"
#include "SDK/FN_Slate_structs.hpp"
#include "SDK/FN_Slate_classes.hpp"
#include "SDK/FN_Slate_parameters.hpp"
#include "SDK/FN_Engine_structs.hpp"
#include "SDK/FN_Engine_classes.hpp"
#include "SDK/FN_Engine_parameters.hpp"
#include "SDK/FN_PurchaseFlow_structs.hpp"
#include "SDK/FN_PurchaseFlow_classes.hpp"
#include "SDK/FN_PurchaseFlow_parameters.hpp"
#include "SDK/FN_AnimGraphRuntime_structs.hpp"
#include "SDK/FN_AnimGraphRuntime_classes.hpp"
#include "SDK/FN_AnimGraphRuntime_parameters.hpp"
#include "SDK/FN_AndroidPermission_structs.hpp"
#include "SDK/FN_AndroidPermission_classes.hpp"
#include "SDK/FN_AndroidPermission_parameters.hpp"
#include "SDK/FN_GameplayTags_structs.hpp"
#include "SDK/FN_GameplayTags_classes.hpp"
#include "SDK/FN_GameplayTags_parameters.hpp"
#include "SDK/FN_UACCommon_structs.hpp"
#include "SDK/FN_UACCommon_classes.hpp"
#include "SDK/FN_UACCommon_parameters.hpp"
#include "SDK/FN_OnlineSubsystem_structs.hpp"
#include "SDK/FN_OnlineSubsystem_classes.hpp"
#include "SDK/FN_OnlineSubsystem_parameters.hpp"
#include "SDK/FN_MovieScene_structs.hpp"
#include "SDK/FN_MovieScene_classes.hpp"
#include "SDK/FN_MovieScene_parameters.hpp"
#include "SDK/FN_MovieSceneTracks_structs.hpp"
#include "SDK/FN_MovieSceneTracks_classes.hpp"
#include "SDK/FN_MovieSceneTracks_parameters.hpp"
#include "SDK/FN_OodleHandlerComponent_structs.hpp"
#include "SDK/FN_OodleHandlerComponent_classes.hpp"
#include "SDK/FN_OodleHandlerComponent_parameters.hpp"
#include "SDK/FN_OnlineSubsystemTencent_structs.hpp"
#include "SDK/FN_OnlineSubsystemTencent_classes.hpp"
#include "SDK/FN_OnlineSubsystemTencent_parameters.hpp"
#include "SDK/FN_BuildPatchServices_structs.hpp"
#include "SDK/FN_BuildPatchServices_classes.hpp"
#include "SDK/FN_BuildPatchServices_parameters.hpp"
#include "SDK/FN_OnlineSubsystemUtils_structs.hpp"
#include "SDK/FN_OnlineSubsystemUtils_classes.hpp"
#include "SDK/FN_OnlineSubsystemUtils_parameters.hpp"
#include "SDK/FN_UMG_structs.hpp"
#include "SDK/FN_UMG_classes.hpp"
#include "SDK/FN_UMG_parameters.hpp"
#include "SDK/FN_BlueprintContext_structs.hpp"
#include "SDK/FN_BlueprintContext_classes.hpp"
#include "SDK/FN_BlueprintContext_parameters.hpp"
#include "SDK/FN_ClothingSystemRuntimeInterface_structs.hpp"
#include "SDK/FN_ClothingSystemRuntimeInterface_classes.hpp"
#include "SDK/FN_ClothingSystemRuntimeInterface_parameters.hpp"
#include "SDK/FN_Overlay_structs.hpp"
#include "SDK/FN_Overlay_classes.hpp"
#include "SDK/FN_Overlay_parameters.hpp"
#include "SDK/FN_MoviePlayer_structs.hpp"
#include "SDK/FN_MoviePlayer_classes.hpp"
#include "SDK/FN_MoviePlayer_parameters.hpp"
#include "SDK/FN_GameplayTasks_structs.hpp"
#include "SDK/FN_GameplayTasks_classes.hpp"
#include "SDK/FN_GameplayTasks_parameters.hpp"
#include "SDK/FN_AnalyticsBlueprintLibrary_structs.hpp"
#include "SDK/FN_AnalyticsBlueprintLibrary_classes.hpp"
#include "SDK/FN_AnalyticsBlueprintLibrary_parameters.hpp"
#include "SDK/FN_SubtitlesWidgets_structs.hpp"
#include "SDK/FN_SubtitlesWidgets_classes.hpp"
#include "SDK/FN_SubtitlesWidgets_parameters.hpp"
#include "SDK/FN_McpProfileSys_structs.hpp"
#include "SDK/FN_McpProfileSys_classes.hpp"
#include "SDK/FN_McpProfileSys_parameters.hpp"
#include "SDK/FN_AudioMixer_structs.hpp"
#include "SDK/FN_AudioMixer_classes.hpp"
#include "SDK/FN_AudioMixer_parameters.hpp"
#include "SDK/FN_ClothingSystemRuntime_structs.hpp"
#include "SDK/FN_ClothingSystemRuntime_classes.hpp"
#include "SDK/FN_ClothingSystemRuntime_parameters.hpp"
#include "SDK/FN_SocialDefaults_structs.hpp"
#include "SDK/FN_SocialDefaults_classes.hpp"
#include "SDK/FN_SocialDefaults_parameters.hpp"
#include "SDK/FN_MediaAssets_structs.hpp"
#include "SDK/FN_MediaAssets_classes.hpp"
#include "SDK/FN_MediaAssets_parameters.hpp"
#include "SDK/FN_EngineSettings_structs.hpp"
#include "SDK/FN_EngineSettings_classes.hpp"
#include "SDK/FN_EngineSettings_parameters.hpp"
#include "SDK/FN_AssetRegistry_structs.hpp"
#include "SDK/FN_AssetRegistry_classes.hpp"
#include "SDK/FN_AssetRegistry_parameters.hpp"
#include "SDK/FN_MRMesh_structs.hpp"
#include "SDK/FN_MRMesh_classes.hpp"
#include "SDK/FN_MRMesh_parameters.hpp"
#include "SDK/FN_MovieSceneCapture_structs.hpp"
#include "SDK/FN_MovieSceneCapture_classes.hpp"
#include "SDK/FN_MovieSceneCapture_parameters.hpp"
#include "SDK/FN_GeometryCache_structs.hpp"
#include "SDK/FN_GeometryCache_classes.hpp"
#include "SDK/FN_GeometryCache_parameters.hpp"
#include "SDK/FN_EpicGameplayStatsRuntime_structs.hpp"
#include "SDK/FN_EpicGameplayStatsRuntime_classes.hpp"
#include "SDK/FN_EpicGameplayStatsRuntime_parameters.hpp"
#include "SDK/FN_NetUI_structs.hpp"
#include "SDK/FN_NetUI_classes.hpp"
#include "SDK/FN_NetUI_parameters.hpp"
#include "SDK/FN_CinematicCamera_structs.hpp"
#include "SDK/FN_CinematicCamera_classes.hpp"
#include "SDK/FN_CinematicCamera_parameters.hpp"
#include "SDK/FN_CommonUI_structs.hpp"
#include "SDK/FN_CommonUI_classes.hpp"
#include "SDK/FN_CommonUI_parameters.hpp"
#include "SDK/FN_MaterialShaderQualitySettings_structs.hpp"
#include "SDK/FN_MaterialShaderQualitySettings_classes.hpp"
#include "SDK/FN_MaterialShaderQualitySettings_parameters.hpp"
#include "SDK/FN_LevelSequence_structs.hpp"
#include "SDK/FN_LevelSequence_classes.hpp"
#include "SDK/FN_LevelSequence_parameters.hpp"
#include "SDK/FN_ImmediatePhysics_structs.hpp"
#include "SDK/FN_ImmediatePhysics_classes.hpp"
#include "SDK/FN_ImmediatePhysics_parameters.hpp"
#include "SDK/FN_Landscape_structs.hpp"
#include "SDK/FN_Landscape_classes.hpp"
#include "SDK/FN_Landscape_parameters.hpp"
#include "SDK/FN_Gauntlet_structs.hpp"
#include "SDK/FN_Gauntlet_classes.hpp"
#include "SDK/FN_Gauntlet_parameters.hpp"
#include "SDK/FN_SignificanceManager_structs.hpp"
#include "SDK/FN_SignificanceManager_classes.hpp"
#include "SDK/FN_SignificanceManager_parameters.hpp"
#include "SDK/FN_Rejoin_structs.hpp"
#include "SDK/FN_Rejoin_classes.hpp"
#include "SDK/FN_Rejoin_parameters.hpp"
#include "SDK/FN_HeadMountedDisplay_structs.hpp"
#include "SDK/FN_HeadMountedDisplay_classes.hpp"
#include "SDK/FN_HeadMountedDisplay_parameters.hpp"
#include "SDK/FN_Account_structs.hpp"
#include "SDK/FN_Account_classes.hpp"
#include "SDK/FN_Account_parameters.hpp"
#include "SDK/FN_Foliage_structs.hpp"
#include "SDK/FN_Foliage_classes.hpp"
#include "SDK/FN_Foliage_parameters.hpp"
#include "SDK/FN_Qos_structs.hpp"
#include "SDK/FN_Qos_classes.hpp"
#include "SDK/FN_Qos_parameters.hpp"
#include "SDK/FN_Hotfix_structs.hpp"
#include "SDK/FN_Hotfix_classes.hpp"
#include "SDK/FN_Hotfix_parameters.hpp"
#include "SDK/FN_Lobby_structs.hpp"
#include "SDK/FN_Lobby_classes.hpp"
#include "SDK/FN_Lobby_parameters.hpp"
#include "SDK/FN_EpicCMSUIFramework_structs.hpp"
#include "SDK/FN_EpicCMSUIFramework_classes.hpp"
#include "SDK/FN_EpicCMSUIFramework_parameters.hpp"
#include "SDK/FN_Party_structs.hpp"
#include "SDK/FN_Party_classes.hpp"
#include "SDK/FN_Party_parameters.hpp"
#include "SDK/FN_PhysXVehicles_structs.hpp"
#include "SDK/FN_PhysXVehicles_classes.hpp"
#include "SDK/FN_PhysXVehicles_parameters.hpp"
#include "SDK/FN_AIModule_structs.hpp"
#include "SDK/FN_AIModule_classes.hpp"
#include "SDK/FN_AIModule_parameters.hpp"
#include "SDK/FN_GameplayAbilities_structs.hpp"
#include "SDK/FN_GameplayAbilities_classes.hpp"
#include "SDK/FN_GameplayAbilities_parameters.hpp"
#include "SDK/FN_GameSubCatalog_structs.hpp"
#include "SDK/FN_GameSubCatalog_classes.hpp"
#include "SDK/FN_GameSubCatalog_parameters.hpp"
#include "SDK/FN_FortniteGame_structs.hpp"
#include "SDK/FN_FortniteGame_classes.hpp"
#include "SDK/FN_FortniteGame_parameters.hpp"
#include "SDK/FN_FortniteUI_structs.hpp"
#include "SDK/FN_FortniteUI_classes.hpp"
#include "SDK/FN_FortniteUI_parameters.hpp"
