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

#include "internal/dependency.h"
#include "internal/type-dependencies.h"

#include <QtTest/QtTest>
#include <string>

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

class type_with_dependency_1 : public QObject
{
	Q_OBJECT

public slots:
	INJEQT_SET void setter_1(type_1 *) { }
	INJEQT_SET void setter_2(type_2 *) { }

};

class type_with_dependency_2 : public QObject
{
	Q_OBJECT

public slots:
	INJEQT_SET void setter_1(type_1 *) { }
	INJEQT_SET void setter_2(type_2 *) { }

};

class type_dependencies_test : public QObject
{
	Q_OBJECT

private slots:
	void should_properly_compare();

};

void type_dependencies_test::should_properly_compare()
{
	auto d11 = dependency{make_test_setter_method<type_with_dependency_1, type_1>("setter_1(type_1*)")};
	auto d12 = dependency{make_test_setter_method<type_with_dependency_1, type_2>("setter_2(type_2*)")};
	auto d21 = dependency{make_test_setter_method<type_with_dependency_2, type_1>("setter_1(type_1*)")};
	auto d22 = dependency{make_test_setter_method<type_with_dependency_2, type_2>("setter_2(type_2*)")};
	
	auto td_empty = type_dependencies{type{}, dependencies{}};
	auto td1a = type_dependencies{make_type<type_1>(), dependencies{d11, d12}};
	auto td1b = type_dependencies{make_type<type_1>(), dependencies{d11, d12}};
	auto td2a = type_dependencies{make_type<type_1>(), dependencies{}};
	auto td2b = type_dependencies{make_type<type_1>(), dependencies{}};
	auto td3a = type_dependencies{make_type<type_2>(), dependencies{d21, d22}};
	auto td3b = type_dependencies{make_type<type_2>(), dependencies{d21, d22}};

	test_compare<type_dependencies>({{td_empty}, {td1a, td1b}, {td2a, td2b}, {td3a, td3b}});
}

QTEST_APPLESS_MAIN(type_dependencies_test)
#include "type-dependencies-test.moc"
