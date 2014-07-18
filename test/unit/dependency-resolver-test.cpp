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

#include "dependencies.cpp"
#include "dependency.cpp"
#include "dependency-resolver.cpp"
#include "implements-extractor.cpp"
#include "meta-object.cpp"
#include "meta-object-factory.cpp"
#include "object-with-meta.cpp"
#include "objects-with-meta.cpp"
#include "resolved-dependency.cpp"
#include "setter-method.cpp"
#include "type.cpp"

#include "utils.h"

#include <QtTest/QtTest>

using namespace injeqt::v1;

class injectable_type1 : public QObject
{
	Q_OBJECT
};

class injectable_type2 : public QObject
{
	Q_OBJECT
};

class injectable_type3 : public QObject
{
	Q_OBJECT
};

class sublcass_injectable_type1 : public injectable_type1
{
	Q_OBJECT
};

class dependency_resolver_test : public QObject
{
	Q_OBJECT

public:
	dependency_resolver_test();

private slots:
	void should_resolve_no_dependencies_when_no_objects_available();
	void should_resolve_all_dependencies();
	void should_resolve_available_dependencies();
	void should_resolve_available_dependencies_using_exact_matching();
	void should_resolve_available_dependencies_using_exact_matching_not_using_subclass();
	void should_resolve_available_dependencies_not_using_superclass();

private:
	type injectable_type1_type;
	type injectable_type2_type;
	type injectable_type3_type;
	type sublcass_injectable_type1_type;

};

dependency_resolver_test::dependency_resolver_test() :
	injectable_type1_type{std::addressof(injectable_type1::staticMetaObject)},
	injectable_type2_type{std::addressof(injectable_type2::staticMetaObject)},
	injectable_type3_type{std::addressof(injectable_type3::staticMetaObject)},
	sublcass_injectable_type1_type{std::addressof(sublcass_injectable_type1::staticMetaObject)}
{
}

void dependency_resolver_test::should_resolve_no_dependencies_when_no_objects_available()
{
	auto object1 = make_object_with_meta<injectable_type1>();
	auto object2 = make_object_with_meta<injectable_type2>();
	auto object3 = make_object_with_meta<injectable_type3>();
	auto objects = std::vector<const object_with_meta *>{};
	auto to_resolve = std::vector<dependency>
	{
		{ injectable_type1_type, {} },
		{ injectable_type2_type, {} },
		{ injectable_type3_type, {} }
	};

	auto result = dependency_resolver{}.resolve_dependencies(dependencies{to_resolve}, objects);
	QVERIFY(result.resolved.empty());
	QCOMPARE(result.unresolved, dependencies{to_resolve});
}

void dependency_resolver_test::should_resolve_all_dependencies()
{
	auto object1 = make_object_with_meta<injectable_type1>();
	auto object2 = make_object_with_meta<injectable_type2>();
	auto object3 = make_object_with_meta<injectable_type3>();
	auto objects = std::vector<const object_with_meta *>
	{
		std::addressof(object1),
		std::addressof(object2),
		std::addressof(object3),
	};
	auto to_resolve = std::vector<dependency>
	{
		{ injectable_type1_type, {} },
		{ injectable_type2_type, {} },
		{ injectable_type3_type, {} }
	};

	auto result = dependency_resolver{}.resolve_dependencies(dependencies{to_resolve}, objects);
	QCOMPARE(result.resolved.size(), 3ul);
	QCOMPARE(result.resolved.at(0), (resolved_dependency{to_resolve.at(0), object1}));
	QCOMPARE(result.resolved.at(1), (resolved_dependency{to_resolve.at(1), object2}));
	QCOMPARE(result.resolved.at(2), (resolved_dependency{to_resolve.at(2), object3}));
	QVERIFY(result.unresolved.empty());
}

void dependency_resolver_test::should_resolve_available_dependencies()
{
	auto object1 = make_object_with_meta<injectable_type1>();
	auto object3 = make_object_with_meta<injectable_type3>();
	auto objects = std::vector<const object_with_meta *>
	{
		std::addressof(object1),
		std::addressof(object3),
	};
	auto to_resolve = std::vector<dependency>
	{
		{ injectable_type1_type, {} },
		{ injectable_type2_type, {} },
		{ injectable_type3_type, {} }
	};

	auto result = dependency_resolver{}.resolve_dependencies(dependencies{to_resolve}, objects);
	QCOMPARE(result.resolved.size(), 2ul);
	QCOMPARE(result.resolved.at(0), (resolved_dependency{to_resolve.at(0), object1}));
	QCOMPARE(result.resolved.at(1), (resolved_dependency{to_resolve.at(2), object3}));
	QCOMPARE(result.unresolved.size(), 1ul);
	QVERIFY(result.unresolved.contains(to_resolve.at(1)));
}

