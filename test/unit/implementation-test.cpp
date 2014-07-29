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

#include "implementation.cpp"
#include "implements-extractor.cpp"
#include "type.cpp"

#include "expect.h"
#include "utils.h"

#include <QtTest/QtTest>

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

class implementation_test : public QObject
{
	Q_OBJECT

public:
	implementation_test();

private slots:
	void should_throw_when_object_is_null();
	void should_accept_object_of_type();
	void should_accept_object_of_sub_type();
	void should_accept_object_of_sub_sub_type();
	void should_not_accept_object_of_super_type();
	void should_not_accept_object_of_other_type();

private:
	type type_1_type;
	type type_2_type;
	type type_1_subtype_1_type;
	type type_1_subtype_1_subtype_1_type;

};

implementation_test::implementation_test() :
	type_1_type{make_type<type_1>()},
	type_2_type{make_type<type_2>()},
	type_1_subtype_1_type{make_type<type_1_subtype_1>()},
	type_1_subtype_1_subtype_1_type{make_type<type_1_subtype_1_subtype_1>()}
{
}

void implementation_test::should_throw_when_object_is_null()
{
	expect<invalid_implementation_availability_exception>([&]{
		auto i = implementation{type_1_type, nullptr};
	});
}

void implementation_test::should_accept_object_of_type()
{
	auto object = make_object<type_1>();
	auto i = implementation{type_1_type, object.get()};

	QCOMPARE(type_1_type, i.interface_type());
	QCOMPARE(object.get(), i.object());
}

void implementation_test::should_accept_object_of_sub_type()
{
	auto object = make_object<type_1_subtype_1>();
	auto i = implementation{type_1_type, object.get()};

	QCOMPARE(type_1_type, i.interface_type());
	QCOMPARE(object.get(), i.object());
}

void implementation_test::should_accept_object_of_sub_sub_type()
{
	auto object = make_object<type_1_subtype_1_subtype_1>();
	auto i = implementation{type_1_type, object.get()};

	QCOMPARE(type_1_type, i.interface_type());
	QCOMPARE(object.get(), i.object());
}

void implementation_test::should_not_accept_object_of_super_type()
{
	auto object = make_object<type_1>();
	expect<invalid_interface_type_exception>([&]{
		auto i = implementation{type_1_subtype_1_subtype_1_type, object.get()};
	});
}

void implementation_test::should_not_accept_object_of_other_type()
{
	auto object = make_object<type_1>();
	expect<invalid_interface_type_exception>([&]{
		auto i = implementation{type_2_type, object.get()};
	});
}

QTEST_APPLESS_MAIN(implementation_test)
#include "implementation-test.moc"
