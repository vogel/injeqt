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
#include "dependency.cpp"
#include "implementation.cpp"
#include "interfaces-utils.cpp"
#include "resolved-dependency.cpp"
#include "resolve-dependencies.cpp"
#include "setter-method.cpp"
#include "type.cpp"

#include "make-setter-method.h"
#include "utils.h"

#include <QtTest/QtTest>

using namespace injeqt::internal;
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

class valid_type : public QObject
{
	Q_OBJECT

public slots:
	void set_type1(injectable_type1 *) {}
	void set_type2(injectable_type2 *) {}
	void set_type3(injectable_type3 *) {}
	void set_sub_type1(sublcass_injectable_type1 *) {}
};

class resolve_dependencies_test : public QObject
{
	Q_OBJECT

public:
	resolve_dependencies_test();

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
	setter_method injectable_type1_setter;
	setter_method injectable_type2_setter;
	setter_method injectable_type3_setter;
	setter_method subclass_injectable_type1_setter;

};

resolve_dependencies_test::resolve_dependencies_test() :
	injectable_type1_type{make_type<injectable_type1>()},
	injectable_type2_type{make_type<injectable_type2>()},
	injectable_type3_type{make_type<injectable_type3>()},
	sublcass_injectable_type1_type{make_type<sublcass_injectable_type1>()},
	injectable_type1_setter{make_setter_method<valid_type, injectable_type1>("set_type1(injectable_type1*)")},
	injectable_type2_setter{make_setter_method<valid_type, injectable_type2>("set_type2(injectable_type2*)")},
	injectable_type3_setter{make_setter_method<valid_type, injectable_type3>("set_type3(injectable_type3*)")},
	subclass_injectable_type1_setter{make_setter_method<valid_type, sublcass_injectable_type1>("set_sub_type1(sublcass_injectable_type1*)")}
{
}

void resolve_dependencies_test::should_resolve_no_dependencies_when_no_objects_available()
{
	auto objects = std::vector<implementation>{};
	auto to_resolve = std::vector<dependency>
	{
		dependency{injectable_type1_setter},
		dependency{injectable_type2_setter},
		dependency{injectable_type3_setter}
	};

	auto result = resolve_dependencies(dependencies{to_resolve}, implementations{objects});
	QVERIFY(result.resolved.empty());
	QCOMPARE(result.unresolved, dependencies{to_resolve});
}

void resolve_dependencies_test::should_resolve_all_dependencies()
{
	auto object1 = make_object<injectable_type1>();
	auto object2 = make_object<injectable_type2>();
	auto object3 = make_object<injectable_type3>();
	auto objects = std::vector<implementation>
	{
		implementation{injectable_type1_type, object1.get()},
		implementation{injectable_type2_type, object2.get()},
		implementation{injectable_type3_type, object3.get()},
	};
	auto to_resolve = std::vector<dependency>
	{
		dependency{injectable_type1_setter},
		dependency{injectable_type2_setter},
		dependency{injectable_type3_setter}
	};

	auto result = resolve_dependencies(dependencies{to_resolve}, implementations{objects});
	QCOMPARE(result.resolved.size(), size_t{3});
	QCOMPARE(result.resolved.at(0), (resolved_dependency{objects.at(0), injectable_type1_setter}));
	QCOMPARE(result.resolved.at(1), (resolved_dependency{objects.at(1), injectable_type2_setter}));
	QCOMPARE(result.resolved.at(2), (resolved_dependency{objects.at(2), injectable_type3_setter}));
	QVERIFY(result.unresolved.empty());
}

void resolve_dependencies_test::should_resolve_available_dependencies()
{
	auto object1 = make_object<injectable_type1>();
	auto object3 = make_object<injectable_type3>();
	auto objects = std::vector<implementation>
	{
		implementation{injectable_type1_type, object1.get()},
		implementation{injectable_type3_type, object3.get()}
	};
	auto to_resolve = std::vector<dependency>
	{
		dependency{injectable_type1_setter},
		dependency{injectable_type2_setter},
		dependency{injectable_type3_setter}
	};

	auto result = resolve_dependencies(dependencies{to_resolve}, implementations{objects});
	QCOMPARE(result.resolved.size(), size_t{2});
	QCOMPARE(result.resolved.at(0), (resolved_dependency{objects.at(0), injectable_type1_setter}));
	QCOMPARE(result.resolved.at(1), (resolved_dependency{objects.at(1), injectable_type3_setter}));
	QCOMPARE(result.unresolved.size(), size_t{1});
	QVERIFY(result.unresolved.contains(to_resolve.at(1)));
}

