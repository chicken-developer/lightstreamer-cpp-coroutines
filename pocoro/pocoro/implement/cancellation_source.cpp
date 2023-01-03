
#include "../pocorocore/cancellation_source.hpp"
#include "cancellation_state.hpp"
#include <cassert>

pocoro::cancellation_source::cancellation_source()
	: m_state(pocoro::detail::cancellation_state::create())
{
}

pocoro::cancellation_source::cancellation_source(const cancellation_source& other) noexcept
	: m_state(other.m_state)
{
	if (m_state != nullptr)
	{
		m_state->add_source_ref();
	}
}

pocoro::cancellation_source::cancellation_source(cancellation_source&& other) noexcept
	: m_state(other.m_state)
{
	other.m_state = nullptr;
}

pocoro::cancellation_source::~cancellation_source()
{
	if (m_state != nullptr)
	{
		m_state->release_source_ref();
	}
}

pocoro::cancellation_source& pocoro::cancellation_source::operator=(const cancellation_source& other) noexcept
{
	if (m_state != other.m_state)
	{
		if (m_state != nullptr)
		{
			m_state->release_source_ref();
		}

		m_state = other.m_state;

		if (m_state != nullptr)
		{
			m_state->add_source_ref();
		}
	}

	return *this;
}

pocoro::cancellation_source& pocoro::cancellation_source::operator=(cancellation_source&& other) noexcept
{
	if (this != &other)
	{
		if (m_state != nullptr)
		{
			m_state->release_source_ref();
		}

		m_state = other.m_state;
		other.m_state = nullptr;
	}

	return *this;
}

bool pocoro::cancellation_source::can_be_cancelled() const noexcept
{
	return m_state != nullptr;
}

pocoro::cancellation_token pocoro::cancellation_source::token() const noexcept
{
	return cancellation_token(m_state);
}

void pocoro::cancellation_source::request_cancellation()
{
	if (m_state != nullptr)
	{
		m_state->request_cancellation();
	}
}

bool pocoro::cancellation_source::is_cancellation_requested() const noexcept
{
	return m_state != nullptr && m_state->is_cancellation_requested();
}
