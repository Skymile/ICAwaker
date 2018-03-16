#pragma once

#include "stdafx.h"
#include "error_codes.h"

namespace ic
{
	/// <summary>
	///		Semi-temporary class for Exception handling <para />
	///		Based on C#'s contracts
	/// </summary>
	struct Exception : std::exception
	{
		Exception()
		{
			throw std::exception(UNKNOWN_ERROR);
		}

		Exception(char* message)
		{
			throw std::exception(message);
		}

		void static require(bool condition, const char* description = '\0')
		{
			if (!condition)
			{
				std::exception ex(description == '\0' ? "Unknown error occured." : description);

				try
				{
					throw ex;
				}
				catch (const std::exception&)
				{
					std::cout << ex.what() << "\n";
					throw ex;
				}
			}
		}
	};
}
