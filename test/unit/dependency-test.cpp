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

#include "exception/empty-type.cpp"
#include "exception/exception.cpp"
#include "exception/interface-not-implemented.cpp"
#include "exception/invalid-qobject.cpp"
#include "exception/qobject-type.cpp"
#include "dependency.cpp"
#include "interfaces-utils.cpp"
#include "setter-method.cpp"
#include "type.cpp"

#include "make-setter-method.h"
#include "utils.h"

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

class type_with_dependency : public QObject
{
	Q_OBJECT

public slots:
	INJEQT_SETTER void setter_1(type_1 *) { }
	INJEQT_SETTER void setter_1b(type_1 *) { }
	INJEQT_SETTER void setter_2(type_2 *) { }

};

class dependency_test : public QObject
{
	Q_OBJECT

private slots:
	void should_create_valid_dependency();
	void should_properly_compare();

};

void dependency_test::should_create_valid_dependency()
{
	auto s = make_setter_method<type_with_dependency>("setter_1(type_1*)");
	auto d = dependency{s};
	QCOMPARE(d.required_type(), make_type<type_1>());
	QCOMPARE(d.setter(), s);
}

void dependency_test::should_properly_compare()
{
	auto s1a = dependency{make_setter_method<type_with_dependency>("setter_1(type_1*)")};
	auto s1b = dependency{make_setter_method<type_with_dependency>("setter_1(type_1*)")};
	auto s2a = dependency{make_setter_method<type_with_dependency>("setter_1b(type_1*)")};
	auto s2b = dependency{make_setter_method<type_with_dependency>("setter_1b(type_1*)")};
	auto s3a = dependency{make_setter_method<type_with_dependency>("setter_2(type_2*)")};
	auto s3b = dependency{make_setter_method<type_with_dependency>("setter_2(type_2*)")};
	
	test_compare<dependency>({{s1a, s1b}, {s2a, s2b}, {s3a, s3b}});
}

QTEST_APPLESS_MAIN(dependency_test)
#include "dependency-test.moc"
