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
#include "utils.h"

#include "internal/implemented-by.h"

#include <QtTest/QtTest>

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

class type_1_subtype_1 : public type_1
{
	Q_OBJECT
};

class type_1_subtype_1_subtype_1 : public type_1_subtype_1
{
	Q_OBJECT
};

class implemented_by_test : public QObject
{
	Q_OBJECT

private slots:
	void should_accept_implemented_by_self();
	void should_accept_implemented_by_subtype();
	void should_properly_compare();

};

void implemented_by_test::should_accept_implemented_by_self()
{
	auto i = implemented_by{make_type<type_1>(), make_type<type_1>()};

	QCOMPARE(make_type<type_1>(), i.interface_type());
	QCOMPARE(make_type<type_1>(), i.implementation_type());
}

void implemented_by_test::should_accept_implemented_by_subtype()
{
	auto i = implemented_by{make_type<type_1>(), make_type<type_1_subtype_1_subtype_1>()};

	QCOMPARE(make_type<type_1>(), i.interface_type());
	QCOMPARE(make_type<type_1_subtype_1_subtype_1>(), i.implementation_type());
}

void implemented_by_test::should_properly_compare()
{
	auto ib1a = implemented_by{make_type<type_1>(), make_type<type_1_subtype_1>()};
	auto ib1b = implemented_by{make_type<type_1>(), make_type<type_1_subtype_1>()};
	auto ib2a = implemented_by{make_type<type_1>(), make_type<type_1_subtype_1_subtype_1>()};
	auto ib2b = implemented_by{make_type<type_1>(), make_type<type_1_subtype_1_subtype_1>()};
	auto ib3a = implemented_by{make_type<type_1_subtype_1>(), make_type<type_1_subtype_1_subtype_1>()};
	auto ib3b = implemented_by{make_type<type_1_subtype_1>(), make_type<type_1_subtype_1_subtype_1>()};

	test_compare<implemented_by>({{ib1a, ib1b}, {ib2a, ib2b}, {ib3a, ib3b}});
}

QTEST_APPLESS_MAIN(implemented_by_test)
#include "implemented-by-test.moc"
