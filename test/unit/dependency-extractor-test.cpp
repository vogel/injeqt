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

#include "dependency-extractor.h"
#include "dependency-extractor.cpp"

#include <QtTest/QtTest>

class C1 : public QObject
{
	Q_OBJECT
};

class C2 : public QObject
{
	Q_OBJECT
};

class C3 : public QObject
{
	Q_OBJECT

public slots:
	injeqt_setter void set_c1(C1 *c1) {}
	injeqt_setter void set_c2(C2 *c2) {}

};

class test_dependency_extractor : public QObject
{
	Q_OBJECT

private slots:
	void shouldFindAllValidDependencies();
};

void test_dependency_extractor::shouldFindAllValidDependencies()
{
	auto dependencies = injeqt::details::dependency_extractor{}.extract_dependencies(C1::staticMetaObject);
}

QTEST_APPLESS_MAIN(test_dependency_extractor);

#include "dependency-extractor-test.moc"