void dependency_resolver_test::should_resolve_available_dependencies_using_exact_matching()
{
	auto object1 = make_object_with_meta<injectable_type1>();
	auto object1b = make_object_with_meta<injectable_type1>();
	auto object3 = make_object_with_meta<injectable_type3>();
	auto object3b = make_object_with_meta<injectable_type3>();
	auto objects = std::vector<const object_with_meta *>
	{
		std::addressof(object1),
		std::addressof(object1b),
		std::addressof(object3),
		std::addressof(object3b),
	};
	auto to_resolve = std::vector<dependency>
	{
		{ injectable_type1_type, {} },
		{ injectable_type2_type, {} },
		{ injectable_type3_type, {} }
	};

	auto result = dependency_resolver{}.resolve_dependencies(dependencies{to_resolve}, objects);
	QCOMPARE(result.resolved.size(), 2ul);
	QCOMPARE(result.resolved.at(0), (resolved_dependency{to_resolve.at(0), object1}));
	QCOMPARE(result.resolved.at(1), (resolved_dependency{to_resolve.at(2), object3}));
	QCOMPARE(result.unresolved.size(), 1ul);
	QCOMPARE(*result.unresolved.begin(), to_resolve.at(1));
	QVERIFY(std::addressof(object1) != std::addressof(object1b));
	QVERIFY(std::addressof(object3) != std::addressof(object3b));
}

void dependency_resolver_test::should_resolve_available_dependencies_using_exact_matching_not_using_subclass()
{
	auto object1 = make_object_with_meta<injectable_type1>();
	auto subobject1 = make_object_with_meta<sublcass_injectable_type1>();
	auto object3 = make_object_with_meta<injectable_type3>();
	auto object3b = make_object_with_meta<injectable_type3>();
	auto objects = std::vector<const object_with_meta *>
	{
		std::addressof(subobject1),
		std::addressof(object1),
		std::addressof(object3),
		std::addressof(object3b),
	};
	auto to_resolve = std::vector<dependency>
	{
		{ injectable_type1_type, {} },
		{ injectable_type2_type, {} },
		{ injectable_type3_type, {} }
	};

	auto result = dependency_resolver{}.resolve_dependencies(dependencies{to_resolve}, objects);
	QCOMPARE(result.resolved.size(), 2ul);
	QCOMPARE(result.resolved.at(0), (resolved_dependency{to_resolve.at(0), object1}));
	QCOMPARE(result.resolved.at(1), (resolved_dependency{to_resolve.at(2), object3}));
	QCOMPARE(result.unresolved.size(), 1ul);
	QVERIFY(result.unresolved.contains(to_resolve.at(1)));
	QVERIFY(std::addressof(object1) != std::addressof(subobject1));
	QVERIFY(std::addressof(object3) != std::addressof(object3b));
}

void dependency_resolver_test::should_resolve_available_dependencies_not_using_superclass()
{
	auto object1 = make_object_with_meta<injectable_type1>();
	auto object2 = make_object_with_meta<injectable_type2>();
	auto objects = std::vector<const object_with_meta *>
	{
		std::addressof(object1),
		std::addressof(object2)
	};
	auto to_resolve = std::vector<dependency>
	{
		{ sublcass_injectable_type1_type, {} },
		{ injectable_type2_type, {} }
	};

	auto result = dependency_resolver{}.resolve_dependencies(dependencies{to_resolve}, objects);
	QCOMPARE(result.resolved.size(), 1ul);
	QCOMPARE(result.resolved.at(0), (resolved_dependency{to_resolve.at(1), object2}));
	QCOMPARE(result.unresolved.size(), 1ul);
	QVERIFY(result.unresolved.contains(to_resolve.at(0)));
}

QTEST_APPLESS_MAIN(dependency_resolver_test);

#include "dependency-resolver-test.moc"
