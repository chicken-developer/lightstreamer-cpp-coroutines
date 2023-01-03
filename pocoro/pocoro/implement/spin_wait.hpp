#ifndef POCORO_SPIN_WAIT_HPP_INCLUDED
#define POCORO_SPIN_WAIT_HPP_INCLUDED

#include <cstdint>

namespace pocoro
{
	class spin_wait
	{
	public:

		spin_wait() noexcept;

		bool next_spin_will_yield() const noexcept;

		void spin_one() noexcept;

		void reset() noexcept;

	private:

		std::uint32_t m_count;

	};
}

#endif
