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

#include "exception/ambiguous-types.cpp"
#include "exception/dependency-duplicated.cpp"
#include "exception/dependency-on-self.cpp"
#include "exception/dependency-on-subtype.cpp"
#include "exception/dependency-on-supertype.cpp"
#include "exception/exception.cpp"
#include "exception/invalid-dependency.cpp"
#include "exception/invalid-setter.cpp"
#include "exception/unresolvable-dependencies.cpp"
#include "dependencies.cpp"
#include "dependency.cpp"
#include "implemented-by.cpp"
#include "interfaces-utils.cpp"
#include "setter-method.cpp"
#include "type-dependencies.cpp"
#include "type-relations.cpp"
#include "type.cpp"
#include "types-by-name.cpp"
#include "types-model.cpp"

#include "expect.h"
#include "utils.h"

#include <QtTest/QtTest>

using namespace injeqt::internal;
using namespace injeqt::v1;

class type_1 : public QObject
{
	Q_OBJECT
};

class type_1_subtype_1 : public type_1
{
	Q_OBJECT
};

class type_1_subtype_2 : public type_1
{
	Q_OBJECT
};

class type_1_subtype_2_subtype_1 : public type_1_subtype_2
{
	Q_OBJECT
};

class type_1_subtype_3 : public type_1
{
	Q_OBJECT

public slots:
	INJEQT_SET void set_type_1_subtype_1(type_1_subtype_1 *) {}
	INJEQT_SET void set_type_1_subtype_2(type_1_subtype_2 *) {}

};

class types_model_test : public QObject
{
	Q_OBJECT

public:
	types_model_test();

private slots:
	void should_create_empty_types_model();
	void should_create_one_type_types_model();
	void should_throw_when_one_type_duplicated();
	void should_throw_when_type_and_subtype_passed();
	void should_create_with_common_supertype();
	void should_create_with_dependencies();
	void should_throw_when_unresolvable_dependency();

private:
	types_by_name known_types;
	type type_1_type;
	type type_1_subtype_1_type;
	type type_1_subtype_2_type;
	type type_1_subtype_2_subtype_1_type;
	type type_1_subtype_3_type;

};

types_model_test::types_model_test() :
	type_1_type{make_type<type_1>()},
	type_1_subtype_1_type{make_type<type_1_subtype_1>()},
	type_1_subtype_2_type{make_type<type_1_subtype_2>()},
	type_1_subtype_2_subtype_1_type{make_type<type_1_subtype_2_subtype_1>()},
	type_1_subtype_3_type{make_type<type_1_subtype_3>()}
{
	known_types = types_by_name{std::vector<type>{
		make_type<type_1>(),
		make_type<type_1_subtype_1>(),
		make_type<type_1_subtype_2>(), 
		make_type<type_1_subtype_2_subtype_1>(),
		make_type<type_1_subtype_3>()
	}};
}

void types_model_test::should_create_empty_types_model()
{
	auto empty_1 = types_model{};
	auto empty_2 = make_types_model(known_types, std::vector<type>{}, std::vector<type>{});

	QCOMPARE(empty_1.available_types(), implemented_by_mapping{});
	QCOMPARE(empty_1.mapped_dependencies(), types_dependencies{});
}

void types_model_test::should_create_one_type_types_model()
{
	auto m1 = make_types_model(known_types, {type_1_type}, {type_1_type});

	QCOMPARE(m1.available_types(), (implemented_by_mapping
	{
		implemented_by{type_1_type, type_1_type}
	}));
	QCOMPARE(m1.mapped_dependencies(), (types_dependencies
	{
		make_type_dependencies(known_types, type_1_type)
	}));

	auto m2 = make_types_model(known_types, {type_1_subtype_1_type}, {type_1_subtype_1_type});

	QCOMPARE(m2.available_types(), (implemented_by_mapping
	{
		implemented_by{type_1_type, type_1_subtype_1_type},
		implemented_by{type_1_subtype_1_type, type_1_subtype_1_type}
	}));
	QCOMPARE(m2.mapped_dependencies(), (types_dependencies
	{
		make_type_dependencies(known_types, type_1_subtype_1_type)
	}));
}

