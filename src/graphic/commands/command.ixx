module;
#include <vector>
#include <memory>
#include <algorithm>
export module null.render:graphic.command;

export namespace null::render {
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

		template <typename command_t, typename ...args_t>
		void add_command(args_t&& ...args) {
			command_buffer.push_back(std::make_unique<command_t>(std::forward<args_t>(args)...));
		}
	};
}