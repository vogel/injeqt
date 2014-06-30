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

#include "implements-extractor.h"
#include "implements-extractor.cpp"

#include <QtTest/QtTest>

using namespace injeqt::internal;

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

class test_implements_extractor : public QObject
{
	Q_OBJECT

private slots:
	void should_find_nothing_in_qobject();
	void should_find_one_in_direct_successor();
	void should_find_two_in_indirect_successor_1();
	void should_find_three_in_indirect_successor_2();

};

void test_implements_extractor::should_find_nothing_in_qobject()
{
	auto implements = implements_extractor{}.extract_implements(QObject::staticMetaObject);
	QCOMPARE(implements.size(), 0UL);
}

void test_implements_extractor::should_find_one_in_direct_successor()
{
	auto implements = implements_extractor{}.extract_implements(direct_successor::staticMetaObject);
	QCOMPARE(implements.size(), 1UL);
	QVERIFY(implements.find(&direct_successor::staticMetaObject) != std::end(implements));
}

void test_implements_extractor::should_find_two_in_indirect_successor_1()
{
	auto implements = implements_extractor{}.extract_implements(indirect_successor_1::staticMetaObject);
	QCOMPARE(implements.size(), 2UL);
	QVERIFY(implements.find(&direct_successor::staticMetaObject) != std::end(implements));
	QVERIFY(implements.find(&indirect_successor_1::staticMetaObject) != std::end(implements));
}

void test_implements_extractor::should_find_three_in_indirect_successor_2()
{
	auto implements = implements_extractor{}.extract_implements(indirect_successor_2::staticMetaObject);
	QCOMPARE(implements.size(), 3UL);
	QVERIFY(implements.find(&direct_successor::staticMetaObject) != std::end(implements));
	QVERIFY(implements.find(&indirect_successor_1::staticMetaObject) != std::end(implements));
	QVERIFY(implements.find(&indirect_successor_2::staticMetaObject) != std::end(implements));
}

QTEST_APPLESS_MAIN(test_implements_extractor);

#include "implements-extractor-test.moc"
