#include "FBXFile.h"
#include <fbxsdk.h>
#include <algorithm>
#include <set>
#include <SOIL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/norm.hpp>
#include <iostream>
// only needed for texture cleanup
#define GLEW_NO_GLU
#include "gl_core_4_4.h"

struct ImportAssistor
{
	ImportAssistor() : evaluator(nullptr), loadAnimationOnly(false) {}
	~ImportAssistor() { evaluator = nullptr; }

	FbxScene*				scene;
	FbxAnimEvaluator*		evaluator;
	FbxImporter*			importer;
	std::vector<FBXNode*>	bones;
	std::vector<glm::mat4>	bindPoses;
	bool					loadTextures;
	bool					loadAnimations;
	bool					loadAnimationOnly;

	//FBXFile::UNIT_SCALE		unitScale;
	float					unitScale;

	std::map<std::string,int> boneIndexList;

};

void FBXFile::unload()
{
	delete m_root;
	m_root = nullptr;

#if (_MSC_VER == 1600)
	for each (auto m in m_materials)
		delete m.second;
	for each (auto s in m_skeletons)
		delete s;
	for each (auto a in m_animations)
		delete a.second;
	for each (auto t in m_textures)
	{
		glDeleteTextures(1, &(t.second));
	}
#else
	for (auto m : m_materials)
		delete m.second;
	for (auto s : m_skeletons)
		delete s;
	for (auto a : m_animations)
		delete a.second;
	for (auto t : m_textures)
	{
		glDeleteTextures(1, &(t.second));
	}
#endif

	m_meshes.clear();
	m_lights.clear();
	m_cameras.clear();
	m_materials.clear();
	m_skeletons.clear();
	m_animations.clear();
	m_textures.clear();
}

bool FBXFile::load(const char* a_filename, UNIT_SCALE a_scale /* = FBXFile::UNITS_METER */, bool a_loadTextures /* = true */, bool a_loadAnimations /* = true */ )
{
	if (m_root != nullptr)
	{
		printf("Scene already loaded!\n");
		return false;
	}

	FbxManager* lSdkManager = nullptr;
	FbxScene* lScene = nullptr;

	// The first thing to do is to create the FBX SDK manager which is the 
	// object allocator for almost all the classes in the SDK.
	lSdkManager = FbxManager::Create();
	if ( !lSdkManager )
	{
		printf("Unable to create the FBX SDK manager\n");
		return false;
	}

	// create an IOSettings object
	FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT );
	lSdkManager->SetIOSettings(ios);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"");

	// Initialize the importer by providing a filename.
	bool lImportStatus = lImporter->Initialize(a_filename, -1, lSdkManager->GetIOSettings());

	if ( !lImportStatus )
	{
		printf("Call to FbxImporter::Initialize() failed:\n\t%s\n", lImporter->GetStatus().GetErrorString());
		lImporter->Destroy();
		return false;
	}

	// Create the entity that will hold the scene.
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor,  lSDKMinor,  lSDKRevision;

	unsigned int i;
	bool lStatus;

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	lScene = FbxScene::Create(lSdkManager,"root");

	// Import the scene.
	lStatus = lImporter->Import(lScene);
	lImporter->Destroy();
	if (lStatus == false)
	{
		printf("Unable to open FBX file!\n");
		return false;
	}
	
	float unitScale = 1;

	// convert scale
	if ( lScene->GetGlobalSettings().GetSystemUnit() != FbxSystemUnit::sPredefinedUnits[a_scale] )
	{
		const FbxSystemUnit::ConversionOptions lConversionOptions = {
			false, // mConvertRrsNodes
			true, // mConvertAllLimits
			true, // mConvertClusters
			true, // mConvertLightIntensity
			true, // mConvertPhotometricLProperties
			true  // mConvertCameraClipPlanes
		};

		unitScale = (float)(lScene->GetGlobalSettings().GetSystemUnit().GetScaleFactor() / FbxSystemUnit::sPredefinedUnits[a_scale].GetScaleFactor());
		
		// Convert the scene to meters using the defined options.
		FbxSystemUnit::sPredefinedUnits[a_scale].ConvertScene(lScene, lConversionOptions);
	}
	
	// convert the scene to OpenGL axis (right-handed Y up)
	FbxAxisSystem::OpenGL.ConvertScene(lScene);

	// DID NOT KNOW WE COULD DO THIS!!!!
	/*
	// Convert mesh, NURBS and patch into triangle mesh
	FbxGeometryConverter lGeomConverter(mSdkManager);
	lGeomConverter.Triangulate(mScene, true);

	// Split meshes per material, so that we only have one material per mesh (for VBO support)
	lGeomConverter.SplitMeshesPerMaterial(mScene, true);
	*/

	FbxNode* lNode = lScene->GetRootNode();

	if (lNode != nullptr)
	{
		// store the folder path of the scene
		m_path = a_filename;
		long iLastForward = m_path.find_last_of('/');
		long iLastBackward = m_path.find_last_of('\\');
		if (iLastForward > iLastBackward)
		{
			m_path.resize(iLastForward + 1);
		}
		else if (iLastBackward != 0)
		{
			m_path.resize(iLastBackward + 1);
		}
		else
		{
			m_path = "";
		}

		m_importAssistor = new ImportAssistor();
		m_importAssistor->scene = lScene;
		m_importAssistor->evaluator = lScene->GetAnimationEvaluator();
		m_importAssistor->importer = lImporter;
		m_importAssistor->loadTextures = a_loadTextures;
		m_importAssistor->loadAnimations = a_loadAnimations;
		m_importAssistor->unitScale = unitScale;//a_scale;

		m_root = new FBXNode();
		strcpy(m_root->m_name,"root");
		m_root->m_globalTransform = m_root->m_localTransform = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

		// grab the ambient light data from the scene
		m_ambientLight.x = (float)lScene->GetGlobalSettings().GetAmbientColor().mRed;
		m_ambientLight.y = (float)lScene->GetGlobalSettings().GetAmbientColor().mGreen;
		m_ambientLight.z = (float)lScene->GetGlobalSettings().GetAmbientColor().mBlue;
		m_ambientLight.w = (float)lScene->GetGlobalSettings().GetAmbientColor().mAlpha;

				
		// extract scene (meshes, lights, cameras)
		for (i = 0; i < (unsigned int)lNode->GetChildCount(); ++i)
		{
			extractObject(m_root, (void*)lNode->GetChild(i));
		}
		

		// build skeleton and extract animation keyframes
		if (a_loadAnimations == true &&
			m_importAssistor->bones.size() > 0)
		{
			FBXSkeleton* skeleton = new FBXSkeleton();
			skeleton->m_boneCount = (unsigned int)m_importAssistor->bones.size();
			skeleton->m_nodes = new FBXNode * [ skeleton->m_boneCount ];
			skeleton->m_bones = new glm::mat4[ skeleton->m_boneCount ];
			skeleton->m_bindPoses = new glm::mat4[ skeleton->m_boneCount ];
			

			skeleton->m_parentIndex = new int[ skeleton->m_boneCount ];

			for ( i = 0 ; i < skeleton->m_boneCount ; ++i )
			{
				skeleton->m_nodes[ i ]		= m_importAssistor->bones[ i ];
				skeleton->m_bones[ i ]		= skeleton->m_nodes[ i ]->m_localTransform;
				skeleton->m_bindPoses[ i ]	= m_importAssistor->bindPoses[i];
			}
			for ( i = 0 ; i < skeleton->m_boneCount ; ++i )
			{
				skeleton->m_parentIndex[i] = -1;
				for ( int j = 0 ; j < (int)skeleton->m_boneCount ; ++j )
				{
					if (skeleton->m_nodes[i]->m_parent == skeleton->m_nodes[j])
					{
						skeleton->m_parentIndex[i] = j;
						break;
					}
				}
			}

			extractSkeleton(skeleton, lScene);

			m_skeletons.push_back(skeleton);

			extractAnimation(lScene);
		}

		m_root->updateGlobalTransform();

		delete m_importAssistor;
		m_importAssistor = nullptr;
	}

	lSdkManager->Destroy();

	return true;
}

