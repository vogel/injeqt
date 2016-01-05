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

#include <QtTest/QtTest>

#include "internal/types-by-name.h"

using namespace injeqt::internal;
using namespace injeqt::v1;

class type_1 : public QObject
{
	Q_OBJECT
};

class type_2 : public QObject
{
	Q_OBJECT
};

class type_3 : public QObject
{
	Q_OBJECT
};

class types_by_name_test : public QObject
{
	Q_OBJECT

public:
	types_by_name_test();

private slots:
	void should_return_empty_for_empty_name();
	void should_return_empty_for_asterix();
	void should_return_empty_for_type_name();
	void should_return_valid_for_type_name_with_asterix();
	void should_return_empty_for_unknown_type_name_with_asterix();

private:
	types_by_name _known_types;

};

types_by_name_test::types_by_name_test() :
	_known_types{
		make_type<type_1>(),
		make_type<type_2>()
	}
{
}

void types_by_name_test::should_return_empty_for_empty_name()
{
	auto t = type_by_pointer(_known_types, {});
	QVERIFY(t.is_empty());
}

void types_by_name_test::should_return_empty_for_asterix()
{
	auto t = type_by_pointer(_known_types, "*");
	QVERIFY(t.is_empty());
}

void types_by_name_test::should_return_empty_for_type_name()
{
	auto t = type_by_pointer(_known_types, "type_1");
	QVERIFY(t.is_empty());
}

void types_by_name_test::should_return_valid_for_type_name_with_asterix()
{
	auto t1 = type_by_pointer(_known_types, "type_1*");
	QCOMPARE(make_type<type_1>(), t1);

	auto t2 = type_by_pointer(_known_types, "type_2*");
	QCOMPARE(make_type<type_2>(), t2);
}

void types_by_name_test::should_return_empty_for_unknown_type_name_with_asterix()
{
	auto t = type_by_pointer(_known_types, "type_3*");
	QVERIFY(t.is_empty());
}

QTEST_APPLESS_MAIN(types_by_name_test)
#include "types-by-name-test.moc"
