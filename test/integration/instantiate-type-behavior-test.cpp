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

std::set<type> instantiated_types;

class on_demand : public QObject
{
	Q_OBJECT
	INJEQT_INSTANCE_ON_DEMAND

public:
	Q_INVOKABLE on_demand() { instantiated_types.insert(make_type<on_demand>()); }
};

class on_demand_required_by_immediate : public QObject
{
	Q_OBJECT
	INJEQT_INSTANCE_ON_DEMAND

public:
	Q_INVOKABLE on_demand_required_by_immediate() { instantiated_types.insert(make_type<on_demand_required_by_immediate>()); }
};

class immediate : public QObject
{
	Q_OBJECT
	INJEQT_INSTANCE_IMMEDIATE

public:
	Q_INVOKABLE immediate() { instantiated_types.insert(make_type<immediate>()); }

private slots:
	INJEQT_SET void set_on_demand_required_by_immediate(on_demand_required_by_immediate *) {}

};

class instantiate_type_behavior_test : public QObject
{
	Q_OBJECT

private:
	injeqt::injector create_injector();

private slots:
	void should_only_create_immediate_types_on_start_and_on_demand_when_requsted();

};

injeqt::injector instantiate_type_behavior_test::create_injector()
{
	instantiated_types.clear();

	class m : public injeqt::module
	{
	public:
		m()
		{
			add_type<on_demand>();
			add_type<on_demand_required_by_immediate>();
			add_type<immediate>();
		}
		virtual ~m() {}
	};

	auto modules = std::vector<std::unique_ptr<injeqt::module>>{};
	modules.emplace_back(std::unique_ptr<m>{new m{}});
	return injeqt::injector{std::move(modules)};
}

void instantiate_type_behavior_test::should_only_create_immediate_types_on_start_and_on_demand_when_requsted()
{
	auto injector = create_injector();
	QCOMPARE((std::set<type>{make_type<immediate>(), make_type<on_demand_required_by_immediate>()}), instantiated_types);

	injector.get<on_demand>();
	QCOMPARE((std::set<type>{make_type<immediate>(), make_type<on_demand_required_by_immediate>(), make_type<on_demand>()}), instantiated_types);
}

QTEST_APPLESS_MAIN(instantiate_type_behavior_test)
#include "instantiate-type-behavior-test.moc"
