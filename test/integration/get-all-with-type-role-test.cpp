/*
 * %injeqt copyright begin%
 * Copyright 2016 Rafał Malinowski (rafal.przemyslaw.malinowski@gmail.com)
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

#include <injeqt/injector.h>
#include <injeqt/module.h>
#include <injeqt/type.h>

#include <QtTest/QtTest>
#include <set>

using injeqt::make_type;
using injeqt::type;

#define ROLE_1 "role1"
#define ROLE_2 "role2"

class role_1_type_1 : public QObject
{
	Q_OBJECT
	INJEQT_TYPE_ROLE(ROLE_1)

public:
	Q_INVOKABLE role_1_type_1() { }
};

class role_1_type_2 : public QObject
{
	Q_OBJECT
	INJEQT_TYPE_ROLE(ROLE_1)

public:
	Q_INVOKABLE role_1_type_2() { }
};

class role_2_type_1 : public QObject
{
	Q_OBJECT
	INJEQT_TYPE_ROLE(ROLE_2)

public:
	Q_INVOKABLE role_2_type_1() { }
};

class role_2_type_2 : public QObject
{
	Q_OBJECT
	INJEQT_TYPE_ROLE(ROLE_2)

public:
	Q_INVOKABLE role_2_type_2() { }
};

class get_all_with_type_role_test : public QObject
{
	Q_OBJECT

private:
	injeqt::injector create_injector();

private slots:
	void should_return_all_role_instances();

};

injeqt::injector get_all_with_type_role_test::create_injector()
{
	class m : public injeqt::module
	{
	public:
		m()
		{
			add_type<role_1_type_1>();
			add_type<role_1_type_2>();
			add_type<role_2_type_1>();
			add_type<role_2_type_2>();
		}
		virtual ~m() {}
	};

	auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
	modules.emplace_back(std::unique_ptr<m>{new m{}});
	return injeqt::injector{std::move(modules)};
}

void get_all_with_type_role_test::should_return_all_role_instances()
{
	auto injector = create_injector();
	auto role_1_objects = injector.get_all_with_type_role(ROLE_1);
	QCOMPARE(role_1_objects.size(), size_t{2});
	QVERIFY(std::any_of(std::begin(role_1_objects), std::end(role_1_objects), [](QObject *o){
		return !!qobject_cast<role_1_type_1 *>(o);
	}));
	QVERIFY(std::any_of(std::begin(role_1_objects), std::end(role_1_objects), [](QObject *o){
		return !!qobject_cast<role_1_type_2 *>(o);
	}));

	auto role_2_object = injector.get<role_2_type_1>();
	auto role_2_objects = injector.get_all_with_type_role(ROLE_2);
	QCOMPARE(role_2_objects.size(), size_t{2});
	QVERIFY(std::any_of(std::begin(role_2_objects), std::end(role_2_objects), [](QObject *o){
		return !!qobject_cast<role_2_type_1 *>(o);
	}));
	QVERIFY(std::any_of(std::begin(role_2_objects), std::end(role_2_objects), [](QObject *o){
		return !!qobject_cast<role_2_type_2 *>(o);
	}));
	QVERIFY(std::any_of(std::begin(role_2_objects), std::end(role_2_objects), [role_2_object](QObject *o){
		return o == role_2_object;
	}));
}

QTEST_APPLESS_MAIN(get_all_with_type_role_test)
#include "get-all-with-type-role-test.moc"
