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

#include "internal/type-role.h"

#include <QtTest/QtTest>
#include <string>

using namespace injeqt::internal;
using namespace injeqt::v1;

#define ROLE_1 "role1"
#define ROLE_2 "role2"

class no_role_type : public QObject
{
	Q_OBJECT
};

class role_1_type : public no_role_type
{
	Q_OBJECT
	INJEQT_TYPE_ROLE(ROLE_1)
};

class role_1_inherited_type : public role_1_type
{
	Q_OBJECT
};

class role_1_double_type : public no_role_type
{
	Q_OBJECT
	INJEQT_TYPE_ROLE(ROLE_1)
	INJEQT_TYPE_ROLE(ROLE_1)
};

class role_2_inherited_from_role_1_type : public role_1_type
{
	Q_OBJECT
	INJEQT_TYPE_ROLE(ROLE_2)
};

class role_1_and_2_type : public no_role_type
{
	Q_OBJECT
	INJEQT_TYPE_ROLE(ROLE_1)
	INJEQT_TYPE_ROLE(ROLE_2)
};

class type_role_test : public QObject
{
	Q_OBJECT

private slots:
	void should_have_no_role_by_default();
	void should_have_one_role_when_directly_declared();
	void should_have_one_role_when_declared_in_supertype();
	void should_have_one_role_when_declared_twice();
	void should_have_two_roles_when_added_in_subtype();
	void should_have_two_roles_when_directly_declared();

};

void type_role_test::should_have_no_role_by_default()
{
	QVERIFY(!has_type_role<no_role_type>(ROLE_1));
	QVERIFY(!has_type_role<no_role_type>(ROLE_2));
}

void type_role_test::should_have_one_role_when_directly_declared()
{
	QVERIFY(has_type_role<role_1_type>(ROLE_1));
	QVERIFY(!has_type_role<role_1_type>(ROLE_2));
}

void type_role_test::should_have_one_role_when_declared_in_supertype()
{
	QVERIFY(has_type_role<role_1_inherited_type>(ROLE_1));
	QVERIFY(!has_type_role<role_1_inherited_type>(ROLE_2));
}

void type_role_test::should_have_one_role_when_declared_twice()
{
	QVERIFY(has_type_role<role_1_double_type>(ROLE_1));
	QVERIFY(!has_type_role<role_1_double_type>(ROLE_2));
}

void type_role_test::should_have_two_roles_when_added_in_subtype()
{
	QVERIFY(has_type_role<role_2_inherited_from_role_1_type>(ROLE_1));
	QVERIFY(has_type_role<role_2_inherited_from_role_1_type>(ROLE_2));
}

void type_role_test::should_have_two_roles_when_directly_declared()
{
	QVERIFY(has_type_role<role_1_and_2_type>(ROLE_1));
	QVERIFY(has_type_role<role_1_and_2_type>(ROLE_2));
}

QTEST_APPLESS_MAIN(type_role_test)
#include "type-role-test.moc"
