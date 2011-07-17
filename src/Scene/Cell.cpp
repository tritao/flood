/************************************************************************
*
* vapor3D Engine � (2008-2010)
*
*	<http://www.vapor3d.org>
*
************************************************************************/

#include "Engine/API.h"
#include "Terrain/Cell.h"
#include "Terrain/Terrain.h"
#include "Scene/Entity.h"
#include "Scene/Geometry.h"
#include "Math/Helpers.h"
#include "Math/Color.h"

namespace vapor {

//-----------------------------------//

REFLECT_CHILD_CLASS(Cell, Geometry)
REFLECT_CLASS_END()

//-----------------------------------//

Cell::Cell() { }

//-----------------------------------//

Cell::Cell( int32 x, int32 y )
	: x(x), y(y)
{ }

//-----------------------------------//

void Cell::setSettings( const TerrainSettings& settings )
{
	this->settings = &settings;

	int32 size = settings.TextureSize;
	
	image = ImageCreate(AllocatorGetHeap(), size, size, PixelFormat::R8G8B8A8);
	image.Resolve()->setColor( Color::LightGrey );

	// Make a copy of the default cell material.
	Material* mat = Allocate(Material, AllocatorGetHeap(), *settings.Material);
	mat->setTexture(0, image);

	material = HandleCast<Material>( ResourceHandleCreate(mat) );
}

//-----------------------------------//

void Cell::setHeights( const std::vector<float>& heights )
{
	this->heights = heights;

	rend = Allocate(Renderable, AllocatorGetHeap(), PolygonType::Triangles);
	rend->setVertexBuffer( Allocate(VertexBuffer, AllocatorGetHeap()) );
	rend->setIndexBuffer( Allocate(IndexBuffer, AllocatorGetHeap()) );
	rend->setMaterial(material);

	addRenderable(rend);

	rebuildGeometry();
	rebuildNormals();
}

//-----------------------------------//

void Cell::rebuildGeometry()
{
	LogInfo( "Rebuilding geometry of cell (%hd, %hd)", x, y );

	rebuildVertices();
	rebuildIndices();

	// Force bounding-box update.
	markDirty();
}

//-----------------------------------//

void Cell::rebuildVertices()
{
	if( heights.empty() ) return;

	// Vertex data
	std::vector<Vector3> vertex;
	std::vector<Vector3> texCoords;

	int numTiles = settings->NumberTiles;
	int sizeCell = settings->CellSize;

	float offsetX = float(x * sizeCell);
	float offsetZ = float(y * sizeCell);

	float tileSize = float(sizeCell / numTiles);
	uint numExpectedVertices = (numTiles+1)*(numTiles+1);

	for( size_t i = 0; i < numExpectedVertices; i++ )
	{
		uint row = i % (numTiles+1);
		uint col = i / (numTiles+1);

		float X = offsetX + tileSize*row;
		float Z = offsetZ + tileSize*col;
		float Y = heights[i] * settings->MaxHeight;
		
		vertex.push_back( Vector3(X, Y, Z) );
		texCoords.push_back( Vector2(X/sizeCell, Z/sizeCell) );
	}

	assert( vertex.size() == numExpectedVertices );
	assert( texCoords.size() == numExpectedVertices );

	// Vertex buffer setup.
	const VertexBufferPtr& vb = rend->getVertexBuffer();
	vb->set( VertexAttribute::Position, vertex );
	vb->set( VertexAttribute::TexCoord0, texCoords );
}

//-----------------------------------//

void Cell::rebuildIndices()
{
	// Index data.
	std::vector<uint16> indices;

	const uint32 numTiles = settings->NumberTiles;
	
	for( size_t col = 0; col < numTiles; col++ )
	{
		for( size_t row = 0; row < numTiles; row++ )
		{
			int i = col * (numTiles + 1) + row;

			// First triangle
			indices.push_back( uint16(i) );
			indices.push_back( uint16(i+(numTiles+1)) );
			indices.push_back( uint16(i+1) );

			// Second triangle
			indices.push_back( uint16(i+1) );
			indices.push_back( uint16(i+(numTiles+1)) ) ;
			indices.push_back( uint16(i+(numTiles+2)) );
		}
	}

	// Index buffer setup.
	const IndexBufferPtr& ib = rend->getIndexBuffer();
	SetIndexBufferData(ib, indices);
}

//-----------------------------------//

void Cell::rebuildNormals()
{
	rebuildFaceNormals();
	rebuildAveragedNormals();
}

//-----------------------------------//

static Vector3 CalculateTriangleNormal( const Vector3& v1, const Vector3& v2, const Vector3& v3 )
{
	Vector3 vec1 = v2 - v1;
	Vector3 vec2 = v3 - v1;
	Vector3 normal = vec1.cross(vec2);

	return normal.normalize();
}

#define index(i) (*(uint16*) &ib->data[indexSizeBytes * (i)])

void Cell::rebuildFaceNormals()
{
	if( heights.empty() ) return;

	const VertexBufferPtr& vb = rend->getVertexBuffer();
	const IndexBufferPtr& ib = rend->getIndexBuffer();

	const std::vector<Vector3>& vs = vb->getVertices();
	assert( !vs.empty() );

	size_t indexSize = ib->getSize();
	int32 indexSizeBytes = ib->indexSize / 8;
	
	faceNormals.clear();

	LogInfo( "Rebuilding face normals of cell (%hd, %hd)", x, y );

	for( size_t i = 0; i < indexSize; i += 3 )
	{
		Vector3 v1 = vs[index(i+0)];
		Vector3 v2 = vs[index(i+1)];
		Vector3 v3 = vs[index(i+2)];

		Vector3 normal = CalculateTriangleNormal(v1, v2, v3);
		faceNormals.push_back( normal );
	}

	const uint numTiles = settings->NumberTiles;
	assert( faceNormals.size() == numTiles*numTiles*2 );
}

//-----------------------------------//

// Resources:
// http://www.gamedev.net/community/forums/topic.asp?topic_id=163625
// http://www.gamedev.net/reference/articles/article2264.asp
//

#define isRegular(x,y) ((x>=1u) && (x<=(numTiles-1u)) \
						&& (y>=1u) && (y<=(numTiles-1u)))

