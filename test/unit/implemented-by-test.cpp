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

#include "exception/exception.cpp"
#include "extract-interfaces.cpp"
#include "implemented-by.cpp"
#include "type.cpp"

#include "expect.h"
#include "utils.h"

#include <QtTest/QtTest>

using namespace injeqt::internal;
using namespace injeqt::v1;

class type_1 : public QObject
{
	Q_OBJECT
};

class type_2 : public QObject
{
	Q_OBJECT
};

class type_1_subtype_1 : public type_1
{
	Q_OBJECT
};

class type_1_subtype_1_subtype_1 : public type_1_subtype_1
{
	Q_OBJECT
};

class implemented_by_test : public QObject
{
	Q_OBJECT

public:
	implemented_by_test();

private slots:
	void should_accept_implemented_by_self();
	void should_accept_implemented_by_subtype();
	void should_not_accept_implemented_by_supertype();
	void should_not_accept_implemented_by_unrelated_type();

private:
	type type_1_type;
	type type_2_type;
	type type_1_subtype_1_type;
	type type_1_subtype_1_subtype_1_type;

};

implemented_by_test::implemented_by_test() :
	type_1_type{make_type<type_1>()},
	type_2_type{make_type<type_2>()},
	type_1_subtype_1_type{make_type<type_1_subtype_1>()},
	type_1_subtype_1_subtype_1_type{make_type<type_1_subtype_1_subtype_1>()}
{
}

void implemented_by_test::should_accept_implemented_by_self()
{
	auto i = make_validated<implemented_by>(type_1_type, type_1_type);

	QCOMPARE(type_1_type, i.interface_type());
	QCOMPARE(type_1_type, i.implementation_type());
}

void implemented_by_test::should_accept_implemented_by_subtype()
{
	auto i = make_validated<implemented_by>(type_1_type, type_1_subtype_1_subtype_1_type);

	QCOMPARE(type_1_type, i.interface_type());
	QCOMPARE(type_1_subtype_1_subtype_1_type, i.implementation_type());
}

void implemented_by_test::should_not_accept_implemented_by_supertype()
{
	expect<invalid_implemented_by_exception>([&]{
		auto i = make_validated<implemented_by>(type_1_subtype_1_subtype_1_type, type_1_type);
	});
}

void implemented_by_test::should_not_accept_implemented_by_unrelated_type()
{
	expect<invalid_implemented_by_exception>([&]{
		auto i = make_validated<implemented_by>(type_1_type, type_2_type);
	});
}

QTEST_APPLESS_MAIN(implemented_by_test)
#include "implemented-by-test.moc"
