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
#include "dependency-apply-method.h"
#include "dependency-extractor.cpp"
#include "expect.h"

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

class non_qobject_invalid_injected_type : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void injeqtSetter1(int) {}

};

class duplicate_dependency_invalid_injected_type : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void injeqtSetter1(injectable_type1 *) {}
	injeqt_setter void injeqtSetter2(injectable_type1 *) {}

};

class dependency_extractor_test : public QObject
{
	Q_OBJECT

private slots:
	void should_find_all_valid_dependencies();
	void should_find_all_valid_dependencies_in_hierarchy();
	void should_fail_when_too_many_parameters();
	void should_fail_when_type_not_qobject();
	void should_fail_when_duplicate_dependency();

private:
	void verify_dependency(const std::map<const QMetaObject *, dependency> dependencies, const dependency &check);

};

void dependency_extractor_test::verify_dependency(const std::map< const QMetaObject *, dependency> dependencies, const dependency &to_verify)
{
	auto iterator = dependencies.find(std::addressof(to_verify.type()));
	QVERIFY(iterator != std::end(dependencies));
	QVERIFY(iterator->second == to_verify);
}

void dependency_extractor_test::should_find_all_valid_dependencies()
{
	auto dependencies = dependency_extractor{}.extract_dependencies(valid_injected_type::staticMetaObject);
	QCOMPARE(dependencies.size(), 2UL);
	verify_dependency(dependencies, {
		dependency_apply_method::setter,
		injectable_type1::staticMetaObject,
		valid_injected_type::staticMetaObject.method(valid_injected_type::staticMetaObject.indexOfMethod("injeqtSetter1(injectable_type1*)"))
	});
	verify_dependency(dependencies, {
		dependency_apply_method::setter,
		injectable_type2::staticMetaObject,
		valid_injected_type::staticMetaObject.method(valid_injected_type::staticMetaObject.indexOfMethod("injeqtSetter2(injectable_type2*)"))
	});
}

void dependency_extractor_test::should_find_all_valid_dependencies_in_hierarchy()
{
	auto dependencies = dependency_extractor{}.extract_dependencies(inheriting_valid_injected_type::staticMetaObject);
	QCOMPARE(dependencies.size(), 3UL);
	verify_dependency(dependencies, {
		dependency_apply_method::setter,
		injectable_type1::staticMetaObject,
		valid_injected_type::staticMetaObject.method(valid_injected_type::staticMetaObject.indexOfMethod("injeqtSetter1(injectable_type1*)"))
	});
	verify_dependency(dependencies, {
		dependency_apply_method::setter,
		injectable_type2::staticMetaObject,
		valid_injected_type::staticMetaObject.method(valid_injected_type::staticMetaObject.indexOfMethod("injeqtSetter2(injectable_type2*)"))
	});
	verify_dependency(dependencies, {
		dependency_apply_method::setter,
		injectable_type3::staticMetaObject,
		inheriting_valid_injected_type::staticMetaObject.method(inheriting_valid_injected_type::staticMetaObject.indexOfMethod("injeqtSetter3(injectable_type3*)"))
	});
}

void dependency_extractor_test::should_fail_when_too_many_parameters()
{
	expect<dependency_too_many_parameters_exception>([]{
		auto dependencies = dependency_extractor{}.extract_dependencies(too_many_parameters_invalid_injected_type::staticMetaObject);
	});
}

void dependency_extractor_test::should_fail_when_type_not_qobject()
{
	expect<dependency_not_qobject_exception>([]{
		auto dependencies = dependency_extractor{}.extract_dependencies(non_qobject_invalid_injected_type::staticMetaObject);
	});
}

void dependency_extractor_test::should_fail_when_duplicate_dependency()
{
	expect<dependency_duplicated_exception>([]{
		auto dependencies = dependency_extractor{}.extract_dependencies(duplicate_dependency_invalid_injected_type::staticMetaObject);
	});
}

QTEST_APPLESS_MAIN(dependency_extractor_test);

#include "dependency-extractor-test.moc"
