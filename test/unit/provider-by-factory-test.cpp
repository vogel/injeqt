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

#include <injeqt/exception/instantiation-failed.h>
#include <injeqt/module.h>

#include "internal/factory-method.h"
#include "internal/injector-core.h"
#include "internal/provider-by-default-constructor.h"
#include "internal/provider-by-factory.h"
#include "internal/types-by-name.h"

#include <QtTest/QtTest>
#include <memory>

using namespace injeqt::v1;
using namespace injeqt::internal;

class by_factory_type : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE by_factory_type() {}

};

class factory_type : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE factory_type() {}
	Q_INVOKABLE by_factory_type * create() { return new by_factory_type{}; }

};

class invalid_factory_type : public QObject
{
	Q_OBJECT

public:
	Q_INVOKABLE invalid_factory_type() {}
	Q_INVOKABLE by_factory_type * create() { return nullptr; }

};

class factory_module : public module
{

public:
	factory_module() { add_type<factory_type>(); }
	virtual ~factory_module() {}

};

class provider_by_factory_test : public QObject
{
	Q_OBJECT

public:
	provider_by_factory_test();

private slots:
	void should_return_always_the_same_object();
	void should_throw_instantiation_failed_when_invalid_factory();

private:
	types_by_name known_types;

};

provider_by_factory_test::provider_by_factory_test()
{
	known_types = types_by_name{std::vector<type>{
		make_type<by_factory_type>(),
		make_type<factory_type>(),
		make_type<invalid_factory_type>()
	}};
}

void provider_by_factory_test::should_return_always_the_same_object()
{
	auto fm = make_factory_method(known_types, make_type<by_factory_type>(), make_type<factory_type>());
	auto fp = std::unique_ptr<provider_by_factory>{new provider_by_factory{fm}};
	auto p = fp.get();

	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.emplace_back(std::move(fp));
	configuration.emplace_back(std::unique_ptr<provider_by_default_constructor>{new provider_by_default_constructor{make_default_constructor_method(make_type<factory_type>())}});
	auto injector = injector_core{types_by_name{}, std::move(configuration)};

	QCOMPARE(p->provided_type(), make_type<by_factory_type>());
	QCOMPARE(p->required_types(), types{make_type<factory_type>()});
	QCOMPARE(p->factory(), fm);

	auto o = p->provide(injector);
	QCOMPARE(p->provide(injector), o);
	QCOMPARE(p->provide(injector), o);
	QCOMPARE(o->metaObject(), &by_factory_type::staticMetaObject);
}

void provider_by_factory_test::should_throw_instantiation_failed_when_invalid_factory()
{
	auto fm = make_factory_method(known_types, make_type<by_factory_type>(), make_type<invalid_factory_type>());
	auto fp = std::unique_ptr<provider_by_factory>{new provider_by_factory{fm}};
	auto p = fp.get();

	auto configuration = std::vector<std::unique_ptr<provider>>{};
	configuration.emplace_back(std::move(fp));
	configuration.emplace_back(std::unique_ptr<provider_by_default_constructor>{new provider_by_default_constructor{make_default_constructor_method(make_type<invalid_factory_type>())}});
	auto injector = injector_core{types_by_name{}, std::move(configuration)};

	QCOMPARE(p->provided_type(), make_type<by_factory_type>());
	QCOMPARE(p->required_types(), types{make_type<invalid_factory_type>()});
	QCOMPARE(p->factory(), fm);

	expect<exception::instantiation_failed>({"by_factory_type"}, [&](){
		p->provide(injector);
	});
}

QTEST_APPLESS_MAIN(provider_by_factory_test)
#include "provider-by-factory-test.moc"