void resolve_dependencies_test::should_resolve_available_dependencies_using_exact_matching()
{
	auto object1 = make_object<injectable_type1>();
	auto object1b = make_object<injectable_type1>();
	auto object3 = make_object<injectable_type3>();
	auto object3b = make_object<injectable_type3>();
	auto objects = std::vector<implementation>
	{
		implementation{injectable_type1_type, object1.get()},
		implementation{injectable_type1_type, object1b.get()},
		implementation{injectable_type3_type, object3.get()},
		implementation{injectable_type3_type, object3b.get()},
	};
	auto to_resolve = std::vector<dependency>
	{
		dependency{injectable_type1_setter},
		dependency{injectable_type2_setter},
		dependency{injectable_type3_setter}
	};

	auto result = resolve_dependencies(dependencies{to_resolve}, implementations{objects});
	QCOMPARE(result.resolved.size(), size_t{2});
	QCOMPARE(result.resolved.at(0), (resolved_dependency{objects.at(0), injectable_type1_setter}));
	QCOMPARE(result.resolved.at(1), (resolved_dependency{objects.at(2), injectable_type3_setter}));
	QCOMPARE(result.unresolved.size(), size_t{1});
	QCOMPARE(*result.unresolved.begin(), to_resolve.at(1));
	QVERIFY(&object1 != &object1b);
	QVERIFY(&object3 != &object3b);
}

void resolve_dependencies_test::should_resolve_available_dependencies_using_exact_matching_not_using_subclass()
{
	auto object1 = make_object<injectable_type1>();
	auto subobject1 = make_object<sublcass_injectable_type1>();
	auto object3 = make_object<injectable_type3>();
	auto object3b = make_object<injectable_type3>();
	auto objects = std::vector<implementation>
	{
		implementation{injectable_type1_type, object1.get()},
		implementation{sublcass_injectable_type1_type, subobject1.get()},
		implementation{injectable_type3_type, object3.get()},
		implementation{injectable_type3_type, object3b.get()}
	};
	auto to_resolve = std::vector<dependency>
	{
		dependency{injectable_type1_setter},
		dependency{injectable_type2_setter},
		dependency{injectable_type3_setter}
	};

	auto result = resolve_dependencies(dependencies{to_resolve}, implementations{objects});
	QCOMPARE(result.resolved.size(), size_t{2});
	QCOMPARE(result.resolved.at(0), (resolved_dependency{objects.at(0), injectable_type1_setter}));
	QCOMPARE(result.resolved.at(1), (resolved_dependency{objects.at(2), injectable_type3_setter}));
	QCOMPARE(result.unresolved.size(), size_t{1});
	QVERIFY(result.unresolved.contains(to_resolve.at(1)));
	QVERIFY(&object1 != &subobject1);
	QVERIFY(&object3 != &object3b);
}

void resolve_dependencies_test::should_resolve_available_dependencies_not_using_superclass()
{
	auto object1 = make_object<injectable_type1>();
	auto object2 = make_object<injectable_type2>();
	auto objects = std::vector<implementation>
	{
		implementation{injectable_type1_type, object1.get()},
		implementation{injectable_type2_type, object2.get()}
	};
	auto to_resolve = std::vector<dependency>
	{
		dependency{subclass_injectable_type1_setter},
		dependency{injectable_type2_setter}
	};

	auto result = resolve_dependencies(dependencies{to_resolve}, implementations{objects});
	QCOMPARE(result.resolved.size(), size_t{1});
	QCOMPARE(result.resolved.at(0), (resolved_dependency{objects.at(1), injectable_type2_setter}));
	QCOMPARE(result.unresolved.size(), size_t{1});
	QVERIFY(result.unresolved.contains(to_resolve.at(0)));
}

QTEST_APPLESS_MAIN(resolve_dependencies_test);

#include "resolve-dependencies-test.moc"
