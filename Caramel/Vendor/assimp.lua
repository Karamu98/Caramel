project "assimp"
	location "assimp"
	kind "StaticLib"
	warnings "Off"
	optimize "Speed"
	language "C++"
    staticruntime "on"

	targetdir ("../../Binaries/" .. outputdir .. "/Vendor/%{prj.name}")
    objdir ("../../Intermediate/" .. outputdir .. "/Vendor/%{prj.name}")

	files {
		-- Dependencies
		'assimp/contrib/unzip/**',
		'assimp/contrib/irrXML/**',
		'assimp/contrib/zlib/*',
		-- Common
		'assimp/code/Common/**',
		'assimp/code/PostProcessing/**',
		'assimp/code/Material/**',
		'assimp/code/CApi/**',
		'assimp/code/Geometry/**',
		-- Importers
		'assimp/code/AssetLib/IQM/**',
		'assimp/code/AssetLib/Assbin/**',

		'assimp/code/AssetLib/Collada/**',
		'assimp/code/AssetLib/Obj/**',
		-- 'assimp/code/AssetLib/Blender/**', 'assimp/contrib/poly2tri/poly2tri/**',
		'assimp/code/AssetLib/FBX/**',
		-- 'assimp/code/glTF2/**',
		-- 'assimp/code/glTF/**',
		'assimp/code/Assbin/**' -- For caching
	}

	includedirs {
		'_config_headers/',
		'_config_headers/assimp/',
		'assimp/',
		'assimp/contrib/',
		'assimp/contrib/irrXML/',
		'assimp/contrib/unzip/',
		'assimp/contrib/rapidjson/include/',
		'assimp/contrib/pugixml/src/',
		'assimp/contrib/zlib/',
		'assimp/contrib/utf8cpp/source',
		'assimp/code',
		'assimp/include',
	}

	-- Importers
	defines {
		'ASSIMP_BUILD_NO_3D_IMPORTER',
		'ASSIMP_BUILD_NO_3DS_IMPORTER',
		'ASSIMP_BUILD_NO_3MF_IMPORTER',
		'ASSIMP_BUILD_NO_AC_IMPORTER',
		'ASSIMP_BUILD_NO_AMF_IMPORTER',
		'ASSIMP_BUILD_NO_ASE_IMPORTER',
		-- 'ASSIMP_BUILD_NO_ASSBIN_IMPORTER'
		'ASSIMP_BUILD_NO_B3D_IMPORTER',
		'ASSIMP_BUILD_NO_BLEND_IMPORTER',
		'ASSIMP_BUILD_NO_BVH_IMPORTER',
		'ASSIMP_BUILD_NO_C4D_IMPORTER',
		'ASSIMP_BUILD_NO_COB_IMPORTER',
		-- 'ASSIMP_BUILD_NO_COLLADA_IMPORTER',
		'ASSIMP_BUILD_NO_CSM_IMPORTER',
		'ASSIMP_BUILD_NO_DXF_IMPORTER',
		-- 'ASSIMP_BUILD_NO_FBX_IMPORTER',
		'ASSIMP_BUILD_NO_GLTF_IMPORTER',
		'ASSIMP_BUILD_NO_HMP_IMPORTER',
		'ASSIMP_BUILD_NO_IFC_IMPORTER',
		'ASSIMP_BUILD_NO_IRR_IMPORTER',
		'ASSIMP_BUILD_NO_IRRMESH_IMPORTER',
		'ASSIMP_BUILD_NO_LWO_IMPORTER',
		'ASSIMP_BUILD_NO_LWS_IMPORTER',
		'ASSIMP_BUILD_NO_M3D_IMPORTER',
		'ASSIMP_BUILD_NO_MD2_IMPORTER',
		'ASSIMP_BUILD_NO_MD3_IMPORTER',
		'ASSIMP_BUILD_NO_MD5_IMPORTER',
		'ASSIMP_BUILD_NO_MDC_IMPORTER',
		'ASSIMP_BUILD_NO_MDL_IMPORTER',
		'ASSIMP_BUILD_NO_MMD_IMPORTER',
		'ASSIMP_BUILD_NO_MS3D_IMPORTER',
		'ASSIMP_BUILD_NO_NDO_IMPORTER',
		'ASSIMP_BUILD_NO_NFF_IMPORTER',
		-- 'ASSIMP_BUILD_NO_OBJ_IMPORTER',
		'ASSIMP_BUILD_NO_OFF_IMPORTER',
		'ASSIMP_BUILD_NO_OGRE_IMPORTER',
		'ASSIMP_BUILD_NO_OPENGEX_IMPORTER',
		'ASSIMP_BUILD_NO_PLY_IMPORTER',
		'ASSIMP_BUILD_NO_Q3BSP_IMPORTER',
		'ASSIMP_BUILD_NO_Q3D_IMPORTER',
		'ASSIMP_BUILD_NO_RAW_IMPORTER',
		'ASSIMP_BUILD_NO_SIB_IMPORTER',
		'ASSIMP_BUILD_NO_SMD_IMPORTER',
		'ASSIMP_BUILD_NO_STEP_IMPORTER',
		'ASSIMP_BUILD_NO_STL_IMPORTER',
		'ASSIMP_BUILD_NO_TERRAGEN_IMPORTER',
		'ASSIMP_BUILD_NO_X_IMPORTER',
		'ASSIMP_BUILD_NO_X3D_IMPORTER',
		'ASSIMP_BUILD_NO_XGL_IMPORTER'
	}
	-- Exporters
	defines {
		'ASSIMP_BUILD_NO_COLLADA_EXPORTER',
		'ASSIMP_BUILD_NO_X_EXPORTER',
		'ASSIMP_BUILD_NO_STEP_EXPORTER',
		'ASSIMP_BUILD_NO_OBJ_EXPORTER',
		'ASSIMP_BUILD_NO_STL_EXPORTER',
		'ASSIMP_BUILD_NO_PLY_EXPORTER',
		'ASSIMP_BUILD_NO_3DS_EXPORTER',
		'ASSIMP_BUILD_NO_GLTF_EXPORTER',
		-- 'ASSIMP_BUILD_NO_ASSBIN_EXPORTER',
		'ASSIMP_BUILD_NO_ASSXML_EXPORTER',
		'ASSIMP_BUILD_NO_X3D_EXPORTER',
		'ASSIMP_BUILD_NO_FBX_EXPORTER',
		'ASSIMP_BUILD_NO_M3D_EXPORTER',
		'ASSIMP_BUILD_NO_3MF_EXPORTER',
		'ASSIMP_BUILD_NO_ASSJSON_EXPORTER'
	}
		
	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

	filter {"system:windows", "configurations:Release"}
		buildoptions "/MT"