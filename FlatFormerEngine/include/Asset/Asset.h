#pragma once

#include "Engine/API.h"

namespace FlatFormer { namespace Asset {

	// Represents an asset handle.
	// Name is unique asset ID
	//
	// example:
	// Asset asset("SomeImage.png");
	class Asset
	{
	public:
		std::string	m_name;

	public:
		Asset(const std::string &name) :
			m_name(name)
		{
			/// force to lowercase
			std::transform(m_name.begin(), m_name.end(), m_name.begin(), tolower);
		}
	};



	// Serves to store the converted asset data in the engine format
	class IAssetPayloadData
	{
	public:
		virtual std::string ToString() = 0;
	};

}}