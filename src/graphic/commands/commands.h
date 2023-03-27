#pragma once
#include <null-sdk.h>

namespace null::render::commands {
	class i_command {
	public:
		i_command() { }
		virtual ~i_command() { }

	public:
		virtual void handle() = 0;
	};

	class c_command_buffer : public i_command {
	public:
		std::vector<std::unique_ptr<i_command>> command_buffer{ };

	public:
		virtual void handle() override { std::ranges::for_each(command_buffer, &i_command::handle); }
		virtual void clear() { command_buffer.clear(); }

	public:
		template <typename command_t>
		void add_command(std::unique_ptr<command_t>&& command) {
			command_buffer.push_back(std::move(command));
		}
	};
}