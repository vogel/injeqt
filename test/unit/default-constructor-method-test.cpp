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
#include "type.cpp"

#include "expect.h"
#include "utils.h"

#include <QtTest/QtTest>
#include <string>

using namespace injeqt::v1;

class no_default_constructor : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE no_default_constructor(int a) { (void)a; }
	Q_INVOKABLE no_default_constructor(std::string a = std::string{}) { (void)a; }

public slots:
	void regular_method() {}

};

class default_not_invokable_constructor : public QObject
{
	Q_OBJECT

public:
	default_not_invokable_constructor() {}

};

class default_invokable_constructor : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE default_invokable_constructor() {}

};

class default_constructor_method_test : public QObject
{
	Q_OBJECT

private slots:
	void should_throw_when_created_with_empty_method();
	void should_throw_when_created_with_regular_method();
	void should_throw_when_created_with_no_default_constructor();
	void should_throw_when_created_with_not_invokable_constructor();
	void should_create_valid_with_invokable_constructor();
	void should_create_object_with_default_constructor();

};

void default_constructor_method_test::should_throw_when_created_with_empty_method()
{
	expect<invalid_default_constructor_exception>([&]{
		default_constructor_method{QMetaMethod{}};
	});
}

void default_constructor_method_test::should_throw_when_created_with_regular_method()
{
	expect<invalid_default_constructor_exception>([&]{
		default_constructor_method{make_method<no_default_constructor>("regular_method()")};
	});
}

void default_constructor_method_test::should_throw_when_created_with_no_default_constructor()
{
	expect<invalid_default_constructor_exception>([&]{
		default_constructor_method{make_constructor<no_default_constructor>("no_default_constructor(int)")};
	});
	expect<invalid_default_constructor_exception>([&]{
		default_constructor_method{make_constructor<no_default_constructor>("no_default_constructor(std::string)")};
	});
}

void default_constructor_method_test::should_throw_when_created_with_not_invokable_constructor()
{
	expect<constructor_not_found_exception>([&]{
		default_constructor_method{make_constructor<default_not_invokable_constructor>("default_not_invokable_constructor()")};
	});
	expect<no_default_constructor_exception>([&]{
		make_default_constructor_method<default_not_invokable_constructor>();
	});
	expect<no_default_constructor_exception>([&]{
		make_default_constructor_method(make_type<default_not_invokable_constructor>());
	});
}

void default_constructor_method_test::should_create_valid_with_invokable_constructor()
{
	auto c1 = default_constructor_method{make_constructor<default_invokable_constructor>("default_invokable_constructor()")};
	QCOMPARE(c1.object_type(), make_type<default_invokable_constructor>());

	auto c2 = make_default_constructor_method<default_invokable_constructor>();
	QCOMPARE(c2.object_type(), make_type<default_invokable_constructor>());

	auto c3 = make_default_constructor_method(make_type<default_invokable_constructor>());
	QCOMPARE(c3.object_type(), make_type<default_invokable_constructor>());

	QCOMPARE(c1, c2);
	QCOMPARE(c2, c3);
	QCOMPARE(c1, c3);
}

void default_constructor_method_test::should_create_object_with_default_constructor()
{
	auto constructor = make_default_constructor_method(make_type<default_invokable_constructor>());
	auto object = constructor.invoke();
	auto cast = qobject_cast<default_invokable_constructor *>(object.get());

	QVERIFY(cast != nullptr);
}

QTEST_APPLESS_MAIN(default_constructor_method_test)
#include "default-constructor-method-test.moc"
