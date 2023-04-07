#pragma once

// The file "FlatFormer.h" is used to connect ONLY on the user's side in order to be able to use the engine

#include "Application/Application.h"

// -----EntryPoint-----
#include "EntryPoint.h"
// --------------------


// example:
//
//	#include <FlatFormer.h>
//	
//	class UserClass : public FlatFormer::Application
//	{
//	public:
//		UserClass()
//		{}
//	
//		~UserClass()
//		{}
//	}
//
//  // определение функции создания приложения пользователем (см. "Application.h")
//	FlatFormer::Application* FlatFormer::CreateApplication()
//	{
//		return new UserClass();
//	}