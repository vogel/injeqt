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
#include "dependency-resolver.cpp"
#include "implements-extractor.cpp"
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

class test_dependency_resolver : public QObject
{
	Q_OBJECT

private slots:
	void should_properly_resolver_one_dependency();
	void should_properly_resolver_all_dependencies();
	void should_properly_resolver_one_dependency_in_hierarchy();
	void should_properly_resolver_all_dependencies_in_hierarchy();

private:
	std::map<const QMetaObject *, dependency> create_dependency_map(bool include_inheriting) const;

};

std::map<const QMetaObject *, dependency> test_dependency_resolver::create_dependency_map(bool include_inheriting) const
{
	auto result = std::map<const QMetaObject *, dependency>{};
	result.emplace(std::addressof(injectable_type1::staticMetaObject), dependency{
		dependency_type::setter,
		injectable_type1::staticMetaObject,
		valid_injected_type::staticMetaObject.method(valid_injected_type::staticMetaObject.indexOfMethod("injeqtSetter1(injectable_type1*)"))
	});
	result.emplace(std::addressof(injectable_type2::staticMetaObject), dependency{
		dependency_type::setter,
		injectable_type2::staticMetaObject,
		valid_injected_type::staticMetaObject.method(valid_injected_type::staticMetaObject.indexOfMethod("injeqtSetter2(injectable_type2*)"))
	});

	if (include_inheriting)
		result.emplace(std::addressof(injectable_type3::staticMetaObject), dependency{
			dependency_type::setter,
			injectable_type3::staticMetaObject,
			inheriting_valid_injected_type::staticMetaObject.method(inheriting_valid_injected_type::staticMetaObject.indexOfMethod("injeqtSetter3(injectable_type3*)"))
		});

	return result;
}

void test_dependency_resolver::should_properly_resolver_one_dependency()
{
	auto injected = std::unique_ptr<valid_injected_type>(new valid_injected_type{});
	auto dependency_one = std::unique_ptr<QObject>(new injectable_type1{});
	auto dependencies = std::vector<QObject *>{dependency_one.get()};
	auto dependency_map = create_dependency_map(false);

	auto unresolved = dependency_resolver{}.resolve_dependencies(*injected.get(), dependency_map, dependencies);
	QCOMPARE(unresolved.size(), 1UL);
	QCOMPARE(unresolved.begin()->first, std::addressof(injectable_type2::staticMetaObject));
	QCOMPARE(unresolved.begin()->second, dependency_map.find(std::addressof(injectable_type2::staticMetaObject))->second);
	QCOMPARE(injected->i1, dependency_one.get());
	QCOMPARE(injected->i2, static_cast<QObject *>(nullptr));
}

void test_dependency_resolver::should_properly_resolver_all_dependencies()
{
	auto injected = std::unique_ptr<valid_injected_type>(new valid_injected_type{});
	auto dependency_one = std::unique_ptr<QObject>(new injectable_type1{});
	auto dependency_two = std::unique_ptr<QObject>(new injectable_type2{});
	auto dependencies = std::vector<QObject *>{dependency_one.get(), dependency_two.get()};
	auto dependency_map = create_dependency_map(false);

	auto unresolved = dependency_resolver{}.resolve_dependencies(*injected.get(), dependency_map, dependencies);
	QVERIFY(unresolved.empty());
	QCOMPARE(injected->i1, dependency_one.get());
	QCOMPARE(injected->i2, dependency_two.get());
}

void test_dependency_resolver::should_properly_resolver_one_dependency_in_hierarchy()
{
	auto injected = std::unique_ptr<inheriting_valid_injected_type>(new inheriting_valid_injected_type{});
	auto dependency_three = std::unique_ptr<QObject>(new injectable_type3{});
	auto dependencies = std::vector<QObject *>{dependency_three.get()};
	auto dependency_map = create_dependency_map(true);

	auto unresolved = dependency_resolver{}.resolve_dependencies(*injected.get(), dependency_map, dependencies);
	QCOMPARE(unresolved.size(), 2UL);
	QVERIFY(unresolved.find(std::addressof(injectable_type1::staticMetaObject)) != std::end(unresolved));
	QCOMPARE(unresolved.find(std::addressof(injectable_type1::staticMetaObject))->second,
			 dependency_map.find(std::addressof(injectable_type1::staticMetaObject))->second);
	QVERIFY(unresolved.find(std::addressof(injectable_type2::staticMetaObject)) != std::end(unresolved));
	QCOMPARE(unresolved.find(std::addressof(injectable_type2::staticMetaObject))->second,
			 dependency_map.find(std::addressof(injectable_type2::staticMetaObject))->second);
	QCOMPARE(injected->i1, static_cast<QObject *>(nullptr));
	QCOMPARE(injected->i2, static_cast<QObject *>(nullptr));
	QCOMPARE(injected->i3, dependency_three.get());
}

void test_dependency_resolver::should_properly_resolver_all_dependencies_in_hierarchy()
{
	auto injected = std::unique_ptr<inheriting_valid_injected_type>(new inheriting_valid_injected_type{});
	auto dependency_one = std::unique_ptr<QObject>(new injectable_type1{});
	auto dependency_two = std::unique_ptr<QObject>(new injectable_type2{});
	auto dependency_three = std::unique_ptr<QObject>(new injectable_type3{});
	auto dependencies = std::vector<QObject *>{dependency_one.get(), dependency_two.get(), dependency_three.get()};
	auto dependency_map = create_dependency_map(true);

	auto unresolved = dependency_resolver{}.resolve_dependencies(*injected.get(), dependency_map, dependencies);
	QVERIFY(unresolved.empty());
	QCOMPARE(injected->i1, dependency_one.get());
	QCOMPARE(injected->i2, dependency_two.get());
	QCOMPARE(injected->i3, dependency_three.get());
}

QTEST_APPLESS_MAIN(test_dependency_resolver);

#include "dependency-resolver-test.moc"
