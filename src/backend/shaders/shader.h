#pragma once
#include "backend/internal/object.h"

namespace null::render::backend {
	class i_shader : public i_object {
	public:
		virtual void use() = 0;
		virtual bool empty() const = 0;
	};

	class i_compiled_shader_object : public i_object {
	public:
		virtual bool empty() const = 0;
	};
}