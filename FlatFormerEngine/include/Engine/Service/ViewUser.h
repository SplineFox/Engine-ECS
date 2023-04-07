#pragma once

#include "Render/View.h"

namespace FlatFormer { namespace Service {

	class ViewUser
	{
	private:
		static Render::View* s_ViewService;

	protected:
		static Render::View& GetViewService()
		{
			assert(s_ViewService != nullptr && "ASSERT: s_ViewService was nullptr!");
			return *s_ViewService;
		}

	public:
		static void ProvideViewService(Render::View* viewInstance)
		{
			s_ViewService = viewInstance;
		}
	};


	using FFE_SERV_USER_View = FlatFormer::Service::ViewUser;
}}