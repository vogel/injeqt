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
#include "required-to-satisfy.cpp"
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

class type_2 : public QObject
{
	Q_OBJECT

public slots:
	INJEQT_SET void set_type_1(type_1 *) {}
};

class type_2_subtype_1 : public type_2
{
	Q_OBJECT
};

class type_3 : public QObject
{
	Q_OBJECT

public slots:
	INJEQT_SET void set_type_2(type_2 *) {}
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
	INJEQT_SET void set_type_3(cyclic_type_3 *) {}
};

class cyclic_type_1_subtype_1 : public cyclic_type_1
{
	Q_OBJECT
};

class cyclic_type_2 : public QObject
{
	Q_OBJECT

public slots:
	INJEQT_SET void set_type_1(cyclic_type_1 *) {}
};

class cyclic_type_2_subtype_1 : public cyclic_type_2
{
	Q_OBJECT
};

class cyclic_type_3 : public QObject
{
	Q_OBJECT

public slots:
	INJEQT_SET void set_type_2(cyclic_type_2 *) {}
};

class cyclic_type_3_subtype_1 : public cyclic_type_3
{
	Q_OBJECT
};

class required_to_satisfy_test : public QObject
{
	Q_OBJECT

public:
	required_to_satisfy_test();

private slots:
	void should_return_nothing_when_empty_dependencies();
	void should_return_dependencies_for_simple_model_with_empty_implementations();
	void should_return_nothing_for_simple_model_with_full_implementations();
	void should_return_subtype_dependencies_for_inheriting_model_with_empty_implementations();
	void should_return_nothing_for_inheriting_model_with_full_implementations();
	void should_return_dependencies_for_inheriting_model_with_supertype_implementations();
	void should_return_all_dependencies_for_simple_model_with_empty_implementations();
	void should_return_partial_dependencies_for_simple_model_with_partial_implementations();
	void should_return_all_types_with_cyclic_dependnecies_for_simple_model_with_partial_implementations();
	void should_return_all_subtypes_with_cyclic_dependnecies_for_inheriting_model_with_partial_implementations();
	void should_return_type_when_supertype_is_already_available();

private:
	types_by_name known_types;
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
	dependencies type_1_dependencies;
	dependencies type_1_subtype_1_dependencies;
	dependencies type_2_dependencies;
	dependencies type_3_dependencies;
	dependencies cyclic_type_1_dependencies;
	dependencies cyclic_type_2_dependencies;
	dependencies cyclic_type_3_dependencies;

};

required_to_satisfy_test::required_to_satisfy_test() :
	known_types{types_by_name{std::vector<type>{
		make_type<type_1>(),
		make_type<type_1_subtype_1>(),
		make_type<type_2>(),
		make_type<type_2_subtype_1>(),
		make_type<type_3>(),
		make_type<type_3_subtype_1>(),
		make_type<cyclic_type_1>(),
		make_type<cyclic_type_1_subtype_1>(),
		make_type<cyclic_type_2>(),
		make_type<cyclic_type_2_subtype_1>(),
		make_type<cyclic_type_3>(),
		make_type<cyclic_type_3_subtype_1>()
	}}},
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
	cyclic_type_3_subtype_1_type{make_type<cyclic_type_3_subtype_1>()}
{
	auto simple_types = std::vector<type>{
		type_1_type,
		type_2_type,
		type_3_type,
		cyclic_type_1_type,
		cyclic_type_2_type,
		cyclic_type_3_type
	};
	auto inheriting_types = std::vector<type>{
		type_1_subtype_1_type,
		type_2_subtype_1_type,
		type_3_subtype_1_type,
		cyclic_type_1_subtype_1_type,
		cyclic_type_2_subtype_1_type,
		cyclic_type_3_subtype_1_type
	};
	simple_types_model = make_types_model(known_types, simple_types, simple_types);
	inheriting_types_model = make_types_model(known_types, inheriting_types, inheriting_types);
	type_1_dependencies = simple_types_model.mapped_dependencies().get(type_1_type)->dependency_list();
	type_1_subtype_1_dependencies = inheriting_types_model.mapped_dependencies().get(type_1_subtype_1_type)->dependency_list();
	type_2_dependencies = simple_types_model.mapped_dependencies().get(type_2_type)->dependency_list();
	type_3_dependencies = simple_types_model.mapped_dependencies().get(type_3_type)->dependency_list();
	cyclic_type_1_dependencies = simple_types_model.mapped_dependencies().get(cyclic_type_1_type)->dependency_list();
	cyclic_type_2_dependencies = simple_types_model.mapped_dependencies().get(cyclic_type_2_type)->dependency_list();
	cyclic_type_3_dependencies = simple_types_model.mapped_dependencies().get(cyclic_type_3_type)->dependency_list();
}

