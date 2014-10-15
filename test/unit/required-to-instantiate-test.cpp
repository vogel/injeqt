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
#include "exception/empty-type.cpp"
#include "exception/exception.cpp"
#include "exception/interface-not-implemented.cpp"
#include "exception/invalid-dependency.cpp"
#include "exception/invalid-qobject.cpp"
#include "exception/invalid-setter.cpp"
#include "exception/qobject-type.cpp"
#include "exception/unresolvable-dependencies.cpp"
#include "dependencies.cpp"
#include "dependency.cpp"
#include "implementation.cpp"
#include "implemented-by.cpp"
#include "interfaces-utils.cpp"
#include "required-to-instantiate.cpp"
#include "setter-method.cpp"
#include "types-model.cpp"
#include "type-dependencies.cpp"
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
	INJEQT_SETTER void set_type_1(type_1 *) {}
};

class type_2_subtype_1 : public type_2
{
	Q_OBJECT
};

class type_3 : public QObject
{
	Q_OBJECT

public slots:
	INJEQT_SETTER void set_type_2(type_2 *) {}
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
	INJEQT_SETTER void set_type_3(cyclic_type_3 *) {}
};

class cyclic_type_1_subtype_1 : public cyclic_type_1
{
	Q_OBJECT
};

class cyclic_type_2 : public QObject
{
	Q_OBJECT

public slots:
	INJEQT_SETTER void set_type_1(cyclic_type_1 *) {}
};

class cyclic_type_2_subtype_1 : public cyclic_type_2
{
	Q_OBJECT
};

class cyclic_type_3 : public QObject
{
	Q_OBJECT

public slots:
	INJEQT_SETTER void set_type_2(cyclic_type_2 *) {}
};

class cyclic_type_3_subtype_1 : public cyclic_type_3
{
	Q_OBJECT
};

class required_to_instantiate_test : public QObject
{
	Q_OBJECT

public:
	required_to_instantiate_test();

private slots:
	void should_return_type_when_simple_types_and_empty_implementation();
	void should_return_subtype_when_inheriting_types_and_empty_implementation();
	void should_return_nothing_when_simple_types_and_implementation();
	void should_return_nothing_when_inheriting_types_and_only_subtype_implementation();
	void should_return_type_with_dependencies_when_simple_types_and_empty_implementation();
	void should_return_type_when_simple_types_and_dependencies_implementation();
	void should_return_nothing_when_simple_types_and_self_implementation();
	void should_return_subtype_with_dependencies_when_inheriting_types_and_empty_implementation();
	void should_return_type_with_all_dependencies_when_simple_types_and_empty_implementation();
	void should_return_type_with_partial_dependencies_when_simple_types_and_partial_implementation();
	void should_return_type_when_simple_types_and_almost_full_implementation();
	void should_return_all_types_with_cyclic_dependnecies_when_simple_types_and_empty_implementation();
	void should_return_all_subtypes_with_cyclic_dependnecies_when_inheriting_types_and_empty_implementation();
	void should_return_type_when_supertype_is_already_available();

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
	types_model empty_types_model;
	types_model simple_types_model;
	types_model inheriting_types_model;

};

required_to_instantiate_test::required_to_instantiate_test() :
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
	simple_types_model{make_types_model(std::vector<type>
	{
		type_1_type,
		type_2_type,
		type_3_type,
		cyclic_type_1_type,
		cyclic_type_2_type,
		cyclic_type_3_type
	})},
	inheriting_types_model{make_types_model(std::vector<type>
	{
		type_1_subtype_1_type,
		type_2_subtype_1_type,
		type_3_subtype_1_type,
		cyclic_type_1_subtype_1_type,
		cyclic_type_2_subtype_1_type,
		cyclic_type_3_subtype_1_type
	})}
{
}

void required_to_instantiate_test::should_return_type_when_simple_types_and_empty_implementation()
{
	auto result = required_to_instantiate(type_1_type, simple_types_model, {});
	QCOMPARE(result, types{type_1_type});
}

void required_to_instantiate_test::should_return_subtype_when_inheriting_types_and_empty_implementation()
{
	auto result = required_to_instantiate(type_1_type, inheriting_types_model, {});
	QCOMPARE(result, types{type_1_subtype_1_type});
}

void required_to_instantiate_test::should_return_nothing_when_simple_types_and_implementation()
{
	auto type_1_object = make_object<type_1>();
	auto available_implementations = implementations
	{
		implementation{type_1_type, type_1_object.get()}
	};

	auto result = required_to_instantiate(type_1_type, simple_types_model, available_implementations);
	QCOMPARE(result, types{});
}

void required_to_instantiate_test::should_return_nothing_when_inheriting_types_and_only_subtype_implementation()
{
	auto type_1_subtype_1_object = make_object<type_1_subtype_1>();
	auto available_implementations = implementations
	{
		implementation{type_1_subtype_1_type, type_1_subtype_1_object.get()}
	};

	auto result = required_to_instantiate(type_1_type, inheriting_types_model, available_implementations);
	QCOMPARE(result, types{});
}