void types_model_test::should_throw_when_one_type_duplicated()
{
	expect<exception::ambiguous_types>({}, [&]{
		make_types_model(known_types, {type_1_type, type_1_type}, {type_1_type, type_1_type});
	});
}

void types_model_test::should_throw_when_type_and_subtype_passed()
{
	expect<exception::ambiguous_types>({}, [&]{
		make_types_model(known_types, {type_1_type, type_1_subtype_1_type}, {type_1_type, type_1_subtype_1_type});
	});

	expect<exception::ambiguous_types>({}, [&]{
		make_types_model(known_types, {type_1_subtype_1_type, type_1_type}, {type_1_subtype_1_type, type_1_type});
	});
}

void types_model_test::should_create_with_common_supertype()
{
	auto m = make_types_model(known_types, {type_1_subtype_1_type, type_1_subtype_2_type}, {type_1_subtype_1_type, type_1_subtype_2_type});

	QCOMPARE(m.available_types(), (implemented_by_mapping
	{
		implemented_by{type_1_subtype_1_type, type_1_subtype_1_type},
		implemented_by{type_1_subtype_2_type, type_1_subtype_2_type}
	}));
	QCOMPARE(m.mapped_dependencies(), (types_dependencies
	{
		make_type_dependencies(known_types, type_1_subtype_1_type),
		make_type_dependencies(known_types, type_1_subtype_2_type)
	}));
}

void types_model_test::should_create_with_dependencies()
{
	auto m1 = make_types_model(known_types,
		{type_1_subtype_1_type, type_1_subtype_2_type, type_1_subtype_3_type},
		{type_1_subtype_1_type, type_1_subtype_2_type, type_1_subtype_3_type});

	QCOMPARE(m1.available_types(), (implemented_by_mapping
	{
		implemented_by{type_1_subtype_1_type, type_1_subtype_1_type},
		implemented_by{type_1_subtype_2_type, type_1_subtype_2_type},
		implemented_by{type_1_subtype_3_type, type_1_subtype_3_type}
	}));
	QCOMPARE(m1.mapped_dependencies(), (types_dependencies
	{
		make_type_dependencies(known_types, type_1_subtype_1_type),
		make_type_dependencies(known_types, type_1_subtype_2_type),
		make_type_dependencies(known_types, type_1_subtype_3_type)
	}));

	auto m2 = make_types_model(known_types,
		{type_1_subtype_1_type, type_1_subtype_2_subtype_1_type, type_1_subtype_3_type},
		{type_1_subtype_1_type, type_1_subtype_2_subtype_1_type, type_1_subtype_3_type});

	QCOMPARE(m2.available_types(), (implemented_by_mapping
	{
		implemented_by{type_1_subtype_1_type, type_1_subtype_1_type},
		implemented_by{type_1_subtype_2_type, type_1_subtype_2_subtype_1_type},
		implemented_by{type_1_subtype_2_subtype_1_type, type_1_subtype_2_subtype_1_type},
		implemented_by{type_1_subtype_3_type, type_1_subtype_3_type}
	}));
	QCOMPARE(m2.mapped_dependencies(), (types_dependencies
	{
		make_type_dependencies(known_types, type_1_subtype_1_type),
		make_type_dependencies(known_types, type_1_subtype_2_subtype_1_type),
		make_type_dependencies(known_types, type_1_subtype_3_type)
	}));
}

void types_model_test::should_throw_when_unresolvable_dependency()
{
	expect<exception::unresolvable_dependencies>({"set_type_1_subtype_1", "set_type_1_subtype_2"}, [&]{
		make_types_model(known_types, {type_1_subtype_3_type}, {type_1_subtype_3_type});
	});
}

QTEST_APPLESS_MAIN(types_model_test)
#include "types-model-test.moc"
