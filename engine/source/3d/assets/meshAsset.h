//-----------------------------------------------------------------------------
// Copyright (c) 2014 Andrew Mac
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef _MESH_ASSET_H_
#define _MESH_ASSET_H_

#ifndef _ASSET_PTR_H_
#include "assets/assetPtr.h"
#endif

#ifndef _VERTEXLAYOUTS_H_
#include "graphics/utilities.h"
#endif

#ifndef _TEXTURE_MANAGER_H_
#include "graphics/TextureManager.h"
#endif

#ifndef _SHADERS_H_
#include "graphics/shaders.h"
#endif

#ifndef __AI_SCENE_H_INC__
#include <assimp/scene.h>
#endif

#ifndef AI_QUATERNION_H_INC
#include <assimp/quaternion.h>
#endif

#ifndef AI_ANIM_H_INC
#include <assimp/anim.h>
#endif

#ifndef _HASHTABLE_H
#include "collection/hashTable.h"
#endif

//-----------------------------------------------------------------------------

DefineConsoleType( TypeMeshAssetPtr )

//-----------------------------------------------------------------------------

class MeshAsset : public AssetBase
{
   struct SubMesh
   {
      Vector<Graphics::PosUVBonesVertex>     mRawVerts;
      Vector<U16>                            mRawIndices;
      bgfx::VertexBufferHandle               mVertexBuffer;
      bgfx::IndexBufferHandle                mIndexBuffer;
      Box3F                                  mBoundingBox;
      U32                                    mMaterialIndex;
   };

private:
   typedef AssetBase  Parent;

   HashMap<const char*, U32>              mBoneMap;
   Vector<MatrixF>                        mBoneOffsets;
   Vector<SubMesh>                        mMeshList;
   StringTableEntry                       mMeshFile;
   const aiScene*                         mScene;
   Box3F                                  mBoundingBox;

public:
   MeshAsset();
   virtual ~MeshAsset();

   static void initPersistFields();
   virtual bool onAdd();
   virtual void onRemove();
   virtual void copyTo(SimObject* object);

   // Asset validation.
   virtual bool isAssetValid( void ) const;

   // Mesh Handling.
   void                       setMeshFile( const char* pMeshFile );
   inline StringTableEntry    getMeshFile( void ) const { return mMeshFile; };
   U32                        getMeshCount() { return mMeshList.size(); }
   Box3F                      getBoundingBox() { return mBoundingBox; }

   // Animation Functions
   U32 getAnimatedTransforms(F64 TimeInSeconds, F32* transformsOut);

   // Buffers
   bgfx::VertexBufferHandle  getVertexBuffer(U32 idx) { return mMeshList[idx].mVertexBuffer; }
   bgfx::IndexBufferHandle   getIndexBuffer(U32 idx) { return mMeshList[idx].mIndexBuffer; }
   U32                       getMaterialIndex(U32 idx) { return mMeshList[idx].mMaterialIndex; }

   /// Declare Console Object.
   DECLARE_CONOBJECT(MeshAsset);

protected:
   virtual void initializeAsset( void );
   virtual void onAssetRefresh( void );

   // Mesh Handling
   void  loadMesh();
   void  importMesh();
   void  saveBin();
   void  loadBin();
   void  processMesh();

   // Animation Functions.
   U32 _readNodeHeirarchy(F64 AnimationTime, const aiNode* pNode, MatrixF ParentTransform, MatrixF GlobalInverseTransform, F32* transformsOut);
   aiNodeAnim* _findNodeAnim(const aiAnimation* pAnimation, const char* nodeName);
   void _calcInterpolatedRotation(aiQuaternion& Out, F64 AnimationTime, const aiNodeAnim* pNodeAnim);
   U32 _findRotation(F64 AnimationTime, const aiNodeAnim* pNodeAnim);
   void _calcInterpolatedScaling(aiVector3D& Out, F64 AnimationTime, const aiNodeAnim* pNodeAnim);
   U32 _findScaling(F64 AnimationTime, const aiNodeAnim* pNodeAnim);
   void _calcInterpolatedPosition(aiVector3D& Out, F64 AnimationTime, const aiNodeAnim* pNodeAnim);
   U32 _findPosition(F64 AnimationTime, const aiNodeAnim* pNodeAnim);

   static bool setMeshFile( void* obj, const char* data )                 { static_cast<MeshAsset*>(obj)->setMeshFile(data); return false; }
   static const char* getMeshFile(void* obj, const char* data)            { return static_cast<MeshAsset*>(obj)->getMeshFile(); }
};

#endif // _MESH_ASSET_H_