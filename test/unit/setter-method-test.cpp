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

class test_type : public QObject
{
	Q_OBJECT

public:
	injectable_type1 *_1 = nullptr;

	Q_INVOKABLE INJEQT_SET void tagged_setter_method_1(injectable_type1 *a) { _1 = a; }
	Q_INVOKABLE INJEQT_SETTER void tagged_setter_method_2(injectable_type1 *a) { _1 = a; }

public slots:
	INJEQT_SET void tagged_setter_slot_1(injectable_type1 *a) { _1 = a; }
	INJEQT_SETTER void tagged_setter_slot_2(injectable_type1 *a) { _1 = a; }

};

class setter_method_test : public QObject
{
	Q_OBJECT

private slots:
	void should_create_empty();
	void should_create_valid_from_tagged_setter_method();
	void should_create_valid_from_tagged_setter_slot();
	void should_invoke_have_results();

};

void setter_method_test::should_create_empty()
{
	auto setter = setter_method{};
	QVERIFY(setter.is_empty());
}

void setter_method_test::should_create_valid_from_tagged_setter_method()
{
	auto setter = make_setter_method<test_type, injectable_type1>("tagged_setter_method_1(injectable_type1*)");
	QVERIFY(!setter.is_empty());
	QCOMPARE(setter.object_type(), make_type<test_type>());
	QCOMPARE(setter.parameter_type(), make_type<injectable_type1>());

	setter = make_setter_method<test_type, injectable_type1>("tagged_setter_method_2(injectable_type1*)");
	QVERIFY(!setter.is_empty());
	QCOMPARE(setter.object_type(), make_type<test_type>());
	QCOMPARE(setter.parameter_type(), make_type<injectable_type1>());
}

void setter_method_test::should_create_valid_from_tagged_setter_slot()
{
	auto setter = make_setter_method<test_type, injectable_type1>("tagged_setter_slot_1(injectable_type1*)");
	QVERIFY(!setter.is_empty());
	QCOMPARE(setter.object_type(), make_type<test_type>());
	QCOMPARE(setter.parameter_type(), make_type<injectable_type1>());

	setter = make_setter_method<test_type, injectable_type1>("tagged_setter_slot_2(injectable_type1*)");
	QVERIFY(!setter.is_empty());
	QCOMPARE(setter.object_type(), make_type<test_type>());
	QCOMPARE(setter.parameter_type(), make_type<injectable_type1>());
}

void setter_method_test::should_invoke_have_results()
{
	auto setter = make_setter_method<test_type, injectable_type1>("tagged_setter_slot_1(injectable_type1*)");
	auto on = make_object<test_type>();
	auto with = make_object<injectable_type1>();
	QCOMPARE(static_cast<QObject *>(nullptr), static_cast<test_type *>(on.get())->_1);

	setter.invoke(on.get(), with.get());
	QCOMPARE(with.get(), static_cast<test_type *>(on.get())->_1);
}

QTEST_APPLESS_MAIN(setter_method_test)
#include "setter-method-test.moc"
