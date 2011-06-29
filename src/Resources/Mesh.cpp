/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Resources/API.h"
#include "Resources/Mesh.h"
#include "Resources/Skeleton.h"
#include "Resources/Animation.h"
#include "Core/Utilities.h"
#include "Math/Vector.h"

NAMESPACE_BEGIN

//-----------------------------------//

REFLECT_CHILD_CLASS(Mesh, Resource)
REFLECT_CLASS_END()

//-----------------------------------//

Mesh::Mesh()
	: animated(false)
	, bindPose(nullptr)
	, built(false)
{ }

//-----------------------------------//

bool Mesh::isBuilt() const
{
	return built;
}

//-----------------------------------//

bool Mesh::isAnimated() const
{
	return animated;
}

//-----------------------------------//

AnimationPtr Mesh::findAnimation( const String& name )
{
	for( size_t i = 0; i < animations.size(); i++ )
	{
		 const AnimationPtr& animation = animations[i];

		if( StringCompareInsensitive(animation->getName(), name) == 0 )
			return animation;
	}

	return nullptr;
}

//-----------------------------------//

void Mesh::buildBounds()
{
	boundingVolume.reset();

	for( size_t i = 0; i < groups.size(); i++ )
	{
		MeshGroup& group = groups[i];
		const std::vector<uint16>& indices = group.indices;
		
		// Update the bounding box to accomodate new geometry.
		for( size_t j = 0; j < indices.size(); j++ )
		{
			const Vector3& v = position[indices[j]];
			boundingVolume.add(v);
		}
	}

	if( boundingVolume.isInfinite() )
		boundingVolume.setZero();
}

//-----------------------------------//

void Mesh::setupInitialVertices()
{
	if( !isAnimated() )
		return;

	if( !skeleton )
		return;

	for( size_t i = 0; i < position.size(); i++ )
	{
		int32 boneIndex = (int32) boneIndices[i];

		if( boneIndex == -1 )
			continue;
		
		const BonePtr& bone = skeleton->getBone(boneIndex);
		Matrix4x3 invJoint = bone->absoluteMatrix.inverse();
		
		Vector3& vertex = position[i];			
		vertex = invJoint*vertex;
	}
}

//-----------------------------------//

NAMESPACE_END