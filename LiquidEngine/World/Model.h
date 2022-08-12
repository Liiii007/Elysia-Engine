#pragma once
#include <vector>
#include <array>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include "../DataStructures.h"

using Microsoft::WRL::ComPtr;

class Model
{
public:
	Model(std::string name, std::string path) :name(name) {
		//importFromDisk(path);
	
	};

	std::string name;
	std::array<Vertex, 8> vertices =
	{
		Vertex({ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),  }),
		Vertex({ DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f),  }),
		Vertex({ DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f),  }),
		Vertex({ DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f),  }),
		Vertex({ DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f),  }),
		Vertex({ DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f),  }),
		Vertex({ DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f),  }),
		Vertex({ DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f),  })
	};


	std::array<std::uint16_t, 36> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};
	
	void processMesh(aiMesh* mesh, const aiScene* scene);
	bool importFromDisk(std::string path);
	void SetToBox();
	void SetView();


	//Vertex Buffer
	D3D12_VERTEX_BUFFER_VIEW mVBV;
	D3D12_INDEX_BUFFER_VIEW mIBV;


	ComPtr<ID3DBlob> VertexBufferCPU;
	ComPtr<ID3DBlob> IndexBufferCPU;
	ComPtr<ID3D12Resource> VertexBufferGPU;
	ComPtr<ID3D12Resource> VertexBufferUploader;
	ComPtr<ID3D12Resource> IndexBufferGPU;
	ComPtr<ID3D12Resource> IndexBufferUploader;

};