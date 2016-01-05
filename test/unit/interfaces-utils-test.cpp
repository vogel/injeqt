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

#include <injeqt/type.h>

#include "internal/interfaces-utils.h"

#include <QtTest/QtTest>

using namespace injeqt::internal;
using namespace injeqt::v1;

class direct_successor : public QObject
{
	Q_OBJECT
};

class indirect_successor_1 : public direct_successor
{
	Q_OBJECT
};

class indirect_successor_2 : public indirect_successor_1
{
	Q_OBJECT
};

class interfaces_utils_test : public QObject
{
	Q_OBJECT

public:
	interfaces_utils_test();

private slots:
	void should_find_nothing_in_qobject();
	void should_find_one_in_direct_successor();
	void should_find_two_in_indirect_successor_1();
	void should_find_three_in_indirect_successor_2();

private:
	type qobject_type;
	type direct_successor_type;
	type indirect_successor_1_type;
	type indirect_successor_2_type;

};

interfaces_utils_test::interfaces_utils_test() :
	qobject_type{type(&QObject::staticMetaObject)},
	direct_successor_type{make_type<direct_successor>()},
	indirect_successor_1_type{make_type<indirect_successor_1>()},
	indirect_successor_2_type{make_type<indirect_successor_2>()}
{
}

void interfaces_utils_test::should_find_nothing_in_qobject()
{
	auto interfaces = extract_interfaces(qobject_type);
	QCOMPARE(interfaces.size(), size_t{0});
	QVERIFY(!implements(qobject_type, qobject_type));
}

void interfaces_utils_test::should_find_one_in_direct_successor()
{
	auto interfaces = extract_interfaces(direct_successor_type);
	QCOMPARE(interfaces, (types{direct_successor_type}));
	QVERIFY(implements(direct_successor_type, direct_successor_type));
}

void interfaces_utils_test::should_find_two_in_indirect_successor_1()
{
	auto interfaces = extract_interfaces(indirect_successor_1_type);
	QCOMPARE(interfaces, (types{direct_successor_type, indirect_successor_1_type}));
	QVERIFY(!implements(indirect_successor_1_type, qobject_type));
	QVERIFY(implements(indirect_successor_1_type, direct_successor_type));
	QVERIFY(implements(indirect_successor_1_type, indirect_successor_1_type));
	QVERIFY(!implements(indirect_successor_1_type, indirect_successor_2_type));
}

void interfaces_utils_test::should_find_three_in_indirect_successor_2()
{
	auto interfaces = extract_interfaces(indirect_successor_2_type);
	QCOMPARE(interfaces, (types{direct_successor_type, indirect_successor_1_type, indirect_successor_2_type}));
	QVERIFY(implements(direct_successor_type, direct_successor_type));
	QVERIFY(!implements(indirect_successor_2_type, qobject_type));
	QVERIFY(implements(indirect_successor_2_type, direct_successor_type));
	QVERIFY(implements(indirect_successor_2_type, indirect_successor_1_type));
	QVERIFY(implements(indirect_successor_2_type, indirect_successor_2_type));
}

QTEST_APPLESS_MAIN(interfaces_utils_test);

#include "interfaces-utils-test.moc"
