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

#include "expect.h"
#include "object-with-meta.cpp"
#include "setter-method.cpp"
#include "type.cpp"

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

class test_type : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void tagged_setter_1(injectable_type1 *) {}
	injeqt_setter void tagged_int_setter(int) {}
	injeqt_setter void tagged_invalid_setter(injectable_type1 *, injectable_type2 *) {}
	void untagged_setter_1(injectable_type1 *) {}
	void untagged_int_setter(int) {}
	void untagged_invalid_setter(injectable_type1 *, injectable_type2 *) {}

};

class setter_method_test : public QObject
{
	Q_OBJECT

public:
	setter_method_test();

private slots:
	void should_throw_when_created_with_empty_method();
	void should_create_valid_from_tagged_setter();
	void should_create_valid_from_untagged_setter();
	void should_throw_when_created_from_tagged_int_setter();
	void should_throw_when_created_from_untagged_int_setter();
	void should_throw_when_created_from_tagged_invalid_setter();
	void should_throw_when_created_from_untagged_invalid_setter();

private:
	type injectable_type1_type;
	type injectable_type2_type;
	type test_type_type;

	QMetaMethod method(const char *signature);

};

setter_method_test::setter_method_test() :
	injectable_type1_type{std::addressof(injectable_type1::staticMetaObject)},
	injectable_type2_type{std::addressof(injectable_type2::staticMetaObject)},
	test_type_type{std::addressof(test_type::staticMetaObject)}
{
}

QMetaMethod setter_method_test::method(const char *signature)
{
	return test_type::staticMetaObject.method(test_type::staticMetaObject.indexOfMethod(signature));
}

void setter_method_test::should_throw_when_created_with_empty_method()
{
	expect<invalid_setter_exception>([]{
		auto setter = setter_method{{}};
	});
}

void setter_method_test::should_create_valid_from_tagged_setter()
{
	auto setter = setter_method{method("tagged_setter_1(injectable_type1*)")};

	QCOMPARE(setter.object_type(), test_type_type);
	QCOMPARE(setter.parameter_type(), injectable_type1_type);
}

void setter_method_test::should_create_valid_from_untagged_setter()
{
	auto setter = setter_method{method("untagged_setter_1(injectable_type1*)")};

	QCOMPARE(setter.object_type(), test_type_type);
	QCOMPARE(setter.parameter_type(), injectable_type1_type);
}

void setter_method_test::should_throw_when_created_from_tagged_int_setter()
{
	expect<invalid_setter_exception>([&]{
		auto setter = setter_method{method("tagged_int_setter(int)")};
	});
}

void setter_method_test::should_throw_when_created_from_untagged_int_setter()
{
	expect<invalid_setter_exception>([&]{
		auto setter = setter_method{method("untagged_int_setter(int)")};
	});
}

void setter_method_test::should_throw_when_created_from_tagged_invalid_setter()
{
	expect<setter_too_many_parameters_exception>([&]{
		auto setter = setter_method{method("tagged_invalid_setter(injectable_type1*,injectable_type2*)")};
	});
}

void setter_method_test::should_throw_when_created_from_untagged_invalid_setter()
{
	expect<setter_too_many_parameters_exception>([&]{
		auto setter = setter_method{method("untagged_invalid_setter(injectable_type1*,injectable_type2*)")};
	});
}

QTEST_APPLESS_MAIN(setter_method_test)
#include "setter-method-test.moc"
