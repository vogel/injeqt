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

#include "exception/empty-type.cpp"
#include "exception/exception.cpp"
#include "exception/interface-not-implemented.cpp"
#include "exception/invalid-qobject.cpp"
#include "exception/qobject-type.cpp"
#include "implementation.cpp"
#include "interfaces-utils.cpp"
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

class implementation_test : public QObject
{
	Q_OBJECT

private slots:
	void should_throw_when_type_is_empty();
	void should_throw_when_type_is_qobject();
	void should_throw_when_object_is_null();
	void should_accept_object_of_type();
	void should_accept_object_of_sub_type();
	void should_accept_object_of_sub_sub_type();
	void should_not_accept_object_of_super_type();
	void should_not_accept_object_of_other_type();
	void should_properly_compare();

};

void implementation_test::should_throw_when_type_is_empty()
{
	auto object = make_object<type_1>();
	expect<exception::empty_type>([&]{
		make_implementation(type{}, object.get());
	});
}

void implementation_test::should_throw_when_type_is_qobject()
{
	auto object = make_object<type_1>();
	expect<exception::qobject_type>([&]{
		make_implementation(make_type<QObject>(), object.get());
	});
}

void implementation_test::should_throw_when_object_is_null()
{
	expect<exception::invalid_qobject>({"type_1"}, [&]{
		make_implementation(make_type<type_1>(), nullptr);
	});
}

void implementation_test::should_accept_object_of_type()
{
	auto object = make_object<type_1>();
	auto i = make_implementation(make_type<type_1>(), object.get());

	QCOMPARE(make_type<type_1>(), i.interface_type());
	QCOMPARE(object.get(), i.object());
}

void implementation_test::should_accept_object_of_sub_type()
{
	auto object = make_object<type_1_subtype_1>();
	auto i = make_implementation(make_type<type_1>(), object.get());

	QCOMPARE(make_type<type_1>(), i.interface_type());
	QCOMPARE(object.get(), i.object());
}

void implementation_test::should_accept_object_of_sub_sub_type()
{
	auto object = make_object<type_1_subtype_1_subtype_1>();
	auto i = make_implementation(make_type<type_1>(), object.get());

	QCOMPARE(make_type<type_1>(), i.interface_type());
	QCOMPARE(object.get(), i.object());
}

void implementation_test::should_not_accept_object_of_super_type()
{
	auto object = make_object<type_1>();
	expect<exception::interface_not_implemented>({"type_1", "type_1_subtype_1_subtype_1"}, [&]{
		make_implementation(make_type<type_1_subtype_1_subtype_1>(), object.get());
	});
}

void implementation_test::should_not_accept_object_of_other_type()
{
	auto object = make_object<type_1>();
	expect<exception::interface_not_implemented>({"type_1", "type_2"}, [&]{
		make_implementation(make_type<type_2>(), object.get());
	});
}

void implementation_test::should_properly_compare()
{
	auto o1a = make_object<type_1>();
	auto o1b = make_object<type_1>();
	auto o2 = make_object<type_2>();

	auto i1a = make_implementation(make_type<type_1>(), o1a.get());
	auto i1b = make_implementation(make_type<type_1>(), o1a.get());
	auto i2a = make_implementation(make_type<type_1>(), o1b.get());
	auto i2b = make_implementation(make_type<type_1>(), o1b.get());
	auto i3a = make_implementation(make_type<type_2>(), o2.get());
	auto i3b = make_implementation(make_type<type_2>(), o2.get());
	
	test_compare<implementation>({{i1a, i1b}, {i2a, i2b}, {i3a, i3b}});
}

QTEST_APPLESS_MAIN(implementation_test)
#include "implementation-test.moc"
