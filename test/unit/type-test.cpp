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
#include "type.cpp"

#include "expect.h"
#include "utils.h"

#include <QtTest/QtTest>

using namespace injeqt::v1;

class type_test : public QObject
{
	Q_OBJECT

private slots:
	void should_return_empty_when_created_with_null_meta_object();
	void should_return_qobject_when_created_with_qobject_meta_object();
	void should_return_valid_meta_object();
	void should_properly_compare();

};

class valid_type : public QObject
{
	Q_OBJECT
};

class valid_type2 : public QObject
{
	Q_OBJECT
};

void type_test::should_return_empty_when_created_with_null_meta_object()
{
	auto t = type{nullptr};
	QVERIFY(t.is_empty());
}

void type_test::should_return_qobject_when_created_with_qobject_meta_object()
{
	auto t = make_type<QObject>();
	QVERIFY(!t.is_empty());
	QVERIFY(t.is_qobject());
}

void type_test::should_return_valid_meta_object()
{
	auto t = make_type<valid_type>();

	QCOMPARE(t.meta_object(), &valid_type::staticMetaObject);
}

void type_test::should_properly_compare()
{
	auto t1a = type{};
	auto t1b = type{};
	auto t2a = make_type<QObject>();
	auto t2b = make_type<QObject>();
	auto t3a = make_type<valid_type>();
	auto t3b = make_type<valid_type>();
	auto t4a = make_type<valid_type2>();
	auto t4b = make_type<valid_type2>();

	test_compare_ex<type>({{t1a, t1b}, {t2a, t2b}, {t3a, t3b}, {t4a, t4b}});
}

QTEST_APPLESS_MAIN(type_test)
#include "type-test.moc"
