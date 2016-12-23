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

std::set<type> instantiated_types;

class role_1_type : public QObject
{
	Q_OBJECT
	INJEQT_TYPE_ROLE(ROLE_1)

public:
	Q_INVOKABLE role_1_type() { instantiated_types.insert(make_type<role_1_type>()); }
};

class no_role_required_by_role_2 : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE no_role_required_by_role_2() { instantiated_types.insert(make_type<no_role_required_by_role_2>()); }
};

class role_2_type : public QObject
{
	Q_OBJECT
	INJEQT_TYPE_ROLE(ROLE_2)

public:
	Q_INVOKABLE role_2_type() { instantiated_types.insert(make_type<role_2_type>()); }

private slots:
	INJEQT_SET void set_no_role_required_by_role_2(no_role_required_by_role_2 *) {}

};

class instantiate_all_with_type_role_test : public QObject
{
	Q_OBJECT

private:
	injeqt::injector create_injector();

private slots:
	void should_create_all_role_instances_with_dependencied_when_requested();

};

injeqt::injector instantiate_all_with_type_role_test::create_injector()
{
	class m : public injeqt::module
	{
	public:
		m()
		{
			add_type<role_1_type>();
			add_type<no_role_required_by_role_2>();
			add_type<role_2_type>();
		}
		virtual ~m() {}
	};

	auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
	modules.emplace_back(std::unique_ptr<m>{new m{}});
	return injeqt::injector{std::move(modules)};
}

void instantiate_all_with_type_role_test::should_create_all_role_instances_with_dependencied_when_requested()
{
	auto injector = create_injector();
	instantiated_types.clear();
	injector.instantiate_all_with_type_role(ROLE_2);
	QCOMPARE(instantiated_types, (std::set<type>{make_type<role_2_type>(), make_type<no_role_required_by_role_2>()}));

	instantiated_types.clear();
	injector.instantiate_all_with_type_role(ROLE_1);
	QCOMPARE(instantiated_types, (std::set<type>{make_type<role_1_type>()}));
}

QTEST_APPLESS_MAIN(instantiate_all_with_type_role_test)
#include "instantiate-all-with-type-role-test.moc"
