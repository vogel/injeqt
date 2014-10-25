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

#include "injeqt/exception/exception.h"

#include <QtCore/QMetaMethod>
#include <QtCore/QObject>
#include <QtTest/QTest>
#include <memory>

namespace injeqt { namespace v1 {

template<typename T>
std::unique_ptr<QObject> make_object()
{
	return std::unique_ptr<QObject>(new T{});
}

template<typename T>
inline QMetaMethod make_method(const std::string &signature)
{
	return T::staticMetaObject.method(T::staticMetaObject.indexOfMethod(signature.data()));
}

template<typename T>
void test_compare(const std::vector<std::vector<T>> objects)
{
	for (decltype(objects.size()) i = 0; i < objects.size(); i++)
		for (decltype(objects.size()) j = 0; j < objects.size(); j++)
			if (i != j)
			{
				for (auto &&x : objects[i])
					for (auto &&y : objects[j])
					{
						QVERIFY(x != y);
						QVERIFY(y != x);
					}
			}
			else
			{
				for (auto &&x : objects[i])
					for (auto &&y : objects[j])
					{
						QVERIFY(x == y);
						QVERIFY(y == x);
					}
			}
}

template<typename T>
void test_compare_ex(const std::vector<std::vector<T>> objects)
{
	for (decltype(objects.size()) i = 0; i < objects.size(); i++)
		for (decltype(objects.size()) j = 0; j < objects.size(); j++)
			if (i != j)
			{
				for (auto &&x : objects[i])
				{
					auto first = true;
					auto c1 = false;
					auto c2 = false;
					auto c3 = false;
					auto c4 = false;
					auto c5 = false;
					auto c6 = false;
					auto c7 = false;
					auto c8 = false;

					for (auto &&y : objects[j])
					{
						QVERIFY(x != y);
						QVERIFY(y != x);
						QVERIFY((x < y) ^ (y < x));
						QVERIFY((x > y) ^ (y > x));
						QVERIFY((x <= y) ^ (y <= x));
						QVERIFY((x >= y) ^ (y >= x));
						if (first)
						{
							c1 = x < y;
							c2 = y < x;
							c3 = x <= y;
							c4 = y <= x;
							c5 = x > y;
							c6 = y > x;
							c7 = x >= y;
							c8 = y >= x;
						}
						else
						{
							QCOMPARE(c1, x < y);
							QCOMPARE(c2, y < x);
							QCOMPARE(c3, x <= y);
							QCOMPARE(c4, y <= y);
							QCOMPARE(c5, x > y);
							QCOMPARE(c6, y > x);
							QCOMPARE(c7, x >= y);
							QCOMPARE(c8, y >= x);
						}
					}
				}
			}
			else
			{
				for (auto &&x : objects[i])
					for (auto &&y : objects[j])
					{
						QVERIFY(x == y);
						QVERIFY(y == x);
					}
			}
}

}}