byte Cell::getNeighborFaces( uint i, std::vector<uint>& ns )
{
	const int numTiles = settings->NumberTiles;
	uint facesPerRow = numTiles*2;
	
	uint row = i / (numTiles+1);
	uint col = i % (numTiles+1);
	
	byte n = 0;

	if( isRegular(col, row) )
	{
		uint startFace = (row-1)*facesPerRow+(col*2);

		ns[n++] = startFace-1;
		ns[n++] = startFace;
		ns[n++] = startFace+1;

		ns[n++] = startFace+facesPerRow-2;
		ns[n++] = startFace+facesPerRow-1;
		ns[n++] = startFace+facesPerRow;
	}
	else
	{
		ns[n++] = 0;
	}

	return n;
}

//-----------------------------------//

void Cell::rebuildAveragedNormals()
{
	if( faceNormals.empty() )
		return;

	const VertexBufferPtr& vb = rend->getVertexBuffer();
	//const IndexBufferPtr& ib = rend->getIndexBuffer();

	const std::vector<Vector3>& vs = vb->getVertices();
	assert( !vs.empty() );

	// Averaged per-vertex normals.
	std::vector<Vector3> normals;

	LogInfo( "Rebuilding average per-vertex normals of cell (%hd, %hd)", x, y );

	std::vector<uint> ns;
	ns.resize(6);

	for(size_t i = 0; i < vs.size(); i++)
	{
		byte n = getNeighborFaces(i, ns);

		Vector3 average;
		for( uint e = 0; e < n; e++ )
			average += faceNormals[ns[e]];

		average /= n;
		average.normalize();

		normals.push_back( average );
	}

	assert( normals.size() == vs.size() );
	vb->set( VertexAttribute::Normal, normals );
}

//-----------------------------------//

} // end namespace