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

#include <injeqt/exception/qobject-type.h>
#include <injeqt/exception/unique-factory-method-not-found.h>

#include "internal/provider-by-factory-configuration.h"
#include "internal/provider.h"
#include "internal/types.h"

#include <QtTest/QtTest>
#include <memory>

using namespace injeqt::v1;
using namespace injeqt::internal;

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

class factory_type : public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE type_1 * create() { return new type_1{}; }
};

class subtype_factory_type : public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE type_1_subtype_1 * create() { return new type_1_subtype_1{}; }
};

class non_unique_factory_type : public QObject
{
	Q_OBJECT
public:
	Q_INVOKABLE type_1 * create() { return nullptr; }
	Q_INVOKABLE type_1 * create2() { return nullptr; }
};

class provider_by_factory_configuration_test : public QObject
{
	Q_OBJECT

public:
	provider_by_factory_configuration_test();

private slots:
	void should_accept_qobject_object_type_and_throw_on_create();
	void should_accept_qobject_factory_type_and_throw_on_create();
	void should_accept_factory_without_factory_method_and_throw_on_create();
	void should_accept_factory_without_unique_factory_method_and_throw_on_create();
	void should_accept_supertype_factory_and_throw_on_create();
	void should_accept_valid_configuration();
	void should_accept_subtype_configuration();

private:
	types_by_name known_types;

};

provider_by_factory_configuration_test::provider_by_factory_configuration_test()
{
	known_types = types_by_name{std::vector<type>{
		make_type<type_1>(),
		make_type<type_1_subtype_1>(),
		make_type<type_2>(),
		make_type<factory_type>(),
		make_type<subtype_factory_type>(),
		make_type<non_unique_factory_type>()
	}};
}

void provider_by_factory_configuration_test::should_accept_qobject_object_type_and_throw_on_create()
{
	auto pc = provider_by_factory_configuration{make_type<QObject>(), make_type<factory_type>()};

	expect<exception::qobject_type>([&](){
		pc.create_provider(known_types);
	});
}

void provider_by_factory_configuration_test::should_accept_qobject_factory_type_and_throw_on_create()
{
	auto pc = provider_by_factory_configuration{make_type<type_1>(), make_type<QObject>()};

	expect<exception::qobject_type>([&](){
		pc.create_provider(known_types);
	});
}

void provider_by_factory_configuration_test::should_accept_factory_without_factory_method_and_throw_on_create()
{
	auto pc = provider_by_factory_configuration{make_type<type_2>(), make_type<factory_type>()};

	expect<exception::unique_factory_method_not_found>([&](){
		pc.create_provider(known_types);
	});
}

void provider_by_factory_configuration_test::should_accept_factory_without_unique_factory_method_and_throw_on_create()
{
	auto pc = provider_by_factory_configuration{make_type<type_2>(), make_type<non_unique_factory_type>()};

	expect<exception::unique_factory_method_not_found>([&](){
		pc.create_provider(known_types);
	});
}

void provider_by_factory_configuration_test::should_accept_supertype_factory_and_throw_on_create()
{
	auto pc = provider_by_factory_configuration{make_type<type_1_subtype_1>(), make_type<factory_type>()};

	expect<exception::unique_factory_method_not_found>([&](){
		pc.create_provider(known_types);
	});
}

void provider_by_factory_configuration_test::should_accept_valid_configuration()
{
	auto pc = provider_by_factory_configuration{make_type<type_1>(), make_type<factory_type>()};
	auto p = pc.create_provider(known_types);

	QCOMPARE(types{make_type<factory_type>()}, p->required_types());
	QCOMPARE(make_type<type_1>(), p->provided_type());
}

void provider_by_factory_configuration_test::should_accept_subtype_configuration()
{
	auto pc = provider_by_factory_configuration{make_type<type_1>(), make_type<subtype_factory_type>()};
	auto p = pc.create_provider(known_types);

	QCOMPARE(types{make_type<subtype_factory_type>()}, p->required_types());
	QCOMPARE(make_type<type_1_subtype_1>(), p->provided_type());
}

QTEST_APPLESS_MAIN(provider_by_factory_configuration_test)
#include "provider-by-factory-configuration-test.moc"
