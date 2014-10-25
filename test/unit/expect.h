/*
 * %injeqt copyright begin%
 * Copyright 2014 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
 * %injeqt copyright end%
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once

#include <QtTest/QTest>
#include <functional>

template<typename E>
void expect(const std::vector<std::string> &contents, std::function<void()> call)
{
	try
	{
		call();
		QFAIL("Exception not thrown");
	}
	catch (E &e)
	{
		for (auto &&content : contents)
			if (!std::string(e.what()).find(content) == std::string::npos)
				QFAIL(qPrintable(QString{"Expected string '%1' not found in exception message '%2'"}
					.arg(content.c_str())
					.arg(e.what())));
	}
	catch (std::exception &e)
	{
		QFAIL(qPrintable(QString{"Unexpected exception thrown: %1 %2"}
			.arg(typeid(e).name())
			.arg(e.what())));
	}
	catch (...)
	{
		QFAIL("Unexpected exception thrown");
	}
}

template<typename E>
void expect(std::function<void()> call)
{
	expect<E>({}, call);
}
