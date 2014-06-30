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
#include "dependency-type.h"
#include "expect.h"

#include <QtTest/QtTest>

using namespace injeqt;
using namespace injeqt::internal;

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

public slots:
	injeqt_setter void injeqtSetter1(injectable_type1 *) {}
	injeqt_setter void injeqtSetter2(injectable_type2 *) {}
	void noninjeqtSetter1(injectable_type1 *) {}
	void noninjeqtSetter2(injectable_type2 *) {}
	void noninjeqtSetter5(int) {}

};

class inheriting_valid_injected_type : public valid_injected_type
{
	Q_OBJECT

public slots:
	injeqt_setter void injeqtSetter3(injectable_type3 *) {}

};

class too_many_parameters_invalid_injected_type : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void injeqtSetter1(injectable_type1 *, injectable_type2 *) {}

};

class non_registered_invalid_injected_type : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void injeqtSetter1(int) {}

};

class test_dependency_extractor : public QObject
{
	Q_OBJECT

private slots:
	void should_find_all_valid_dependencies();
	void should_find_all_valid_dependencies_in_hierarchy();
	void should_fail_when_too_many_parameters();
	void should_fail_when_type_not_registered();

private:
	void verify_dependency(const std::map<const QMetaObject *, dependency> dependencies, const dependency &check);

};

void test_dependency_extractor::verify_dependency(const std::map< const QMetaObject *, dependency> dependencies, const dependency &to_verify)
{
	auto iterator = dependencies.find(std::addressof(to_verify.object()));
	QVERIFY(iterator != std::end(dependencies));
	QVERIFY(iterator->second == to_verify);
}

void test_dependency_extractor::should_find_all_valid_dependencies()
{
	auto dependencies = dependency_extractor{}.extract_dependencies(valid_injected_type::staticMetaObject);
	QCOMPARE(dependencies.size(), 2UL);
	verify_dependency(dependencies, {
		dependency_type::setter,
		injectable_type1::staticMetaObject,
		valid_injected_type::staticMetaObject.method(valid_injected_type::staticMetaObject.indexOfMethod("injeqtSetter1(injectable_type1*)"))
	});
	verify_dependency(dependencies, {
		dependency_type::setter,
		injectable_type2::staticMetaObject,
		valid_injected_type::staticMetaObject.method(valid_injected_type::staticMetaObject.indexOfMethod("injeqtSetter2(injectable_type2*)"))
	});
}

void test_dependency_extractor::should_find_all_valid_dependencies_in_hierarchy()
{
	auto dependencies = dependency_extractor{}.extract_dependencies(inheriting_valid_injected_type::staticMetaObject);
	QCOMPARE(dependencies.size(), 3UL);
	verify_dependency(dependencies, {
		dependency_type::setter,
		injectable_type1::staticMetaObject,
		valid_injected_type::staticMetaObject.method(valid_injected_type::staticMetaObject.indexOfMethod("injeqtSetter1(injectable_type1*)"))
	});
	verify_dependency(dependencies, {
		dependency_type::setter,
		injectable_type2::staticMetaObject,
		valid_injected_type::staticMetaObject.method(valid_injected_type::staticMetaObject.indexOfMethod("injeqtSetter2(injectable_type2*)"))
	});
	verify_dependency(dependencies, {
		dependency_type::setter,
		injectable_type3::staticMetaObject,
		inheriting_valid_injected_type::staticMetaObject.method(inheriting_valid_injected_type::staticMetaObject.indexOfMethod("injeqtSetter3(injectable_type3*)"))
	});
}

void test_dependency_extractor::should_fail_when_too_many_parameters()
{
	expect<invalid_dependency>([]{
		auto dependencies = dependency_extractor{}.extract_dependencies(too_many_parameters_invalid_injected_type::staticMetaObject);
	});
}

void test_dependency_extractor::should_fail_when_type_not_registered()
{
	expect<invalid_dependency>([]{
		auto dependencies = dependency_extractor{}.extract_dependencies(non_registered_invalid_injected_type::staticMetaObject);
	});
}

QTEST_APPLESS_MAIN(test_dependency_extractor);

#include "dependency-extractor-test.moc"
