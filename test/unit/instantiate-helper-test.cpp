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

#include "dependency.cpp"
#include "dependency-extractor.cpp"
#include "implementation.cpp"
#include "interfaces-extractor.cpp"
#include "implemented-by.cpp"
#include "instantiate-helper.cpp"
#include "setter-method.cpp"
#include "type.cpp"
#include "type-relations.cpp"
#include "type-relations-factory.cpp"

#include "expect.h"
#include "utils.h"

#include <QtTest/QtTest>

using namespace injeqt::v1;

class type_1 : public QObject
{
	Q_OBJECT
};

class type_1_subtype_1 : public type_1
{
	Q_OBJECT
};

class type_2 : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void set_type_1(type_1 *) {}
};

class type_2_subtype_1 : public type_2
{
	Q_OBJECT
};

class instantiate_helper_test : public QObject
{
	Q_OBJECT

public:
	instantiate_helper_test();

private slots:
	void should_throw_when_type_not_in_mapping();
	void should_return_type_when_simple_mapping_and_empty_implementation();
	void should_return_subtype_when_inheriting_mapping_and_empty_implementation();
	void should_return_nothing_when_simple_mapping_and_implementation();
	void should_return_nothing_when_inheriting_mapping_and_only_subtype_implementation();
	void should_return_nothing_when_inheriting_mapping_and_subtype_implementation();
	void should_throw_when_inheriting_mapping_and_supertype_implementation_available();
	void should_return_type_with_dependencies_when_simple_mapping_and_empty_implementation();
	void should_return_type_when_simple_mapping_and_dependencies_implementation();
	void should_return_nothing_when_simple_mapping_and_self_implementation();
	void should_return_subtype_with_dependencies_when_inheriting_mapping_and_empty_implementation();

private:
	type type_1_type;
	type type_1_subtype_1_type;
	type type_2_type;
	type type_2_subtype_1_type;
	implemented_by_mapping simple_mapping;
	implemented_by_mapping inheriting_mapping;

};

instantiate_helper_test::instantiate_helper_test() :
	type_1_type{make_type<type_1>()},
	type_1_subtype_1_type{make_type<type_1_subtype_1>()},
	type_2_type{make_type<type_2>()},
	type_2_subtype_1_type{make_type<type_2_subtype_1>()},
	simple_mapping
	{
		implemented_by{type_1_type, type_1_type},
		implemented_by{type_2_type, type_2_type}
	},
	inheriting_mapping
	{
		implemented_by{type_1_type, type_1_subtype_1_type},
		implemented_by{type_1_subtype_1_type, type_1_subtype_1_type},
		implemented_by{type_2_type, type_2_subtype_1_type},
		implemented_by{type_2_subtype_1_type, type_2_subtype_1_type},
	}
{
}

void instantiate_helper_test::should_throw_when_type_not_in_mapping()
{
	expect<type_not_mapped_exception>([&]{
		auto result = instantiate_helper{}.required_to_instantiate(type_1_type, {}, {});
	});
}

void instantiate_helper_test::should_return_type_when_simple_mapping_and_empty_implementation()
{
	auto result = instantiate_helper{}.required_to_instantiate(type_1_type, simple_mapping, {});
	QCOMPARE(result, types{type_1_type});
}

void instantiate_helper_test::should_return_subtype_when_inheriting_mapping_and_empty_implementation()
{
	auto result = instantiate_helper{}.required_to_instantiate(type_1_type, inheriting_mapping, {});
	QCOMPARE(result, types{type_1_subtype_1_type});
}

void instantiate_helper_test::should_return_nothing_when_simple_mapping_and_implementation()
{
	auto type_1_object = make_object<type_1>();
	auto available_implementations = implementations
	{
		implementation{type_1_type, type_1_object.get()}
	};

	auto result = instantiate_helper{}.required_to_instantiate(type_1_type, simple_mapping, available_implementations);
	QCOMPARE(result, types{});
}

void instantiate_helper_test::should_return_nothing_when_inheriting_mapping_and_only_subtype_implementation()
{
	auto type_1_subtype_1_object = make_object<type_1_subtype_1>();
	auto available_implementations = implementations
	{
		implementation{type_1_subtype_1_type, type_1_subtype_1_object.get()}
	};

	auto result = instantiate_helper{}.required_to_instantiate(type_1_type, inheriting_mapping, available_implementations);
	QCOMPARE(result, types{});
}

void instantiate_helper_test::should_return_nothing_when_inheriting_mapping_and_subtype_implementation()
{
	auto type_1_subtype_1_object = make_object<type_1_subtype_1>();
	auto available_implementations = implementations
	{
		implementation{type_1_type, type_1_subtype_1_object.get()},
		implementation{type_1_subtype_1_type, type_1_subtype_1_object.get()}
	};

	auto result = instantiate_helper{}.required_to_instantiate(type_1_type, inheriting_mapping, available_implementations);
	QCOMPARE(result, types{});
}

void instantiate_helper_test::should_throw_when_inheriting_mapping_and_supertype_implementation_available()
{
	auto type_1_object = make_object<type_1>();
	auto available_implementations = implementations
	{
		implementation{type_1_type, type_1_object.get()}
	};

	expect<supertype_implementation_available>([&]{
		auto result = instantiate_helper{}.required_to_instantiate(type_1_subtype_1_type, inheriting_mapping, available_implementations);
	});
}

void instantiate_helper_test::should_return_type_with_dependencies_when_simple_mapping_and_empty_implementation()
{
	auto result = instantiate_helper{}.required_to_instantiate(type_2_type, simple_mapping, {});
	QCOMPARE(result, (types{type_1_type, type_2_type}));
}

void instantiate_helper_test::should_return_type_when_simple_mapping_and_dependencies_implementation()
{
	auto type_1_object = make_object<type_1>();
	auto available_implementations = implementations
	{
		implementation{type_1_type, type_1_object.get()}
	};

	auto result = instantiate_helper{}.required_to_instantiate(type_2_type, simple_mapping, available_implementations);
	QCOMPARE(result, (types{type_2_type}));
}

void instantiate_helper_test::should_return_nothing_when_simple_mapping_and_self_implementation()
{
	auto type_2_object = make_object<type_2>();
	auto available_implementations = implementations
	{

		implementation{type_2_type, type_2_object.get()}
	};

	auto result = instantiate_helper{}.required_to_instantiate(type_2_type, simple_mapping, available_implementations);
	QCOMPARE(result, (types{}));
}

void instantiate_helper_test::should_return_subtype_with_dependencies_when_inheriting_mapping_and_empty_implementation()
{
	auto result = instantiate_helper{}.required_to_instantiate(type_2_type, inheriting_mapping, {});
	QCOMPARE(result, (types{type_1_subtype_1_type, type_2_subtype_1_type}));
}

QTEST_APPLESS_MAIN(instantiate_helper_test);

#include "instantiate-helper-test.moc"