bool FBXFile::loadAnimationsOnly(const char* a_filename, UNIT_SCALE a_scale /* = FBXFile::UNITS_METER */)
{
	if (m_root != nullptr)
	{
		printf("Scene already loaded!\n");
		return false;
	}

	FbxManager* lSdkManager = nullptr;
	FbxScene* lScene = nullptr;

	// The first thing to do is to create the FBX SDK manager which is the 
	// object allocator for almost all the classes in the SDK.
	lSdkManager = FbxManager::Create();
	if ( !lSdkManager )
	{
		printf("Unable to create the FBX SDK manager\n");
		return false;
	}

	// create an IOSettings object
	FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT );
	lSdkManager->SetIOSettings(ios);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"");

	// Initialize the importer by providing a filename.
	bool lImportStatus = lImporter->Initialize(a_filename, -1, lSdkManager->GetIOSettings());

	if ( !lImportStatus )
	{
		printf("Call to FbxImporter::Initialize() failed:\n\t%s\n", lImporter->GetStatus().GetErrorString());
		lImporter->Destroy();
		return false;
	}

	// Create the entity that will hold the scene.
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor,  lSDKMinor,  lSDKRevision;

	unsigned int i;
	bool lStatus;

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	lScene = FbxScene::Create(lSdkManager,"root");

	// Import the scene.
	lStatus = lImporter->Import(lScene);
	lImporter->Destroy();
	if (lStatus == false)
	{
		printf("Unable to open FBX file!\n");
		return false;
	}

	float unitScale = 1;

	// convert scale
	if ( lScene->GetGlobalSettings().GetSystemUnit() != FbxSystemUnit::sPredefinedUnits[a_scale] )
	{
		const FbxSystemUnit::ConversionOptions lConversionOptions = {
			false, // mConvertRrsNodes
			true, // mConvertAllLimits
			true, // mConvertClusters
			true, // mConvertLightIntensity
			true, // mConvertPhotometricLProperties
			true  // mConvertCameraClipPlanes
		};

		unitScale = (float)(lScene->GetGlobalSettings().GetSystemUnit().GetScaleFactor() / FbxSystemUnit::sPredefinedUnits[a_scale].GetScaleFactor());

		// Convert the scene to meters using the defined options.
		FbxSystemUnit::sPredefinedUnits[a_scale].ConvertScene(lScene, lConversionOptions);
	}

	// convert the scene to OpenGL axis (right-handed Y up)
	FbxAxisSystem::OpenGL.ConvertScene(lScene);

	FbxNode* lNode = lScene->GetRootNode();

	if (lNode != nullptr)
	{
		// store the folder path of the scene
		m_path = a_filename;
		long iLastForward = m_path.find_last_of('/');
		long iLastBackward = m_path.find_last_of('\\');
		if (iLastForward > iLastBackward)
		{
			m_path.resize(iLastForward + 1);
		}
		else if (iLastBackward != 0)
		{
			m_path.resize(iLastBackward + 1);
		}
		else
		{
			m_path = "";
		}

		m_importAssistor = new ImportAssistor();

		m_importAssistor->scene = lScene;
		m_importAssistor->evaluator = lScene->GetAnimationEvaluator();
		m_importAssistor->importer = lImporter;
		m_importAssistor->loadTextures = false;
		m_importAssistor->loadAnimations = true;
		m_importAssistor->loadAnimationOnly = true;
		m_importAssistor->unitScale = unitScale;//a_scale;

		m_root = new FBXNode();
		strcpy(m_root->m_name,"root");
		m_root->m_globalTransform = m_root->m_localTransform = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

		
		// extract children
		for (i = 0; i < (unsigned int)lNode->GetChildCount(); ++i)
		{
			extractObject(m_root, (void*)lNode->GetChild(i));
		}
		
		if (m_importAssistor->bones.size() > 0)
		{
			FBXSkeleton* skeleton = new FBXSkeleton();
			skeleton->m_boneCount = (unsigned int)m_importAssistor->bones.size();
			skeleton->m_nodes = new FBXNode * [ skeleton->m_boneCount ];
			skeleton->m_bones = new glm::mat4[ skeleton->m_boneCount ];
			skeleton->m_bindPoses = new glm::mat4[ skeleton->m_boneCount ];
			skeleton->m_parentIndex = new int[ skeleton->m_boneCount ];

			for ( i = 0 ; i < skeleton->m_boneCount ; ++i )
			{
				skeleton->m_nodes[ i ] = m_importAssistor->bones[ i ];
				skeleton->m_bones[ i ] = skeleton->m_nodes[ i ]->m_localTransform;
				skeleton->m_bindPoses[i] = m_importAssistor->bindPoses[i];
			}
			for ( i = 0 ; i < skeleton->m_boneCount ; ++i )
			{
				skeleton->m_parentIndex[i] = -1;
				for ( int j = 0 ; j < (int)skeleton->m_boneCount ; ++j )
				{
					if (skeleton->m_nodes[i]->m_parent == skeleton->m_nodes[j])
					{
						skeleton->m_parentIndex[i] = j;
						break;
					}
				}
			}

			extractSkeleton(skeleton, lScene);

			m_skeletons.push_back(skeleton);

			extractAnimation(lScene);
		}

		m_root->updateGlobalTransform();

		delete m_importAssistor;
		m_importAssistor = nullptr;
	}

	lSdkManager->Destroy();

	return true;
}

void FBXFile::extractObject(FBXNode* a_parent, void* a_object)
{
	FbxNode* fbxNode = (FbxNode*)a_object;

	FBXNode* node = nullptr;

	FbxNodeAttribute::EType lAttributeType;
	int i;

	bool bIsBone = false;

	if (fbxNode->GetNodeAttribute() != nullptr)
	{
		lAttributeType = (fbxNode->GetNodeAttribute()->GetAttributeType());

		switch (lAttributeType)
		{
		case FbxNodeAttribute::eSkeleton:	
			{
				bIsBone = true;
				unsigned int index = (unsigned int)m_importAssistor->boneIndexList.size();
				char name[MAX_PATH];
				strncpy(name, fbxNode->GetName(), MAX_PATH);
				m_importAssistor->boneIndexList[name] = index;
				m_importAssistor->bindPoses.push_back(glm::mat4());
			}
			break;

		case FbxNodeAttribute::eMesh:      
			{
				if (m_importAssistor->loadAnimationOnly == false)
					node = (FBXNode*)extractMeshes(fbxNode);
			}
			break;

		case FbxNodeAttribute::eCamera:   
			{
				if (m_importAssistor->loadAnimationOnly == false)
				{
					node = new FBXCameraNode();
					extractCamera((FBXCameraNode*)node,fbxNode);

					if (strlen(fbxNode->GetName()) > 0)
						strncpy(node->m_name,fbxNode->GetName(),MAX_PATH-1);

					m_cameras[node->m_name] = (FBXCameraNode*)node;
				}
			}
			break;

		case FbxNodeAttribute::eLight:   
			{
				if (m_importAssistor->loadAnimationOnly == false)
				{
					node = new FBXLightNode();
					extractLight((FBXLightNode*)node,fbxNode);

					if (strlen(fbxNode->GetName()) > 0)
						strncpy(node->m_name,fbxNode->GetName(),MAX_PATH-1);

					m_lights[node->m_name] = (FBXLightNode*)node;
				}
			}
			break;

		default: break;
		}   
	}
	

	// if null then use it as a plain 3D node
	if (node == nullptr)
	{
		node = new FBXNode();
		if (strlen(fbxNode->GetName()) > 0)
			strncpy(node->m_name,fbxNode->GetName(),MAX_PATH-1);
	}

	// add to parent's children and update parent
	a_parent->m_children.push_back(node);
	node->m_parent = a_parent;

	// build local transform
	// use anim evaluator as bones store transforms in a different spot
	FbxAMatrix lLocal = m_importAssistor->evaluator->GetNodeLocalTransform(fbxNode);

	node->m_localTransform = glm::mat4( lLocal[0][0], lLocal[0][1], lLocal[0][2], lLocal[0][3],
										lLocal[1][0], lLocal[1][1], lLocal[1][2], lLocal[1][3],
										lLocal[2][0], lLocal[2][1], lLocal[2][2], lLocal[2][3],
										lLocal[3][0], lLocal[3][1], lLocal[3][2], lLocal[3][3] );

	if (m_importAssistor->loadAnimations == true &&
		bIsBone == true)
	{
		m_importAssistor->bones.push_back(node);
	}

	// children
	for (i = 0; i < fbxNode->GetChildCount(); i++)
	{
		extractObject(node, (void*)fbxNode->GetChild(i));
	}
}

