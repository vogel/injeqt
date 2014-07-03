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
#include "dependency-resolver.cpp"
#include "implements-extractor.cpp"
#include "injeqt-object.cpp"
#include "meta-object.cpp"
#include "meta-object-factory.cpp"
#include "expect.h"

#include <QtTest/QtTest>
#include <memory>

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

class valid_injected_type : public QObject
{
	Q_OBJECT

public:
	injectable_type1 *i1 = nullptr;
	injectable_type2 *i2 = nullptr;

public slots:
	injeqt_setter void injeqtSetter1(injectable_type1 *v) { i1 = v; }
	injeqt_setter void injeqtSetter2(injectable_type2 *v) { i2 = v; }

};

class inheriting_valid_injected_type : public valid_injected_type
{
	Q_OBJECT

public:
	injectable_type3 *i3 = nullptr;

public slots:
	injeqt_setter void injeqtSetter3(injectable_type3 *v) { i3 = v; }

};

class dependency_resolver_test : public QObject
{
	Q_OBJECT

private slots:
	void should_properly_resolve_one_dependency();
	void should_properly_resolve_all_objects();
	void should_properly_resolve_one_dependency_in_hierarchy();
	void should_properly_resolve_all_objects_in_hierarchy();

private:
	template<typename T>
	injeqt_object make_injeqt_object();
	std::vector<dependency> create_dependencies(bool include_inheriting) const;

};

template<typename T>
injeqt_object dependency_resolver_test::make_injeqt_object()
{
	auto qobject = std::unique_ptr<QObject>(new T{});
	auto object = injeqt_object
	{
		meta_object_factory{}.create_meta_object(T::staticMetaObject),
		std::move(qobject)
	};

	return object;
}

std::vector<dependency> dependency_resolver_test::create_dependencies(bool include_inheriting) const
{
	auto result = std::vector<dependency>{};
	result.emplace_back(
		dependency_apply_method::setter,
		injectable_type1::staticMetaObject,
		valid_injected_type::staticMetaObject.method(valid_injected_type::staticMetaObject.indexOfMethod("injeqtSetter1(injectable_type1*)"))
	);
	result.emplace_back(
		dependency_apply_method::setter,
		injectable_type2::staticMetaObject,
		valid_injected_type::staticMetaObject.method(valid_injected_type::staticMetaObject.indexOfMethod("injeqtSetter2(injectable_type2*)"))
	);

	if (include_inheriting)
		result.emplace_back(
			dependency_apply_method::setter,
			injectable_type3::staticMetaObject,
			inheriting_valid_injected_type::staticMetaObject.method(inheriting_valid_injected_type::staticMetaObject.indexOfMethod("injeqtSetter3(injectable_type3*)"))
		);

	return result;
}

void dependency_resolver_test::should_properly_resolve_one_dependency()
{
	auto injected = make_injeqt_object<valid_injected_type>();
	auto objects = std::vector<injeqt_object>{};
	objects.emplace_back(make_injeqt_object<injectable_type1>());
	auto dependencies = create_dependencies(false);

	auto unresolved = dependency_resolver{}.resolve_dependencies(injected, dependencies, objects);
	QCOMPARE(unresolved.size(), 1UL);
	QCOMPARE(unresolved[0], dependencies[1]);
	QCOMPARE(static_cast<valid_injected_type *>(injected.object())->i1, objects[0].object());
	QCOMPARE(static_cast<valid_injected_type *>(injected.object())->i2, static_cast<QObject *>(nullptr));
}

void dependency_resolver_test::should_properly_resolve_all_objects()
{
	auto injected = make_injeqt_object<valid_injected_type>();
	auto objects = std::vector<injeqt_object>{};
	objects.emplace_back(make_injeqt_object<injectable_type1>());
	objects.emplace_back(make_injeqt_object<injectable_type2>());
	auto dependencies = create_dependencies(false);

	auto unresolved = dependency_resolver{}.resolve_dependencies(injected, dependencies, objects);
	QVERIFY(unresolved.empty());
	QCOMPARE(static_cast<valid_injected_type *>(injected.object())->i1, objects[0].object());
	QCOMPARE(static_cast<valid_injected_type *>(injected.object())->i2, objects[1].object());
}

void dependency_resolver_test::should_properly_resolve_one_dependency_in_hierarchy()
{
	auto injected = make_injeqt_object<inheriting_valid_injected_type>();
	auto objects = std::vector<injeqt_object>{};
	objects.emplace_back(make_injeqt_object<injectable_type3>());
	auto dependencies = create_dependencies(true);

	auto unresolved = dependency_resolver{}.resolve_dependencies(injected, dependencies, objects);
	QCOMPARE(unresolved.size(), 2UL);
	QCOMPARE(unresolved[0], dependencies[0]);
	QCOMPARE(unresolved[1], dependencies[1]);
	QCOMPARE(static_cast<inheriting_valid_injected_type *>(injected.object())->i1, static_cast<QObject *>(nullptr));
	QCOMPARE(static_cast<inheriting_valid_injected_type *>(injected.object())->i2, static_cast<QObject *>(nullptr));
	QCOMPARE(static_cast<inheriting_valid_injected_type *>(injected.object())->i3, objects[0].object());
}

void dependency_resolver_test::should_properly_resolve_all_objects_in_hierarchy()
{
	auto injected = make_injeqt_object<inheriting_valid_injected_type>();
	auto objects = std::vector<injeqt_object>{};
	objects.emplace_back(make_injeqt_object<injectable_type1>());
	objects.emplace_back(make_injeqt_object<injectable_type2>());
	objects.emplace_back(make_injeqt_object<injectable_type3>());
	auto dependencies = create_dependencies(true);

	auto unresolved = dependency_resolver{}.resolve_dependencies(injected, dependencies, objects);
	QVERIFY(unresolved.empty());
	QCOMPARE(static_cast<inheriting_valid_injected_type *>(injected.object())->i1, objects[0].object());
	QCOMPARE(static_cast<inheriting_valid_injected_type *>(injected.object())->i2, objects[1].object());
	QCOMPARE(static_cast<inheriting_valid_injected_type *>(injected.object())->i3, objects[2].object());
}

QTEST_APPLESS_MAIN(dependency_resolver_test);

#include "dependency-resolver-test.moc"
