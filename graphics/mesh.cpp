#include <graphics/mesh.h>
#include <graphics/primitive.h>
#include <graphics/vertex_buffer.h>
#include <system/platform.h>

namespace gef
{
	Mesh::Mesh(Platform& platform) :
	num_primitives_(0),
	primitives_(NULL),
	vertex_buffer_(NULL),
	platform_(platform)
	{
	}

	Mesh::~Mesh()
	{
		ReleasePrimitives();

		if(vertex_buffer_)
		{
			platform_.RemoveVertexBuffer(vertex_buffer_);
			delete vertex_buffer_;
		}
	}
	bool Mesh::InitVertexBuffer(Platform& platform, const void* vertices, const UInt32 num_vertices, const UInt32 vertex_byte_size, bool read_only)
	{
		vertex_buffer_ = platform.CreateVertexBuffer();
		bool success = vertex_buffer_->Init(platform, vertices, num_vertices, vertex_byte_size, read_only);
		platform.AddVertexBuffer(vertex_buffer_);

		return success;
	}

	void Mesh::AllocatePrimitives(const UInt32 num_primitives)
	{
		if(primitives_)
			ReleasePrimitives();
		num_primitives_ = num_primitives;
		if(num_primitives_)
		{
			primitives_ = new Primitive*[num_primitives_];
			for(UInt32 primitive_index=0;primitive_index<num_primitives_;++primitive_index)
			{
//				primitives_[primitive_index] = AllocatePrimitive();
				primitives_[primitive_index] = new Primitive(platform_);
			}
		}
	}

	Primitive* Mesh::AllocatePrimitive()
	{
		return new Primitive(platform_);
	}

	void Mesh::ReleasePrimitives()
	{
		if(num_primitives_ && primitives_)
		{
			for(UInt32 primitive_index=0;primitive_index<num_primitives_;++primitive_index)
			{
				delete primitives_[primitive_index];
				primitives_[primitive_index] = NULL;
			}
			delete[] primitives_;
			primitives_ = NULL;
		}
		num_primitives_ = 0;
	}

/*
	void Mesh::CalculateBounds(const Vector3* vertex_positions, const UInt32 num_vertices, const UInt32 vertex_byte_size)
	{
		// go through all the vertex positions and set up the axis aligned bounding box that encapsulates the mesh
		const Vector3* position = vertex_positions;
		for(Int32 vertex_num = 0; vertex_num < num_vertices; ++vertex_num)
		{
			aabb_.Update(*position);
			position = reinterpret_cast<const Vector3*>(reinterpret_cast<const UInt8*>(position)+vertex_byte_size);
		}

		// now the aabb is setup, it can be used to initialise the bounding sphere
		bounding_sphere_ = Sphere(aabb_);
	}
*/
}