void required_to_satisfy_test::should_return_nothing_when_empty_dependencies()
{
	auto result = required_to_satisfy(dependencies{}, simple_types_model, {});
	QCOMPARE(result, types{});
}

void required_to_satisfy_test::should_return_dependencies_for_simple_model_with_empty_implementations()
{
	auto result = required_to_satisfy(type_2_dependencies, simple_types_model, {});
	QCOMPARE(result, (types{type_1_type}));
}

void required_to_satisfy_test::should_return_nothing_for_simple_model_with_full_implementations()
{
	auto type_1_object = make_object<type_1>();
	auto available_implementations = implementations
	{
		implementation{type_1_type, type_1_object.get()}
	};
	auto result = required_to_satisfy(type_2_dependencies, simple_types_model, available_implementations);
	QCOMPARE(result, (types{}));
}

void required_to_satisfy_test::should_return_subtype_dependencies_for_inheriting_model_with_empty_implementations()
{
	auto result = required_to_satisfy(type_2_dependencies, inheriting_types_model, {});
	QCOMPARE(result, (types{type_1_subtype_1_type}));
}

void required_to_satisfy_test::should_return_nothing_for_inheriting_model_with_full_implementations()
{
	auto type_1_subtype_1_object = make_object<type_1_subtype_1>();
	auto available_implementations = implementations
	{
		implementation{type_1_subtype_1_type, type_1_subtype_1_object.get()}
	};
	auto result = required_to_satisfy(type_2_dependencies, inheriting_types_model, available_implementations);
	QCOMPARE(result, (types{}));
}

void required_to_satisfy_test::should_return_dependencies_for_inheriting_model_with_supertype_implementations()
{
	auto type_1_object = make_object<type_1>();
	auto available_implementations = implementations
	{
		implementation{type_1_type, type_1_object.get()}
	};
	auto result = required_to_satisfy(type_2_dependencies, inheriting_types_model, available_implementations);
	QCOMPARE(result, (types{type_1_subtype_1_type}));
}

void required_to_satisfy_test::should_return_all_dependencies_for_simple_model_with_empty_implementations()
{
	auto result = required_to_satisfy(type_3_dependencies, simple_types_model, {});
	QCOMPARE(result, (types{type_1_type, type_2_type}));
}

void required_to_satisfy_test::should_return_partial_dependencies_for_simple_model_with_partial_implementations()
{
	auto type_1_object = make_object<type_1>();
	auto available_implementations = implementations
	{
		implementation{type_1_type, type_1_object.get()}
	};

	auto result = required_to_satisfy(type_3_dependencies, simple_types_model, available_implementations);
	QCOMPARE(result, (types{type_2_type}));
}

void required_to_satisfy_test::should_return_all_types_with_cyclic_dependnecies_for_simple_model_with_partial_implementations()
{
	auto result1 = required_to_satisfy(cyclic_type_1_dependencies, simple_types_model, {});
	QCOMPARE(result1, (types{cyclic_type_1_type, cyclic_type_2_type, cyclic_type_3_type}));

	auto result2 = required_to_satisfy(cyclic_type_2_dependencies, simple_types_model, {});
	QCOMPARE(result2, (types{cyclic_type_1_type, cyclic_type_2_type, cyclic_type_3_type}));

	auto result3 = required_to_satisfy(cyclic_type_3_dependencies, simple_types_model, {});
	QCOMPARE(result3, (types{cyclic_type_1_type, cyclic_type_2_type, cyclic_type_3_type}));
}

void required_to_satisfy_test::should_return_all_subtypes_with_cyclic_dependnecies_for_inheriting_model_with_partial_implementations()
{
	auto result1 = required_to_satisfy(cyclic_type_1_dependencies, inheriting_types_model, {});
	QCOMPARE(result1, (types{cyclic_type_1_subtype_1_type, cyclic_type_2_subtype_1_type, cyclic_type_3_subtype_1_type}));

	auto result2 = required_to_satisfy(cyclic_type_2_dependencies, inheriting_types_model, {});
	QCOMPARE(result2, (types{cyclic_type_1_subtype_1_type, cyclic_type_2_subtype_1_type, cyclic_type_3_subtype_1_type}));

	auto result3 = required_to_satisfy(cyclic_type_3_dependencies, inheriting_types_model, {});
	QCOMPARE(result3, (types{cyclic_type_1_subtype_1_type, cyclic_type_2_subtype_1_type, cyclic_type_3_subtype_1_type}));
}

void required_to_satisfy_test::should_return_type_when_supertype_is_already_available()
{
	auto type_1_object = make_object<type_1>();
	auto available_implementations = implementations
	{
		implementation{type_1_type, type_1_object.get()},
	};

	auto result = required_to_satisfy(type_1_subtype_1_dependencies, inheriting_types_model, available_implementations);
	QCOMPARE(result, (types{}));
}

QTEST_APPLESS_MAIN(required_to_satisfy_test);

#include "required-to-satisfy-test.moc"