void required_to_instantiate_test::should_return_type_with_dependencies_when_simple_types_and_empty_implementation()
{
	auto result = required_to_instantiate(type_2_type, simple_types_model, {});
	QCOMPARE(result, (types{type_1_type, type_2_type}));
}

void required_to_instantiate_test::should_return_type_when_simple_types_and_dependencies_implementation()
{
	auto type_1_object = make_object<type_1>();
	auto available_implementations = implementations
	{
		implementation{type_1_type, type_1_object.get()}
	};

	auto result = required_to_instantiate(type_2_type, simple_types_model, available_implementations);
	QCOMPARE(result, (types{type_2_type}));
}

void required_to_instantiate_test::should_return_nothing_when_simple_types_and_self_implementation()
{
	auto type_2_object = make_object<type_2>();
	auto available_implementations = implementations
	{
		implementation{type_2_type, type_2_object.get()}
	};

	auto result = required_to_instantiate(type_2_type, simple_types_model, available_implementations);
	QCOMPARE(result, (types{}));
}

void required_to_instantiate_test::should_return_subtype_with_dependencies_when_inheriting_types_and_empty_implementation()
{
	auto result = required_to_instantiate(type_2_type, inheriting_types_model, {});
	QCOMPARE(result, (types{type_1_subtype_1_type, type_2_subtype_1_type}));
}

void required_to_instantiate_test::should_return_type_with_all_dependencies_when_simple_types_and_empty_implementation()
{
	auto result = required_to_instantiate(type_3_type, simple_types_model, {});
	QCOMPARE(result, (types{type_1_type, type_2_type, type_3_type}));
}

void required_to_instantiate_test::should_return_type_with_partial_dependencies_when_simple_types_and_partial_implementation()
{
	auto type_1_object = make_object<type_1>();
	auto available_implementations = implementations
	{
		implementation{type_1_type, type_1_object.get()}
	};

	auto result = required_to_instantiate(type_3_type, simple_types_model, available_implementations);
	QCOMPARE(result, (types{type_2_type, type_3_type}));
}

void required_to_instantiate_test::should_return_type_when_simple_types_and_almost_full_implementation()
{
	auto type_1_object = make_object<type_1>();
	auto type_2_object = make_object<type_2>();
	auto available_implementations = implementations
	{
		implementation{type_1_type, type_1_object.get()},
		implementation{type_2_type, type_2_object.get()}
	};

	auto result = required_to_instantiate(type_3_type, simple_types_model, available_implementations);
	QCOMPARE(result, (types{type_3_type}));
}

void required_to_instantiate_test::should_return_all_types_with_cyclic_dependnecies_when_simple_types_and_empty_implementation()
{
	auto result1 = required_to_instantiate(cyclic_type_1_type, simple_types_model, {});
	QCOMPARE(result1, (types{cyclic_type_1_type, cyclic_type_2_type, cyclic_type_3_type}));

	auto result2 = required_to_instantiate(cyclic_type_2_type, simple_types_model, {});
	QCOMPARE(result2, (types{cyclic_type_1_type, cyclic_type_2_type, cyclic_type_3_type}));

	auto result3 = required_to_instantiate(cyclic_type_3_type, simple_types_model, {});
	QCOMPARE(result3, (types{cyclic_type_1_type, cyclic_type_2_type, cyclic_type_3_type}));
}

void required_to_instantiate_test::should_return_all_subtypes_with_cyclic_dependnecies_when_inheriting_types_and_empty_implementation()
{
	auto result1 = required_to_instantiate(cyclic_type_1_type, inheriting_types_model, {});
	QCOMPARE(result1, (types{cyclic_type_1_subtype_1_type, cyclic_type_2_subtype_1_type, cyclic_type_3_subtype_1_type}));

	auto result2 = required_to_instantiate(cyclic_type_2_type, inheriting_types_model, {});
	QCOMPARE(result2, (types{cyclic_type_1_subtype_1_type, cyclic_type_2_subtype_1_type, cyclic_type_3_subtype_1_type}));

	auto result3 = required_to_instantiate(cyclic_type_3_type, inheriting_types_model, {});
	QCOMPARE(result3, (types{cyclic_type_1_subtype_1_type, cyclic_type_2_subtype_1_type, cyclic_type_3_subtype_1_type}));
}

void required_to_instantiate_test::should_return_type_when_supertype_is_already_available()
{
	auto type_1_object = make_object<type_1>();
	auto available_implementations = implementations
	{
		implementation{type_1_type, type_1_object.get()},
	};

	auto result = required_to_instantiate(type_1_subtype_1_type, inheriting_types_model, available_implementations);
	QCOMPARE(result, (types{type_1_subtype_1_type}));
}

QTEST_APPLESS_MAIN(required_to_instantiate_test);

#include "required-to-instantiate-test.moc"
