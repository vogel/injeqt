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

#include "exception/exception.cpp"
#include "factory-method.cpp"
#include "interfaces-utils.cpp"
#include "type.cpp"

#include "expect.h"
#include "utils.h"

#include <QtTest/QtTest>
#include <string>

using namespace injeqt::internal;
using namespace injeqt::v1;

class result_object : public QObject
{
	Q_OBJECT
};

class other_object : public QObject
{
	Q_OBJECT
};

class factory_not_invokable : public QObject
{
	Q_OBJECT

public:
	result_object * create_result_object() { return nullptr; }

};

class non_unique_factory : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE result_object * create_result_object_1() { return nullptr; }
	Q_INVOKABLE result_object * create_result_object_2() { return nullptr; }

};

class valid_factory : public QObject
{
	Q_OBJECT

public slots:
	Q_INVOKABLE result_object * create_result_object() { return new result_object{}; }

};

class valid_factory_with_default_parameter : public QObject
{
	Q_OBJECT

public slots:
	Q_INVOKABLE result_object * create_result_object(int a = 0) { (void)a; return new result_object{}; }

};

class invalid_factory : public QObject
{
	Q_OBJECT

public slots:
	Q_INVOKABLE result_object * create_result_object(int) { return new result_object{}; }

};

class factory_method_test : public QObject
{
	Q_OBJECT

private slots:
	void should_return_empty_when_created_with_not_invokable_factory_method();
	void should_return_empty_when_created_with_non_unique_factory_method();
	void should_return_empty_when_created_with_different_type_factory_method();
	void should_return_empty_when_created_with_invalid_factory_method();
	void should_create_empty();
	void should_create_valid_with_invokable_factory_method();
	void should_create_valid_with_invokable_factory_with_default_parameter_method();
	void should_create_object_with_factory_method();

};

void factory_method_test::should_return_empty_when_created_with_not_invokable_factory_method()
{
	auto f = make_factory_method(make_type<result_object>(), make_type<factory_not_invokable>());
	QVERIFY(f.is_empty());
}

void factory_method_test::should_return_empty_when_created_with_non_unique_factory_method()
{
	auto f = make_factory_method(make_type<result_object>(), make_type<non_unique_factory>());
	QVERIFY(f.is_empty());
}

void factory_method_test::should_return_empty_when_created_with_different_type_factory_method()
{
	auto f = make_factory_method(make_type<other_object>(), make_type<valid_factory>());
	QVERIFY(f.is_empty());
}

void factory_method_test::should_return_empty_when_created_with_invalid_factory_method()
{
	auto f = make_factory_method(make_type<other_object>(), make_type<invalid_factory>());
	QVERIFY(f.is_empty());
}

void factory_method_test::should_create_empty()
{
	auto f = factory_method{};
	QVERIFY(f.is_empty());
}

void factory_method_test::should_create_valid_with_invokable_factory_method()
{
	auto f1 = factory_method{make_method<valid_factory>("create_result_object()")};
	QVERIFY(!f1.is_empty());
	QCOMPARE(f1.object_type(), make_type<valid_factory>());
	QCOMPARE(f1.result_type(), make_type<result_object>());

	auto f2 = make_factory_method(make_type<result_object>(), make_type<valid_factory>());
	QVERIFY(!f2.is_empty());
	QCOMPARE(f2.object_type(), make_type<valid_factory>());
	QCOMPARE(f2.result_type(), make_type<result_object>());

	QCOMPARE(f1, f2);
}

void factory_method_test::should_create_valid_with_invokable_factory_with_default_parameter_method()
{
	auto f = make_factory_method(make_type<result_object>(), make_type<valid_factory_with_default_parameter>());
	QVERIFY(!f.is_empty());
	QCOMPARE(f.object_type(), make_type<valid_factory_with_default_parameter>());
	QCOMPARE(f.result_type(), make_type<result_object>());
}

void factory_method_test::should_create_object_with_factory_method()
{
	auto f = make_factory_method(make_type<result_object>(), make_type<valid_factory>());
	auto factory_object = make_object<valid_factory>();
	auto object = f.invoke(factory_object.get());
	auto cast = qobject_cast<result_object *>(object.get());
	QVERIFY(cast != nullptr);
}

QTEST_APPLESS_MAIN(factory_method_test)
#include "factory-method-test.moc"
