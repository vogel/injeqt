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

#include "default-constructor-method.cpp"
#include "dependencies.cpp"
#include "dependency.cpp"
#include "extract-interfaces.cpp"
#include "implementation.cpp"
#include "implemented-by.cpp"
#include "instantiation-state.cpp"
#include "provider-by-default-constructor.cpp"
#include "required-to-instantiate.cpp"
#include "resolve-dependencies.cpp"
#include "resolved-dependency.cpp"
#include "scope.cpp"
#include "setter-method.cpp"
#include "type-relations.cpp"
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

class type_3 : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void set_type_2(type_2 *) {}
};

class type_3_subtype_1 : public type_3
{
	Q_OBJECT
};

class cyclic_type_3;

class cyclic_type_1 : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void set_type_3(cyclic_type_3 *) {}
};

class cyclic_type_1_subtype_1 : public cyclic_type_1
{
	Q_OBJECT
};

class cyclic_type_2 : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void set_type_1(cyclic_type_1 *) {}
};

class cyclic_type_2_subtype_1 : public cyclic_type_2
{
	Q_OBJECT
};

class cyclic_type_3 : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void set_type_2(cyclic_type_2 *) {}
};

class cyclic_type_3_subtype_1 : public cyclic_type_3
{
	Q_OBJECT
};

class scope_test : public QObject
{
	Q_OBJECT

public:
	scope_test();

private slots:
	void should_create_valid_scopes_with_no_objects();
	void should_throw_when_creating_scope_with_ambiguous_types();

private:
	type type_1_type;
	type type_1_subtype_1_type;
	type type_2_type;
	type type_2_subtype_1_type;
	type type_3_type;
	type type_3_subtype_1_type;
	type cyclic_type_1_type;
	type cyclic_type_1_subtype_1_type;
	type cyclic_type_2_type;
	type cyclic_type_2_subtype_1_type;
	type cyclic_type_3_type;
	type cyclic_type_3_subtype_1_type;
	std::vector<type> simple_types;
	std::vector<type> inheriting_types;
	std::vector<type> simple_cyclic_types;
	std::vector<type> inheriting_simple_cyclic_types;
	implemented_by_mapping simple_mapping;
	implemented_by_mapping inheriting_mapping;
	implemented_by_mapping simple_cycling_mapping;
	implemented_by_mapping inheriting_cycling_mapping;

};

scope_test::scope_test() :
	type_1_type{make_type<type_1>()},
	type_1_subtype_1_type{make_type<type_1_subtype_1>()},
	type_2_type{make_type<type_2>()},
	type_2_subtype_1_type{make_type<type_2_subtype_1>()},
	type_3_type{make_type<type_3>()},
	type_3_subtype_1_type{make_type<type_3_subtype_1>()},
	cyclic_type_1_type{make_type<cyclic_type_1>()},
	cyclic_type_1_subtype_1_type{make_type<cyclic_type_1_subtype_1>()},
	cyclic_type_2_type{make_type<cyclic_type_2>()},
	cyclic_type_2_subtype_1_type{make_type<cyclic_type_2_subtype_1>()},
	cyclic_type_3_type{make_type<cyclic_type_3>()},
	cyclic_type_3_subtype_1_type{make_type<cyclic_type_3_subtype_1>()},
	simple_types{type_1_type, type_2_type, type_3_type},
	inheriting_types{type_1_subtype_1_type, type_2_subtype_1_type, type_3_subtype_1_type},
	simple_cyclic_types{},
	inheriting_simple_cyclic_types{},
	simple_mapping
	{
		implemented_by{type_1_type, type_1_type},
		implemented_by{type_2_type, type_2_type},
		implemented_by{type_3_type, type_3_type},
		implemented_by{cyclic_type_1_type, cyclic_type_1_type},
		implemented_by{cyclic_type_2_type, cyclic_type_2_type},
		implemented_by{cyclic_type_3_type, cyclic_type_3_type}
	},
	inheriting_mapping
	{
		implemented_by{type_1_type, type_1_subtype_1_type},
		implemented_by{type_1_subtype_1_type, type_1_subtype_1_type},
		implemented_by{type_2_type, type_2_subtype_1_type},
		implemented_by{type_2_subtype_1_type, type_2_subtype_1_type},
		implemented_by{type_3_type, type_3_subtype_1_type},
		implemented_by{type_3_subtype_1_type, type_3_subtype_1_type},
		implemented_by{cyclic_type_1_type, cyclic_type_1_subtype_1_type},
		implemented_by{cyclic_type_1_subtype_1_type, cyclic_type_1_subtype_1_type},
		implemented_by{cyclic_type_2_type, cyclic_type_2_subtype_1_type},
		implemented_by{cyclic_type_2_subtype_1_type, cyclic_type_2_subtype_1_type},
		implemented_by{cyclic_type_3_type, cyclic_type_3_subtype_1_type},
		implemented_by{cyclic_type_3_subtype_1_type, cyclic_type_3_subtype_1_type}
	},
	simple_cycling_mapping
	{
	},
	inheriting_cycling_mapping
	{
	}
{
}

void scope_test::should_create_valid_scopes_with_no_objects()
{
	auto empty_scope = make_scope({}, {});
	//QCOMPARE(empty_scope.state(), (instantiation_state{implemented_by_mapping{}, implementations{}}));

	auto simple_scope = make_scope({}, simple_types);
	//QCOMPARE(empty_scope.state(), (instantiation_state{implemented_by_mapping{}, implementations{}}));

	auto inheriting_scope = make_scope({}, inheriting_types);

	auto simple_cyclic_scope = make_scope({}, simple_cyclic_types);

	auto inheriting_simple_cyclic_scope = make_scope({}, inheriting_simple_cyclic_types);
}

void scope_test::should_throw_when_creating_scope_with_ambiguous_types()
{
	auto ambiguous_scope = make_scope({}, std::vector<type>{type_1_type, type_1_subtype_1_type});
}

QTEST_APPLESS_MAIN(scope_test);

#include "scope-test.moc"
