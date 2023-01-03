
#include "../pocorocore/cancellation_token.hpp"
#include "../pocorocore/operation_cancelled.hpp"

#include "cancellation_state.hpp"

#include <utility>
#include <cassert>

pocoro::cancellation_token::cancellation_token() noexcept
	: m_state(nullptr)
{
}

pocoro::cancellation_token::cancellation_token(const cancellation_token& other) noexcept
	: m_state(other.m_state)
{
	if (m_state != nullptr)
	{
		m_state->add_token_ref();
	}
}

pocoro::cancellation_token::cancellation_token(cancellation_token&& other) noexcept
	: m_state(other.m_state)
{
	other.m_state = nullptr;
}

pocoro::cancellation_token::~cancellation_token()
{
	if (m_state != nullptr)
	{
		m_state->release_token_ref();
	}
}

pocoro::cancellation_token& pocoro::cancellation_token::operator=(const cancellation_token& other) noexcept
{
	if (other.m_state != m_state)
	{
		if (m_state != nullptr)
		{
			m_state->release_token_ref();
		}

		m_state = other.m_state;

		if (m_state != nullptr)
		{
			m_state->add_token_ref();
		}
	}

	return *this;
}

pocoro::cancellation_token& pocoro::cancellation_token::operator=(cancellation_token&& other) noexcept
{
	if (this != &other)
	{
		if (m_state != nullptr)
		{
			m_state->release_token_ref();
		}

		m_state = other.m_state;
		other.m_state = nullptr;
	}

	return *this;
}

void pocoro::cancellation_token::swap(cancellation_token& other) noexcept
{
	std::swap(m_state, other.m_state);
}

bool pocoro::cancellation_token::can_be_cancelled() const noexcept
{
	return m_state != nullptr && m_state->can_be_cancelled();
}

bool pocoro::cancellation_token::is_cancellation_requested() const noexcept
{
	return m_state != nullptr && m_state->is_cancellation_requested();
}

void pocoro::cancellation_token::throw_if_cancellation_requested() const
{
	if (is_cancellation_requested())
	{
		throw operation_cancelled{};
	}
}

pocoro::cancellation_token::cancellation_token(detail::cancellation_state* state) noexcept
	: m_state(state)
{
	if (m_state != nullptr)
	{
		m_state->add_token_ref();
	}
}
