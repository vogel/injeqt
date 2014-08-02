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

#include "extract-interfaces.cpp"
#include "implementation.cpp"
#include "implemented-by.cpp"
#include "instantiation-state.cpp"
#include "type.cpp"

#include "expect.h"
#include "utils.h"

#include <QtTest/QtTest>

using namespace injeqt::v1;

class type_1 : public QObject
{
	Q_OBJECT
};

class type_1_sub_1 : public type_1
{
	Q_OBJECT
};

class instantiation_state_test : public QObject
{
	Q_OBJECT

public:
	instantiation_state_test();

private slots:
	void should_create_empty_state();
	void should_create_state_without_objects();
	void should_create_state_with_objects();
	void should_throw_for_invalid_types();
	void should_throw_for_inconsistent_types();

private:
	type type_1_type;
	type type_1_sub_1_type;
	implemented_by_mapping simple_mapping;
	implemented_by_mapping inheriting_mapping;

};

instantiation_state_test::instantiation_state_test() :
	type_1_type{make_type<type_1>()},
	type_1_sub_1_type{make_type<type_1_sub_1>()},
	simple_mapping
	{
		implemented_by{type_1_type, type_1_type}
	},
	inheriting_mapping
	{
		implemented_by{type_1_type, type_1_sub_1_type},
		implemented_by{type_1_sub_1_type, type_1_sub_1_type}
	}
{
}

void instantiation_state_test::should_create_empty_state()
{
	auto state = instantiation_state{{}, {}};
	QCOMPARE(state.available_types(), implemented_by_mapping{});
	QCOMPARE(state.objects(), implementations{});
}

void instantiation_state_test::should_create_state_without_objects()
{
	auto simple_state = instantiation_state{simple_mapping, {}};
	QCOMPARE(simple_state.available_types(), simple_mapping);
	QCOMPARE(simple_state.objects(), implementations{});

	auto inheriting_state = instantiation_state{inheriting_mapping, {}};
	QCOMPARE(inheriting_state.available_types(), inheriting_mapping);
	QCOMPARE(inheriting_state.objects(), implementations{});
}

void instantiation_state_test::should_create_state_with_objects()
{
	auto simple_type_1_object = make_object<type_1>();
	auto simple_implementations = implementations{implementation{type_1_type, simple_type_1_object.get()}};
	auto simple_state = instantiation_state{simple_mapping, simple_implementations};
	QCOMPARE(simple_state.available_types(), simple_mapping);
	QCOMPARE(simple_state.objects(), simple_implementations);

	auto inheriting_type_1_sub_1_object = make_object<type_1_sub_1>();
	auto inheriting_implementations = implementations{implementation{type_1_sub_1_type, inheriting_type_1_sub_1_object.get()}};
	auto inheriting_state = instantiation_state{inheriting_mapping, inheriting_implementations};
	QCOMPARE(inheriting_state.available_types(), inheriting_mapping);
	QCOMPARE(inheriting_state.objects(), inheriting_implementations);
}

void instantiation_state_test::should_throw_for_invalid_types()
{
	expect<type_not_in_mapping_exception>([&]{
		auto simple_type_1_object = make_object<type_1>();
		auto simple_implementations = implementations{implementation{type_1_type, simple_type_1_object.get()}};
		auto simple_state = instantiation_state{{}, simple_implementations};
	});

	expect<type_not_in_mapping_exception>([&]{
		auto inheriting_type_1_sub_1_object = make_object<type_1_sub_1>();
		auto inheriting_implementations = implementations{implementation{type_1_sub_1_type, inheriting_type_1_sub_1_object.get()}};
		auto simple_state = instantiation_state{simple_mapping, inheriting_implementations};
	});
}

void instantiation_state_test::should_throw_for_inconsistent_types()
{
	expect<type_implementation_inconsistent_exception>([&]{
		auto simple_type_1_object = make_object<type_1>();
		auto simple_implementations = implementations{implementation{type_1_type, simple_type_1_object.get()}};
		auto simple_state = instantiation_state{inheriting_mapping, simple_implementations};
	});

	expect<type_implementation_inconsistent_exception>([&]{
		auto inheriting_type_1_sub_1_object = make_object<type_1_sub_1>();
		auto simple_implementations = implementations{implementation{type_1_type, inheriting_type_1_sub_1_object.get()}};
		auto simple_state = instantiation_state{inheriting_mapping, simple_implementations};
	});
}

QTEST_APPLESS_MAIN(instantiation_state_test);

#include "instantiation-state-test.moc"