void* FBXFile::extractMeshes(void* a_object)
{
	FbxNode* fbxNode = (FbxNode*)a_object;
	FbxMesh* fbxMesh = (FbxMesh*)fbxNode->GetNodeAttribute();

	int i, j, k, l, lPolygonCount = fbxMesh->GetPolygonCount();
	FbxVector4* lControlPoints = fbxMesh->GetControlPoints(); 
	FbxGeometryElementMaterial* lMaterialElement = fbxMesh->GetElementMaterial(0);

	int materialCount = fbxNode->GetMaterialCount();

	FBXMeshNode** meshes = new FBXMeshNode * [ materialCount ];
	for ( j = 0 ; j < materialCount ; ++j )
		meshes[j] = new FBXMeshNode();

	unsigned int vertexIndex[4] = {};

	unsigned int* nextIndex = new unsigned int[ materialCount ];
	memset(nextIndex, 0, sizeof(unsigned int) * materialCount);
	
	std::map<FBXVertex,unsigned int>* vertexIndexMap = new std::map<FBXVertex,unsigned int>[ materialCount ];

	FbxGeometryElementVertexColor* fbxColours = fbxMesh->GetElementVertexColor(0);
	FbxGeometryElementUV* fbxTexCoord0 = fbxMesh->GetElementUV(0);
	FbxGeometryElementUV* fbxTexCoord1 = fbxMesh->GetElementUV(1);
	FbxGeometryElementNormal* fbxNormal = fbxMesh->GetElementNormal(0);
	FbxGeometryElementTangent* fbxTangent = fbxMesh->GetElementTangent(0);
	FbxGeometryElementBinormal* fbxBiNormal = fbxMesh->GetElementBinormal(0);

	// gather skinning info
	FbxSkin* fbxSkin = (FbxSkin*)fbxMesh->GetDeformer(0, FbxDeformer::eSkin);
	int skinClusterCount = fbxSkin != nullptr ? fbxSkin->GetClusterCount() : 0;
	FbxCluster** skinClusters = nullptr;
	int* skinClusterBoneIndices = nullptr;
	char linkName[MAX_PATH];	
	if (skinClusterCount > 0)
	{
		skinClusters = new FbxCluster * [ skinClusterCount ];
		skinClusterBoneIndices = new int[ skinClusterCount ];
		
		for (i = 0 ; i < skinClusterCount ; ++i)
		{
			skinClusters[i] = fbxSkin->GetCluster(i);
			if (skinClusters[i]->GetLink() == nullptr)
			{
				skinClusterBoneIndices[i] = -1;
			}
			else
			{
				strncpy(linkName,skinClusters[i]->GetLink()->GetName(),MAX_PATH);
				unsigned int boneIndex = m_importAssistor->boneIndexList[linkName];
				skinClusterBoneIndices[i] = boneIndex;
				FbxAMatrix bindPoseMatrix;
				FbxAMatrix transformMatrix;
				
				
				skinClusters[i]->GetTransformMatrix(transformMatrix);
				skinClusters[i]->GetTransformLinkMatrix(bindPoseMatrix);
				
				FbxNode* bone = skinClusters[i]->GetLink();
				FbxVector4 lT = bone->GetGeometricTranslation(FbxNode::eSourcePivot);
				FbxVector4 lR = bone->GetGeometricRotation(FbxNode::eSourcePivot);
				FbxVector4 lS = bone->GetGeometricScaling(FbxNode::eSourcePivot);
				FbxAMatrix matGeom = FbxAMatrix(lT, lR, lS);
				transformMatrix *= matGeom;

				bindPoseMatrix = bindPoseMatrix.Inverse() * transformMatrix;
				
				FbxVector4 row0 = bindPoseMatrix.GetRow(0);
				FbxVector4 row1 = bindPoseMatrix.GetRow(1);
				FbxVector4 row2 = bindPoseMatrix.GetRow(2);
				FbxVector4 row3 = bindPoseMatrix.GetRow(3);
				
				glm::mat4 bindP = glm::mat4(
					(float)row0[0], (float)row0[1], (float)row0[2], (float)row0[3],
					(float)row1[0], (float)row1[1], (float)row1[2], (float)row1[3],
					(float)row2[0], (float)row2[1], (float)row2[2], (float)row2[3],
					(float)row3[0], (float)row3[1], (float)row3[2], (float)row3[3]);

				m_importAssistor->bindPoses[boneIndex] = bindP;
			}
		}
	}

	unsigned int vertexAttributes = 0;

	int vertexId = 0;

	// process each polygon (tris and quads only)
	for (i = 0; i < lPolygonCount; ++i)
	{
		int polygonSize = fbxMesh->GetPolygonSize(i);

		int material = lMaterialElement->GetIndexArray().GetAt(i);
		
		for (j = 0; j < polygonSize && j < 4 ; ++j)
		{
			FBXVertex vertex;

			int controlPointIndex = fbxMesh->GetPolygonVertex(i, j);

			FbxVector4 vPos = lControlPoints[controlPointIndex];
			vertex.position.x = (float)vPos[0];
			vertex.position.y = (float)vPos[1];
			vertex.position.z = (float)vPos[2];
			vertex.position.w = 1;
			
			vertexAttributes |= FBXVertex::POSITION;
			
			// extract colour data
			if (fbxColours != nullptr)
			{
				int id = -1;
				switch (fbxColours->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
					switch (fbxColours->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{						
							id = controlPointIndex;
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							id = fbxColours->GetIndexArray().GetAt(controlPointIndex);
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
					{
						switch (fbxColours->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							{							
								id = vertexId;
							}
							break;
						case FbxGeometryElement::eIndexToDirect:
							{
								id = fbxColours->GetIndexArray().GetAt(vertexId);
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
					default:    break;
				}
				if (id != -1)
				{
					FbxColor colour = fbxColours->GetDirectArray().GetAt(id);
					vertexAttributes |= FBXVertex::COLOUR;

					vertex.colour.x = (float)colour.mRed;
					vertex.colour.y = (float)colour.mGreen;
					vertex.colour.z = (float)colour.mBlue;
					vertex.colour.w = (float)colour.mAlpha;
				}
			}
			// extract first texture coordinate set
			if (fbxTexCoord0 != nullptr)
			{
				switch (fbxTexCoord0->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
					switch (fbxTexCoord0->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector2 uv = fbxTexCoord0->GetDirectArray().GetAt(controlPointIndex);
							vertexAttributes |= FBXVertex::TEXCOORD1;

							vertex.texCoord1.x = (float)uv[0];
							vertex.texCoord1.y = (float)uv[1];
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = fbxTexCoord0->GetIndexArray().GetAt(controlPointIndex);
							FbxVector2 uv = fbxTexCoord0->GetDirectArray().GetAt(id);
							vertexAttributes |= FBXVertex::TEXCOORD1;

							vertex.texCoord1.x = (float)uv[0];
							vertex.texCoord1.y = (float)uv[1];
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
					{
						int lTextureUVIndex = fbxMesh->GetTextureUVIndex(i, j);
						switch (fbxTexCoord0->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
						case FbxGeometryElement::eIndexToDirect:
							{
								FbxVector2 uv = fbxTexCoord0->GetDirectArray().GetAt(lTextureUVIndex);
								vertexAttributes |= FBXVertex::TEXCOORD1;

								vertex.texCoord1.x = (float)uv[0];
								vertex.texCoord1.y = (float)uv[1];
							}								
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
				default: break;
				}
			}

			// extract second coordinate set
			if (fbxTexCoord1 != nullptr)
			{
				switch (fbxTexCoord1->GetMappingMode())
				{
				case FbxGeometryElement::eByControlPoint:
					switch (fbxTexCoord1->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector2 uv = fbxTexCoord1->GetDirectArray().GetAt(controlPointIndex);
							vertexAttributes |= FBXVertex::TEXCOORD2;

							vertex.texCoord2.x = (float)uv[0];
							vertex.texCoord2.y = (float)uv[1];
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = fbxTexCoord1->GetIndexArray().GetAt(controlPointIndex);
							FbxVector2 uv = fbxTexCoord1->GetDirectArray().GetAt(id);
							vertexAttributes |= FBXVertex::TEXCOORD2;

							vertex.texCoord2.x = (float)uv[0];
							vertex.texCoord2.y = (float)uv[1];
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
					break;

				case FbxGeometryElement::eByPolygonVertex:
					{
						int lTextureUVIndex = fbxMesh->GetTextureUVIndex(i, j);
						switch (fbxTexCoord1->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
						case FbxGeometryElement::eIndexToDirect:
							{
								FbxVector2 uv = fbxTexCoord1->GetDirectArray().GetAt(lTextureUVIndex);
								vertexAttributes |= FBXVertex::TEXCOORD2;

								vertex.texCoord2.x = (float)uv[0];
								vertex.texCoord2.y = (float)uv[1];
							}								
							break;
						default:
							break; // other reference modes not shown here!
						}
					}
					break;

				case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
				case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
				case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
					break;
					default:    break;
				}
			}

			// extract normal data
			if (fbxNormal != nullptr)
			{
				if (fbxNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					switch (fbxNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector4 normal = fbxNormal->GetDirectArray().GetAt(controlPointIndex);
							vertexAttributes |= FBXVertex::NORMAL;

							vertex.normal.x = (float)normal[0];
							vertex.normal.y = (float)normal[1];
							vertex.normal.z = (float)normal[2];
							vertex.normal.w = 0;
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = fbxNormal->GetIndexArray().GetAt(controlPointIndex);
							FbxVector4 normal = fbxNormal->GetDirectArray().GetAt(id);
							vertexAttributes |= FBXVertex::NORMAL;

							vertex.normal.x = (float)normal[0];
							vertex.normal.y = (float)normal[1];
							vertex.normal.z = (float)normal[2];
							vertex.normal.w = 0;
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
				else if(fbxNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (fbxNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector4 normal = fbxNormal->GetDirectArray().GetAt(vertexId);
							vertexAttributes |= FBXVertex::NORMAL;

							vertex.normal.x = (float)normal[0];
							vertex.normal.y = (float)normal[1];
							vertex.normal.z = (float)normal[2];
							vertex.normal.w = 0;
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = fbxNormal->GetIndexArray().GetAt(vertexId);
							FbxVector4 normal = fbxNormal->GetDirectArray().GetAt(id);
							vertexAttributes |= FBXVertex::NORMAL;

							vertex.normal.x = (float)normal[0];
							vertex.normal.y = (float)normal[1];
							vertex.normal.z = (float)normal[2];
							vertex.normal.w = 0;
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}

			// extract tangent data (if it exists)
			if (fbxTangent != nullptr)
			{
				if (fbxTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					switch (fbxTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector4 normal = fbxTangent->GetDirectArray().GetAt(controlPointIndex);
							vertexAttributes |= FBXVertex::TANGENT;

							vertex.tangent.x = (float)normal[0];
							vertex.tangent.y = (float)normal[1];
							vertex.tangent.z = (float)normal[2];
							vertex.tangent.w = 0;
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = fbxTangent->GetIndexArray().GetAt(controlPointIndex);
							FbxVector4 normal = fbxTangent->GetDirectArray().GetAt(id);
							vertexAttributes |= FBXVertex::TANGENT;

							vertex.tangent.x = (float)normal[0];
							vertex.tangent.y = (float)normal[1];
							vertex.tangent.z = (float)normal[2];
							vertex.tangent.w = 0;
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
				else if(fbxTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (fbxTangent->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector4 normal = fbxTangent->GetDirectArray().GetAt(vertexId);
							vertexAttributes |= FBXVertex::TANGENT;

							vertex.tangent.x = (float)normal[0];
							vertex.tangent.y = (float)normal[1];
							vertex.tangent.z = (float)normal[2];
							vertex.tangent.w = 0;
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = fbxTangent->GetIndexArray().GetAt(vertexId);
							FbxVector4 normal = fbxTangent->GetDirectArray().GetAt(id);
							vertexAttributes |= FBXVertex::TANGENT;

							vertex.tangent.x = (float)normal[0];
							vertex.tangent.y = (float)normal[1];
							vertex.tangent.z = (float)normal[2];
							vertex.tangent.w = 0;
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}

			// extract binormal data if it exists
			if (fbxBiNormal != nullptr)
			{
				if (fbxBiNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
				{
					switch (fbxBiNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector4 normal = fbxBiNormal->GetDirectArray().GetAt(controlPointIndex);
							vertexAttributes |= FBXVertex::BINORMAL;

							vertex.binormal.x = (float)normal[0];
							vertex.binormal.y = (float)normal[1];
							vertex.binormal.z = (float)normal[2];
							vertex.binormal.w = 0;
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = fbxBiNormal->GetIndexArray().GetAt(controlPointIndex);
							FbxVector4 normal = fbxBiNormal->GetDirectArray().GetAt(id);
							vertexAttributes |= FBXVertex::BINORMAL;

							vertex.binormal.x = (float)normal[0];
							vertex.binormal.y = (float)normal[1];
							vertex.binormal.z = (float)normal[2];
							vertex.binormal.w = 0;
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
				else if(fbxBiNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
				{
					switch (fbxBiNormal->GetReferenceMode())
					{
					case FbxGeometryElement::eDirect:
						{
							FbxVector4 normal = fbxBiNormal->GetDirectArray().GetAt(vertexId);
							vertexAttributes |= FBXVertex::BINORMAL;

							vertex.binormal.x = (float)normal[0];
							vertex.binormal.y = (float)normal[1];
							vertex.binormal.z = (float)normal[2];
							vertex.binormal.w = 0;
						}
						break;
					case FbxGeometryElement::eIndexToDirect:
						{
							int id = fbxBiNormal->GetIndexArray().GetAt(vertexId);
							FbxVector4 normal = fbxBiNormal->GetDirectArray().GetAt(id);
							vertexAttributes |= FBXVertex::BINORMAL;

							vertex.binormal.x = (float)normal[0];
							vertex.binormal.y = (float)normal[1];
							vertex.binormal.z = (float)normal[2];
							vertex.binormal.w = 0;
						}
						break;
					default:
						break; // other reference modes not shown here!
					}
				}
			}

			// gather skinning data (slow but can't find any other way, yet!)
			if (fbxSkin != nullptr)
			{
				for (k = 0; k != skinClusterCount; ++k)
				{
					if (skinClusterBoneIndices[k] == -1)
						continue;

					int lIndexCount = skinClusters[k]->GetControlPointIndicesCount();
					int* lIndices = skinClusters[k]->GetControlPointIndices();
					double* lWeights = skinClusters[k]->GetControlPointWeights();

					for (l = 0; l < lIndexCount; l++)
					{
						if (controlPointIndex == lIndices[l])
						{
							// add weight and index
							if (vertex.weights.x == 0)
							{
								vertex.weights.x = (float)lWeights[l];
								vertex.indices.x = (float)skinClusterBoneIndices[k];
							}
							else if (vertex.weights.y == 0)
							{
								vertex.weights.y = (float)lWeights[l];
								vertex.indices.y = (float)skinClusterBoneIndices[k];
							}
							else if (vertex.weights.z == 0)
							{
								vertex.weights.z = (float)lWeights[l];
								vertex.indices.z = (float)skinClusterBoneIndices[k];
							}
							else
							{
								vertex.weights.w = (float)lWeights[l];
								vertex.indices.w = (float)skinClusterBoneIndices[k];
							}
							break;
						}
					}
				}
			}

			// either add a new unique vertex or use an existing one
			auto iter = vertexIndexMap[material].find( vertex );
			if (iter == vertexIndexMap[material].end())
			{
				// add new unique vertex
				meshes[material]->m_vertices.push_back(vertex);
				vertexIndexMap[material][ vertex ] = nextIndex[material]++;
				vertexIndex[j] = nextIndex[material] - 1;
			}
			else
			{
				vertexIndex[j] = iter->second;
			}
			vertexId++;
		}

		// add triangle indices
		meshes[material]->m_indices.push_back(vertexIndex[0]);
		meshes[material]->m_indices.push_back(vertexIndex[1]);
		meshes[material]->m_indices.push_back(vertexIndex[2]);

		// handle quads
		if (polygonSize == 4)
		{
			meshes[material]->m_indices.push_back(vertexIndex[0]);
			meshes[material]->m_indices.push_back(vertexIndex[2]);
			meshes[material]->m_indices.push_back(vertexIndex[3]);
		}
	}
	
	// always build tangents/binormals as long as we have texture coordinates (saves ticking the box in maya)
	if ((vertexAttributes & FBXVertex::TEXCOORD1) != 0)
		vertexAttributes |= FBXVertex::TANGENT|FBXVertex::BINORMAL;

	// reclalc because I don't trust maya!
	if ((vertexAttributes & (FBXVertex::TANGENT|FBXVertex::BINORMAL)) != 0)
	{
		for ( j = 0 ; j < materialCount ; ++j )
			calculateTangentsBinormals(meshes[j]->m_vertices,meshes[j]->m_indices);
	}

	// set mesh names, vertex attributes, extract material and add to mesh map
	for ( j = 0 ; j < materialCount ; ++j )
	{
		std::string name = fbxNode->GetName();

		// append material name to mesh name
		if (materialCount > 1)
			name += fbxNode->GetMaterial(j)->GetName();	

		if (strlen(fbxNode->GetName()) > 0)
			strncpy(meshes[j]->m_name,name.c_str(),MAX_PATH-1);		

		meshes[j]->m_vertexAttributes = vertexAttributes;

		meshes[j]->m_material = extractMaterial(fbxMesh,j);
		
		m_meshes[meshes[j]->m_name] = meshes[j];
	}

	FBXNode* node = nullptr;

	// if there is a single mesh return it, else make a new parent node and return that
	if (materialCount > 1)
	{
		node = new FBXNode();

		if (strlen(fbxNode->GetName()) > 0)
			strncpy(node->m_name,fbxNode->GetName(),MAX_PATH-1);

		for ( j = 0 ; j < materialCount ; ++j )
		{
			node->m_children.push_back( meshes[j] );
			meshes[j]->m_parent = node;
		}
	}
	else
	{
		node = meshes[0];
	}

	delete[] skinClusters;
	delete[] skinClusterBoneIndices;

	delete[] meshes;
	delete[] nextIndex;
	delete[] vertexIndexMap;
	
	return node;
}

void FBXFile::extractLight(FBXLightNode* a_light, void* a_object)
{
	FbxNode* fbxNode = (FbxNode*)a_object;
	FbxLight* fbxLight = (FbxLight*)fbxNode->GetNodeAttribute();

	// get type, if on, and colour
	a_light->m_type = (FBXLightNode::LightType)fbxLight->LightType.Get();
	a_light->m_on = fbxLight->CastLight.Get();
	a_light->m_colour.x = (float)fbxLight->Color.Get()[0];
	a_light->m_colour.y = (float)fbxLight->Color.Get()[1];
	a_light->m_colour.z = (float)fbxLight->Color.Get()[2];
	a_light->m_colour.w = (float)fbxLight->Intensity.Get();

	// get spot light angles (will return data even for non-spotlights)
	a_light->m_innerAngle = (float)fbxLight->InnerAngle.Get() * (glm::pi<float>() / 180);
	a_light->m_outerAngle = (float)fbxLight->OuterAngle.Get() * (glm::pi<float>() / 180);

	// get falloff data (none,linear, quadratic), cubic is ignored
	switch (fbxLight->DecayType.Get())
	{
	case 0:
		a_light->m_attenuation = glm::vec4(1,0,0,0);
		break;
	case 1:
		break;
		a_light->m_attenuation = glm::vec4(0,1,0,0);
	case 2:
		break;
		a_light->m_attenuation = glm::vec4(0,0,1,0);
	default:
		break;
	};
}

void FBXFile::extractCamera(FBXCameraNode* a_camera, void* a_object)
{
	FbxNode* fbxNode = (FbxNode*)a_object;
	FbxCamera* fbxCamera = (FbxCamera*)fbxNode->GetNodeAttribute();

	// get field of view
	if (fbxCamera->ProjectionType.Get() != FbxCamera::eOrthogonal)
	{
		a_camera->m_fieldOfView = (float)fbxCamera->FieldOfView.Get() * (glm::pi<float>() / 180);
	}
	else
	{
		a_camera->m_fieldOfView = 0;
	}

	// get aspect ratio if one was defined
	if (fbxCamera->GetAspectRatioMode() != FbxCamera::eWindowSize)
	{
		a_camera->m_aspectRatio = (float)fbxCamera->AspectWidth.Get() / (float)fbxCamera->AspectHeight.Get();
	}
	else
	{
		a_camera->m_aspectRatio = 0;
	}

	// get near/far
	a_camera->m_near = (float)fbxCamera->NearPlane.Get();
	a_camera->m_far = (float)fbxCamera->FarPlane.Get();

	// build view matrix
	glm::vec3 vEye, vTo, vUp;

	vEye.x = (float)fbxCamera->Position.Get()[0];
	vEye.y = (float)fbxCamera->Position.Get()[1];
	vEye.z = (float)fbxCamera->Position.Get()[2];

	if (fbxNode->GetTarget() != nullptr)
	{
		vTo.x = (float)fbxNode->GetTarget()->LclTranslation.Get()[0];
		vTo.y = (float)fbxNode->GetTarget()->LclTranslation.Get()[1];
		vTo.z = (float)fbxNode->GetTarget()->LclTranslation.Get()[2];
	}
	else
	{
		vTo.x = (float)fbxCamera->InterestPosition.Get()[0];
		vTo.y = (float)fbxCamera->InterestPosition.Get()[1];
		vTo.z = (float)fbxCamera->InterestPosition.Get()[2];
	}

	if (fbxNode->GetTargetUp())
	{
		vUp.x = (float)fbxNode->GetTargetUp()->LclTranslation.Get()[0];
		vUp.y = (float)fbxNode->GetTargetUp()->LclTranslation.Get()[1];
		vUp.z = (float)fbxNode->GetTargetUp()->LclTranslation.Get()[2];

	}
	else
	{
		vUp.x = (float)fbxCamera->UpVector.Get()[0];
		vUp.y = (float)fbxCamera->UpVector.Get()[1];
		vUp.z = (float)fbxCamera->UpVector.Get()[2];
	}

	a_camera->m_viewMatrix = glm::lookAt(vEye,vTo,vUp);
}

FBXMaterial* FBXFile::extractMaterial(void* a_mesh, int a_materialIndex)
{
	FbxGeometry* pGeometry = (FbxGeometry*)a_mesh;
	FbxNode* lNode = pGeometry->GetNode();
	FbxSurfaceMaterial *lMaterial = lNode->GetMaterial(a_materialIndex);

	char matName[MAX_PATH];
	strncpy(matName,lMaterial->GetName(),MAX_PATH-1);

	// check if material already loaded, else create new material
	auto oIter = m_materials.find(matName);
	if (oIter != m_materials.end())
	{
		return oIter->second;
	}
	else
	{
		FBXMaterial* material = new FBXMaterial;
		memcpy(material->name,matName,MAX_PATH);

		// get the implementation to see if it's a hardware shader.
		const FbxImplementation* lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_HLSL);
		if (lImplementation == nullptr)
		{
			lImplementation = GetImplementation(lMaterial, FBXSDK_IMPLEMENTATION_CGFX);
		}
		if (lImplementation != nullptr)
		{
			FbxBindingTable const* lRootTable = lImplementation->GetRootTable();
			FbxString lFileName = lRootTable->DescAbsoluteURL.Get();
			FbxString lTechniqueName = lRootTable->DescTAG.Get(); 

			printf("Unsupported hardware shader material!\n");
			printf("\tFile: %s\n",lFileName.Buffer());
			printf("\tTechnique: %s\n\n",lTechniqueName.Buffer());
		}
		else if (lMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			// We found a Phong material
			FbxSurfacePhong* pPhong = (FbxSurfacePhong*)lMaterial;

			material->ambient.x = (float)pPhong->Ambient.Get()[0];
			material->ambient.y = (float)pPhong->Ambient.Get()[1];
			material->ambient.z = (float)pPhong->Ambient.Get()[2];
			material->ambient.w = (float)pPhong->AmbientFactor.Get();

			material->diffuse.x = (float)pPhong->Diffuse.Get()[0];
			material->diffuse.y = (float)pPhong->Diffuse.Get()[1];
			material->diffuse.z = (float)pPhong->Diffuse.Get()[2];
			material->diffuse.w = (float)pPhong->TransparencyFactor.Get();

			material->specular.x = (float)pPhong->Specular.Get()[0];
			material->specular.y = (float)pPhong->Specular.Get()[1];
			material->specular.z = (float)pPhong->Specular.Get()[2];
			material->specular.w = (float)pPhong->Shininess.Get();

			material->emissive.x = (float)pPhong->Emissive.Get()[0];
			material->emissive.y = (float)pPhong->Emissive.Get()[1];
			material->emissive.z = (float)pPhong->Emissive.Get()[2];
			material->emissive.w = (float)pPhong->EmissiveFactor.Get();
		}
		else if(lMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId) )
		{
			FbxSurfaceLambert* pLambert = (FbxSurfaceLambert*)lMaterial;

			material->ambient.x = (float)pLambert->Ambient.Get()[0];
			material->ambient.y = (float)pLambert->Ambient.Get()[1];
			material->ambient.z = (float)pLambert->Ambient.Get()[2];
			material->ambient.w = (float)pLambert->AmbientFactor.Get();

			material->diffuse.x = (float)pLambert->Diffuse.Get()[0];
			material->diffuse.y = (float)pLambert->Diffuse.Get()[1];
			material->diffuse.z = (float)pLambert->Diffuse.Get()[2];
			material->diffuse.w = (float)pLambert->TransparencyFactor.Get();

			// No specular in lambert materials
			material->specular.x = 0;
			material->specular.y = 0;
			material->specular.z = 0;
			material->specular.w = 0;

			material->emissive.x = (float)pLambert->Emissive.Get()[0];
			material->emissive.y = (float)pLambert->Emissive.Get()[1];
			material->emissive.z = (float)pLambert->Emissive.Get()[2];
			material->emissive.w = (float)pLambert->EmissiveFactor.Get();
		}
		else
		{
			printf("Unknown type of Material: %s\n", lMaterial->GetClassId().GetName());
		}

		// still not sure if I should bother with these
			/*DisplayDouble("            Scale U: ", pTexture->GetScaleU());
			DisplayDouble("            Scale V: ", pTexture->GetScaleV());
			DisplayDouble("            Translation U: ", pTexture->GetTranslationU());
			DisplayDouble("            Translation V: ", pTexture->GetTranslationV());
			DisplayBool("            Swap UV: ", pTexture->GetSwapUV());
			DisplayDouble("            Rotation U: ", pTexture->GetRotationU());
			DisplayDouble("            Rotation V: ", pTexture->GetRotationV());
			DisplayDouble("            Rotation W: ", pTexture->GetRotationW()); */  

		unsigned int auiTextureLookup[] =
		{
			FbxLayerElement::eTextureDiffuse - FbxLayerElement::sTypeTextureStartIndex,
			FbxLayerElement::eTextureAmbient - FbxLayerElement::sTypeTextureStartIndex,
			FbxLayerElement::eTextureEmissive - FbxLayerElement::sTypeTextureStartIndex,
			FbxLayerElement::eTextureSpecular - FbxLayerElement::sTypeTextureStartIndex,
			FbxLayerElement::eTextureShininess - FbxLayerElement::sTypeTextureStartIndex,
			FbxLayerElement::eTextureNormalMap - FbxLayerElement::sTypeTextureStartIndex,
			FbxLayerElement::eTextureTransparency - FbxLayerElement::sTypeTextureStartIndex,
			FbxLayerElement::eTextureDisplacement - FbxLayerElement::sTypeTextureStartIndex,
		};

		if (m_importAssistor->loadTextures == true)
		{
			for ( unsigned int i = 0 ; i < FBXMaterial::TextureTypes_Count ; ++i )
			{
				FbxProperty pProperty = lMaterial->FindProperty(FbxLayerElement::sTextureChannelNames[auiTextureLookup[i]]);						
				if ( pProperty.IsValid() &&
					pProperty.GetSrcObjectCount<FbxTexture>() > 0)
				{
					FbxFileTexture* fileTexture = FbxCast<FbxFileTexture>(pProperty.GetSrcObject<FbxTexture>(0));
					if (fileTexture != nullptr)
					{
						const char* szLastForward = strrchr(fileTexture->GetFileName(),'/');
						const char* szLastBackward = strrchr(fileTexture->GetFileName(),'\\');
						const char* szFilename = fileTexture->GetFileName();

						material->textureRotation[i] = (float)fileTexture->GetRotationW();
						material->textureTiling[i].x = (float)fileTexture->GetScaleU();
						material->textureTiling[i].y = (float)fileTexture->GetScaleV();
						material->textureOffsets[i].x = (float)fileTexture->GetTranslationU();
						material->textureOffsets[i].y = (float)fileTexture->GetTranslationV();
						
						if (szLastForward != nullptr && szLastForward > szLastBackward)
							szFilename = szLastForward + 1;
						else if (szLastBackward != nullptr)
							szFilename = szLastBackward + 1;

						if (strlen(szFilename) >= MAX_PATH)
						{
							printf("Texture filename too long!: %s\n", szFilename);
						}
						else
						{
							strcpy(material->textureFilenames[i],szFilename);
						}

						std::string fullPath = m_path + szFilename;

						auto iter = m_textures.find(fullPath);
						if (iter != m_textures.end())
						{
							material->textureIDs[i] = iter->second;
						}
						else
						{
							material->textureIDs[i] = SOIL_load_OGL_texture(fullPath.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
								SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
							if (material->textureIDs[i] > 0)
							{
								m_textures[ fullPath ] = material->textureIDs[i];
							}
							else
							{
								printf("Failed to load texture: %s\n", fullPath.c_str());
							}
						}
					}
				}   
			}
		}

		m_materials[material->name] = material;
		return material;
	}

	return nullptr;
}

void FBXFile::extractAnimation(void* a_scene)
{
	FbxScene* fbxScene = (FbxScene*)a_scene;

	int animStackCount = fbxScene->GetSrcObjectCount<FbxAnimStack>();

	std::vector<int> tracks;
	std::vector<void*> nodes;
	tracks.reserve(128);
	nodes.reserve(128);

	FbxTime frameTime;

	for (int i = 0; i < animStackCount; i++)
	{		
		tracks.clear();
		nodes.clear();

		FbxAnimStack* lAnimStack = fbxScene->GetSrcObject<FbxAnimStack>(i);

		FBXAnimation* anim = new FBXAnimation();
		strncpy(anim->m_name,lAnimStack->GetName(),MAX_PATH);
		
		// get animated track bone indices and nodes, and calculate start/end frame
		int nbAnimLayers = lAnimStack->GetMemberCount(FbxCriteria::ObjectType(FbxAnimLayer::ClassId));
		for (int j = 0; j < nbAnimLayers; ++j)
		{
			FbxAnimLayer* lAnimLayer = lAnimStack->GetMember<FbxAnimLayer>(j);
			extractAnimationTrack(tracks, lAnimLayer, fbxScene->GetRootNode(), nodes, anim->m_startFrame, anim->m_endFrame);
		}

		// frame count (includes start/end frames)
		unsigned int frameCount = anim->m_endFrame - anim->m_startFrame + 1;

		// allocate tracks and keyframes
		anim->m_trackCount = (unsigned int)tracks.size();
		anim->m_tracks = new FBXTrack[ anim->m_trackCount ];
		for (unsigned int track = 0 ; track < anim->m_trackCount ; ++track )
		{
			anim->m_tracks[track].m_boneIndex = tracks[track];
			anim->m_tracks[track].m_keyframeCount = frameCount;
			anim->m_tracks[track].m_keyframes = new FBXKeyFrame[ frameCount ];
		}
		
		// evaluate all of the animated track keyframes
		// loop by frame first to limit FBX time changes (dreadfully slow!)
		for ( unsigned int frame = 0 ; frame < frameCount ; ++frame )
		{
			frameTime.SetFrame(frame + anim->m_startFrame);

			for (unsigned int track = 0 ; track < anim->m_trackCount ; ++track )
			{
				FbxAMatrix localMatrix = m_importAssistor->evaluator->GetNodeLocalTransform( (FbxNode*)nodes[ track ], frameTime );

				FbxQuaternion R = localMatrix.GetQ();
				FbxVector4 T = localMatrix.GetT();
				FbxVector4 S = localMatrix.GetS();

				anim->m_tracks[track].m_keyframes[ frame ].m_key = frame + anim->m_startFrame;
				anim->m_tracks[track].m_keyframes[ frame ].m_rotation = glm::quat((float)R[3],(float)R[0],(float)R[1],(float)R[2]);
				anim->m_tracks[track].m_keyframes[ frame ].m_translation = glm::vec3((float)T[0],(float)T[1],(float)T[2]);
				anim->m_tracks[track].m_keyframes[ frame ].m_scale = glm::vec3((float)S[0],(float)S[1],(float)S[2]);
			}
		}

		m_animations[ anim->m_name ] = anim;
	}
}

void FBXFile::extractAnimationTrack(std::vector<int>& a_tracks, void* a_layer, void* a_node, std::vector<void*>& a_nodes, unsigned int& a_startFrame, unsigned int& a_endFrame)
{
	FbxAnimLayer* fbxAnimLayer = (FbxAnimLayer*)a_layer;
	FbxNode* fbxNode = (FbxNode*)a_node;

	// find node index in skeleton
	int boneIndex = -1;
	FBXSkeleton* skeleton = m_skeletons[0];
	for ( unsigned int i = 0 ; i < skeleton->m_boneCount ; ++i )
	{
		if (strncmp(skeleton->m_nodes[i]->m_name,fbxNode->GetName(),MAX_PATH) == 0)
		{
			boneIndex = i;
			break;
		}
	}

	// extract bones that have animated properties on them only
	bool hasKeys = false;
	FbxAnimCurve* lAnimCurve = nullptr;
	if (boneIndex >= 0)
	{
		// translation
		lAnimCurve = fbxNode->LclTranslation.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve)
		{
			hasKeys = true;

			unsigned int frame = (unsigned int)lAnimCurve->KeyGetTime(0).GetFrameCount();
			if (frame < a_startFrame)
				a_startFrame = frame;

			frame = (unsigned int)lAnimCurve->KeyGetTime(lAnimCurve->KeyGetCount()-1).GetFrameCount();
			if (frame > a_endFrame)
				a_endFrame = frame;
		}
		lAnimCurve = fbxNode->LclTranslation.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (lAnimCurve)
		{
			hasKeys = true;

			unsigned int frame = (unsigned int)lAnimCurve->KeyGetTime(0).GetFrameCount();
			if (frame < a_startFrame)
				a_startFrame = frame;

			frame = (unsigned int)lAnimCurve->KeyGetTime(lAnimCurve->KeyGetCount()-1).GetFrameCount();
			if (frame > a_endFrame)
				a_endFrame = frame;
			
		}
		lAnimCurve = fbxNode->LclTranslation.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (lAnimCurve)
		{
			hasKeys = true;

			unsigned int frame = (unsigned int)lAnimCurve->KeyGetTime(0).GetFrameCount();
			if (frame < a_startFrame)
				a_startFrame = frame;

			frame = (unsigned int)lAnimCurve->KeyGetTime(lAnimCurve->KeyGetCount()-1).GetFrameCount();
			if (frame > a_endFrame)
				a_endFrame = frame;
			
		}

		// rotation
		lAnimCurve = fbxNode->LclRotation.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve)
		{
			hasKeys = true;

			unsigned int frame = (unsigned int)lAnimCurve->KeyGetTime(0).GetFrameCount();
			if (frame < a_startFrame)
				a_startFrame = frame;

			frame = (unsigned int)lAnimCurve->KeyGetTime(lAnimCurve->KeyGetCount()-1).GetFrameCount();
			if (frame > a_endFrame)
				a_endFrame = frame;
			
		}
		lAnimCurve = fbxNode->LclRotation.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (lAnimCurve)
		{
			hasKeys = true;

			unsigned int frame = (unsigned int)lAnimCurve->KeyGetTime(0).GetFrameCount();
			if (frame < a_startFrame)
				a_startFrame = frame;

			frame = (unsigned int)lAnimCurve->KeyGetTime(lAnimCurve->KeyGetCount()-1).GetFrameCount();
			if (frame > a_endFrame)
				a_endFrame = frame;
			
		}
		lAnimCurve = fbxNode->LclRotation.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (lAnimCurve)
		{
			hasKeys = true;

			unsigned int frame = (unsigned int)lAnimCurve->KeyGetTime(0).GetFrameCount();
			if (frame < a_startFrame)
				a_startFrame = frame;

			frame = (unsigned int)lAnimCurve->KeyGetTime(lAnimCurve->KeyGetCount()-1).GetFrameCount();
			if (frame > a_endFrame)
				a_endFrame = frame;
			
		}

		// scale
		lAnimCurve = fbxNode->LclScaling.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if (lAnimCurve)
		{
			hasKeys = true;

			unsigned int frame = (unsigned int)lAnimCurve->KeyGetTime(0).GetFrameCount();
			if (frame < a_startFrame)
				a_startFrame = frame;

			frame = (unsigned int)lAnimCurve->KeyGetTime(lAnimCurve->KeyGetCount()-1).GetFrameCount();
			if (frame > a_endFrame)
				a_endFrame = frame;
			
		}    
		lAnimCurve = fbxNode->LclScaling.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if (lAnimCurve)
		{
			hasKeys = true;

			unsigned int frame = (unsigned int)lAnimCurve->KeyGetTime(0).GetFrameCount();
			if (frame < a_startFrame)
				a_startFrame = frame;

			frame = (unsigned int)lAnimCurve->KeyGetTime(lAnimCurve->KeyGetCount()-1).GetFrameCount();
			if (frame > a_endFrame)
				a_endFrame = frame;
			
		}
		lAnimCurve = fbxNode->LclScaling.GetCurve(fbxAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if (lAnimCurve)
		{
			hasKeys = true;

			unsigned int frame = (unsigned int)lAnimCurve->KeyGetTime(0).GetFrameCount();
			if (frame < a_startFrame)
				a_startFrame = frame;

			frame = (unsigned int)lAnimCurve->KeyGetTime(lAnimCurve->KeyGetCount()-1).GetFrameCount();
			if (frame > a_endFrame)
				a_endFrame = frame;			
		}

		if (hasKeys)
		{
			a_nodes.push_back(fbxNode);
			a_tracks.push_back(boneIndex);
		}
	}

	for (int i = 0; i < fbxNode->GetChildCount(); ++i)
	{
		extractAnimationTrack(a_tracks, fbxAnimLayer, fbxNode->GetChild(i), a_nodes, a_startFrame, a_endFrame);
	}
}

void FBXFile::extractSkeleton(FBXSkeleton* a_skeleton, void* a_scene)
{
	FbxScene* fbxScene = (FbxScene*)a_scene;

	int poseCount = fbxScene->GetPoseCount();

	char name[MAX_PATH];

	for (int i = 0; i < poseCount; ++i)
	{
		FbxPose* lPose = fbxScene->GetPose(i);
		if (lPose->IsBindPose())
		{
			int rootID = lPose->Find("root");

			for (int j = 0; j < lPose->GetCount(); ++j)
			{
				strncpy(name, lPose->GetNodeName(j).GetCurrentName(), MAX_PATH);

				float scaleFactor = m_importAssistor->unitScale;

				FbxMatrix scale(scaleFactor, 0, 0, 0,
					0, scaleFactor, 0, 0,
					0, 0, scaleFactor, 0,
					0, 0, 0, 1);
				//Not all FBX Skeletons contain bind pose data there is another way to extract this data using 
				//the routines found on lines 600 - 640 of this file.
				FbxMatrix lMatrix = lPose->GetMatrix(j);
				FbxMatrix lBindMatrix = lMatrix.Inverse() * scale;
				for (unsigned int k = 0; k < a_skeleton->m_boneCount; ++k)
				{
					if (strcmp(name, a_skeleton->m_nodes[k]->m_name) == 0)
					{
						FbxVector4 row0 = lBindMatrix.GetRow(0);
						FbxVector4 row1 = lBindMatrix.GetRow(1);
						FbxVector4 row2 = lBindMatrix.GetRow(2);
						FbxVector4 row3 = lBindMatrix.GetRow(3);

						a_skeleton->m_bindPoses[k][0] = glm::vec4((float)row0[0], (float)row0[1], (float)row0[2], (float)row0[3]);
						a_skeleton->m_bindPoses[k][1] = glm::vec4((float)row1[0], (float)row1[1], (float)row1[2], (float)row1[3]);
						a_skeleton->m_bindPoses[k][2] = glm::vec4((float)row2[0], (float)row2[1], (float)row2[2], (float)row2[3]);
						a_skeleton->m_bindPoses[k][3] = glm::vec4((float)row3[0], (float)row3[1], (float)row3[2], (float)row3[3]);
					}
				}
			}
		}
	}
}

void FBXSkeleton::evaluate(const FBXAnimation* a_animation, float a_time, bool a_loop, float a_FPS)
{
	if (a_animation != nullptr)
	{
		// determine frame we're on
		int totalFrames = a_animation->m_endFrame - a_animation->m_startFrame;
		float animDuration = totalFrames / a_FPS;

		// get time through frame
		float frameTime = 0;
		if (a_loop)
			frameTime = glm::max(glm::mod(a_time,animDuration),0.0f);
		else
			frameTime = glm::min(glm::max(a_time,0.0f),animDuration);
				
		const FBXTrack* track = nullptr;
		const FBXKeyFrame* start = nullptr;
		const FBXKeyFrame* end = nullptr;

		for ( unsigned int i = 0 ; i < a_animation->m_trackCount ; ++i )
		{
			track = &(a_animation->m_tracks[i]);

			start = nullptr;
			end = nullptr;
			float startTime, endTime;

			// determine the two keyframes we're between
			for ( unsigned int j = 0 ; j < track->m_keyframeCount - 1 ; ++j )
			{
				startTime = (track->m_keyframes[j].m_key - a_animation->m_startFrame) / a_FPS;
				endTime = (track->m_keyframes[j + 1].m_key - a_animation->m_startFrame) / a_FPS;

				if (startTime <= frameTime &&
					endTime >= frameTime)
				{
					// found?
					start = &(track->m_keyframes[j]);
					end = &(track->m_keyframes[j+1]);
					break;
				}
			}

			// interpolate between them
			if (start != nullptr &&
				end != nullptr)
			{
				float fScale = glm::max(0.0f,glm::min(1.0f,(frameTime - startTime) / (endTime - startTime)));

				// translation
				glm::vec3 T = glm::mix(start->m_translation,end->m_translation,fScale);
				
				// scale
				glm::vec3 S = glm::mix(start->m_scale,end->m_scale,fScale);
				
				// rotation (quaternion slerp)
				glm::quat R = glm::normalize(glm::slerp(start->m_rotation,end->m_rotation,fScale));

				// build matrix
				glm::mat4 mRot = glm::mat4_cast( R );
				glm::mat4 mScale = glm::scale( S );
				glm::mat4 mTranslate = glm::translate( T );
				m_nodes[ track->m_boneIndex ]->m_localTransform = mTranslate * mRot;
			}
		}
	}
}

void FBXSkeleton::updateBones()
{
	// update bones
	for ( int i = 0 ; i < (int)m_boneCount ; ++i )
	{
		if ( m_parentIndex[i] == -1 )
			m_bones[ i ] = m_nodes[ i ]->m_localTransform;
		else
			m_bones[i] = m_bones[ m_parentIndex[ i ] ] * m_nodes[ i ]->m_localTransform;
	}
	// combine bind pose
	for ( int i = 0 ; i < (int)m_boneCount ; ++i )
	{
		m_bones[ i ] = m_bones[ i ] * m_bindPoses[ i ];
	}
}

void FBXFile::calculateTangentsBinormals(std::vector<FBXVertex>& a_vertices, const std::vector<unsigned int>& a_indices)
{
	unsigned int vertexCount = (unsigned int)a_vertices.size();
	glm::vec3* tan1 = new glm::vec3[vertexCount * 2];
	glm::vec3* tan2 = tan1 + vertexCount;
	memset(tan1, 0, vertexCount * sizeof(glm::vec3) * 2);

	unsigned int indexCount = (unsigned int)a_indices.size();
	for (unsigned int a = 0; a < indexCount; a += 3)
	{
		unsigned int i1 = a_indices[a];
		unsigned int i2 = a_indices[a + 1];
		unsigned int i3 = a_indices[a + 2];

		const glm::vec4& v1 = a_vertices[i1].position;
		const glm::vec4& v2 = a_vertices[i2].position;
		const glm::vec4& v3 = a_vertices[i3].position;

		const glm::vec2& w1 = a_vertices[i1].texCoord1;
		const glm::vec2& w2 = a_vertices[i2].texCoord1;
		const glm::vec2& w3 = a_vertices[i3].texCoord1;

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		float t = s1 * t2 - s2 * t1;
		float r = t == 0 ? 0 : 1.0f / t;
		glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
		glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

		tan1[i1] += sdir;
		tan1[i2] += sdir;
		tan1[i3] += sdir;

		tan2[i1] += tdir;
		tan2[i2] += tdir;
		tan2[i3] += tdir;
	}

	for (unsigned int a = 0; a < vertexCount; a++)
	{
		const glm::vec3& n = a_vertices[a].normal.xyz();
		const glm::vec3& t = tan1[a];

		// Gram-Schmidt orthogonalize
		glm::vec3 p = t - n * glm::dot(n, t);
		if ( glm::length2(p) != 0 )
		{
			a_vertices[a].tangent = glm::vec4( glm::normalize( p ), 0.0f );

			// calculate binormal
			float sign = glm::dot(glm::cross(n.xyz(), t.xyz()), tan2[a].xyz()) < 0.0f ? -1.0f : 1.0f;
			a_vertices[a].binormal = glm::vec4(glm::cross(a_vertices[a].normal.xyz(),a_vertices[a].tangent.xyz()) * sign, 0);
		}
	}

	delete[] tan1;
}

unsigned int FBXFile::nodeCount(FBXNode* a_node)
{
	if (a_node == nullptr)
		return 0;

	unsigned int uiCount = 1;

#if (_MSC_VER == 1600)
	for each (auto n in a_node->m_children)
#else
	for (auto n : a_node->m_children)
#endif
		uiCount += nodeCount(n);

	return uiCount;
}

FBXMeshNode* FBXFile::getMeshByName(const char* a_name)
{
	auto oIter = m_meshes.find(a_name);
	if (oIter != m_meshes.end())
		return oIter->second;
	return nullptr;
}

FBXLightNode* FBXFile::getLightByName(const char* a_name)
{
	auto oIter = m_lights.find(a_name);
	if (oIter != m_lights.end())
		return oIter->second;
	return nullptr;
}

FBXCameraNode* FBXFile::getCameraByName(const char* a_name)
{
	auto oIter = m_cameras.find(a_name);
	if (oIter != m_cameras.end())
		return oIter->second;
	return nullptr;
}

FBXMaterial* FBXFile::getMaterialByName(const char* a_name)
{
	auto oIter = m_materials.find(a_name);
	if (oIter != m_materials.end())
		return oIter->second;
	return nullptr;
}

FBXAnimation* FBXFile::getAnimationByName(const char* a_name)
{
	auto oIter = m_animations.find(a_name);
	if (oIter != m_animations.end())
		return oIter->second;
	return nullptr;
}

FBXMeshNode* FBXFile::getMeshByIndex(unsigned int a_index)
{
	FBXMeshNode* mesh = nullptr;
	auto iter = m_meshes.begin();
	unsigned int size = (unsigned int)m_meshes.size();

	for ( unsigned int i = 0 ; i <= a_index && a_index < size ; ++i, ++iter )
		mesh = iter->second; 

	return mesh;
}

FBXLightNode* FBXFile::getLightByIndex(unsigned int a_index)
{
	FBXLightNode* light = nullptr;
	auto iter = m_lights.begin();
	unsigned int size = (unsigned int)m_lights.size();

	for ( unsigned int i = 0 ; i <= a_index && a_index < size ; ++i, ++iter )
		light = iter->second; 

	return light;
}

FBXCameraNode* FBXFile::getCameraByIndex(unsigned int a_index)
{
	FBXCameraNode* camera = nullptr;
	auto iter = m_cameras.begin();
	unsigned int size = (unsigned int)m_cameras.size();

	for ( unsigned int i = 0 ; i <= a_index && a_index < size ; ++i, ++iter )
		camera = iter->second; 

	return camera;
}

FBXMaterial* FBXFile::getMaterialByIndex(unsigned int a_index)
{
	FBXMaterial* material = nullptr;
	auto iter = m_materials.begin();
	unsigned int size = (unsigned int)m_materials.size();

	for ( unsigned int i = 0 ; i <= a_index && a_index < size ; ++i, ++iter )
		material = iter->second; 

	return material;
}

FBXAnimation* FBXFile::getAnimationByIndex(unsigned int a_index)
{
	FBXAnimation* animation = nullptr;
	auto iter = m_animations.begin();
	unsigned int size = (unsigned int)m_animations.size();

	for ( unsigned int i = 0 ; i <= a_index && a_index < size ; ++i, ++iter )
		animation = iter->second; 

	return animation;
}

unsigned int FBXFile::getTextureByName(const char* a_name)
{
	auto oIter = m_textures.find(a_name);
	if (oIter != m_textures.end())
		return oIter->second;
	return 0;
}

unsigned int FBXFile::getTextureByIndex(unsigned int a_index)
{
#if (_MSC_VER == 1600)
	for each (auto t in m_textures)
#else
	for (auto t : m_textures)
#endif
	{
		if (a_index-- == 0)
			return t.second;
	}

	return 0;
}

void FBXNode::updateGlobalTransform()
{
	if (m_parent != nullptr)
		m_globalTransform = m_parent->m_globalTransform * m_localTransform;
	else
		m_globalTransform = m_localTransform;

#if (_MSC_VER == 1600)
	for each (auto child in m_children)
#else
	for (auto child : m_children)
#endif
		child->updateGlobalTransform();
}

void FBXCameraNode::updateGlobalTransform()
{
	if (m_parent != nullptr)
		m_globalTransform = m_parent->m_globalTransform * m_localTransform;
	else
		m_globalTransform = m_localTransform;

	m_viewMatrix = glm::inverse( m_globalTransform );

#if (_MSC_VER == 1600)
	for each (auto child in m_children)
#else
	for (auto child : m_children)
#endif
		child->updateGlobalTransform();
}

FBXAnimation* FBXAnimation::clone() const
{
	FBXAnimation* copy = new FBXAnimation();
	strncpy(copy->m_name,m_name,MAX_PATH);
	copy->m_startFrame = m_startFrame;
	copy->m_endFrame = m_endFrame;
	copy->m_trackCount = m_trackCount;
	copy->m_tracks = new FBXTrack[ m_trackCount ];

	for ( unsigned int i = 0 ; i < m_trackCount ; ++i )
	{
		copy->m_tracks[i].m_boneIndex = m_tracks[i].m_boneIndex;
		copy->m_tracks[i].m_keyframeCount = m_tracks[i].m_keyframeCount;
		copy->m_tracks[i].m_keyframes = new FBXKeyFrame[ m_tracks[i].m_keyframeCount ];

		memcpy(copy->m_tracks[i].m_keyframes,m_tracks[i].m_keyframes,sizeof(FBXKeyFrame) * m_tracks[i].m_keyframeCount);
	}

	return copy;
}
