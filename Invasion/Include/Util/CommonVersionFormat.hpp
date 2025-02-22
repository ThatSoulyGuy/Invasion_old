#pragma once

#include "Util/Typedefs.hpp"

namespace Invasion::Util
{
	class CommonVersionFormat
	{

	public:

		operator String() const
		{
			return std::to_string(first) + "." + std::to_string(second) + "." + std::to_string(third);
		}

		int GetFirst() const
		{
			return first;
		}

		int GetSecond() const
		{
			return second;
		}

		int GetThird() const
		{
			return third;
		}

		static CommonVersionFormat Create(int first, int second, int third)
		{
			CommonVersionFormat out = {};

			out.first = first;
			out.second = second;
			out.third = third;

			return out;
		}

#undef NULL

		static const CommonVersionFormat DEFAULT;
		static const CommonVersionFormat NULL;

	private:

		int first = 0;
		int second = 0;
		int third = 0;

	};

	const CommonVersionFormat CommonVersionFormat::DEFAULT = CommonVersionFormat::Create(0, 0, 1);
	const CommonVersionFormat CommonVersionFormat::NULL = CommonVersionFormat::Create(0, 0, 0);

#define NULL nullptr
}